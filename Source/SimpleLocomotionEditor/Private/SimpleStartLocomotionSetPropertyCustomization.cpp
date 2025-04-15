// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleStartLocomotionSetPropertyCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "SimpleTags.h"
#include "SimpleLocomotionSets.h"
#include "System/SimpleLocomotionVersioning.h"

TSharedRef<IPropertyTypeCustomization> FSimpleStartLocomotionSetPropertyCustomization::MakeInstance()
{
	return MakeShared<FSimpleStartLocomotionSetPropertyCustomization>();
}

void FSimpleStartLocomotionSetPropertyCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	];
}

void FSimpleStartLocomotionSetPropertyCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	// Add Mode property
	const TSharedPtr<IPropertyHandle> ModeProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStartLocomotionSet, Mode));
	ChildBuilder.AddProperty(ModeProperty.ToSharedRef());

	// Force a refresh on the details panel when Mode property changes
	ModeProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([this, &CustomizationUtils]()
	{
#if UE_5_04_OR_LATER
		CustomizationUtils.GetPropertyUtilities().Get()->RequestForceRefresh();
#else
		CustomizationUtils.GetPropertyUtilities().Get()->ForceRefresh();
#endif
	}));

	// Determine the mode tag
	FString ModeTagName;
	FGameplayTag ModeTag;
	if (ModeProperty->GetValueAsFormattedString(ModeTagName) == FPropertyAccess::Success)
	{
		// Remove the prefix and suffix to get the actual tag
		ModeTagName.RemoveFromStart(TEXT("(TagName=\""));
		ModeTagName.RemoveFromEnd(TEXT("\")"));

		ModeTag = FGameplayTag::RequestGameplayTag(FName(*ModeTagName));
	}
	
	// Form booleans to conditionally toggle visibility
	const bool b1Way = ModeTag == FSimpleTags::Simple_Mode_Start_1Way;
	const bool b4Way = ModeTag == FSimpleTags::Simple_Mode_Start_4Way;
	const bool b8Way = ModeTag == FSimpleTags::Simple_Mode_Start_8Way;
	
	// Add with conditional visibility
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStartLocomotionSet, Forward)), true);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStartLocomotionSet, ForwardLeft)), b8Way);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStartLocomotionSet, ForwardRight)), b8Way);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStartLocomotionSet, Left)), b4Way || b8Way);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStartLocomotionSet, Right)), b4Way || b8Way);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStartLocomotionSet, BackwardTurnLeft)), !b1Way);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStartLocomotionSet, BackwardTurnRight)), !b1Way);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStartLocomotionSet, BackwardLeft)), b8Way);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStartLocomotionSet, BackwardRight)), b8Way);

	// Add CardinalType property
	const TSharedPtr<IPropertyHandle> CardinalTypeProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStartLocomotionSet, CardinalType));
	ChildBuilder.AddProperty(CardinalTypeProperty.ToSharedRef());
}

void FSimpleStartLocomotionSetPropertyCustomization::AddPropertyWithVisibility(IDetailChildrenBuilder& ChildBuilder, const TSharedPtr<IPropertyHandle>& PropertyHandle, bool bVisible)
{
	IDetailPropertyRow& PropertyRow = ChildBuilder.AddProperty(PropertyHandle.ToSharedRef());
	PropertyRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateLambda([=]()
	{
		return bVisible ? EVisibility::Visible : EVisibility::Collapsed;
	})));
}