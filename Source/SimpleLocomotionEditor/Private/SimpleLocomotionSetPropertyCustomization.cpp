// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleLocomotionSetPropertyCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "SimpleGameplayTags.h"
#include "SimpleLocomotionTypes.h"
#include "System/SimpleLocomotionVersioning.h"

TSharedRef<IPropertyTypeCustomization> FSimpleLocomotionSetPropertyCustomization::MakeInstance()
{
	return MakeShared<FSimpleLocomotionSetPropertyCustomization>();
}

void FSimpleLocomotionSetPropertyCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	];
}

void FSimpleLocomotionSetPropertyCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	// Add Mode property
	TSharedPtr<IPropertyHandle> ModeProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleLocomotionSet, Mode));
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
	const bool b2Way = ModeTag == FSimpleGameplayTags::Simple_Cardinal_Mode_2Way;
	const bool b4Way = ModeTag == FSimpleGameplayTags::Simple_Cardinal_Mode_4Way;
	const bool b6Way = ModeTag == FSimpleGameplayTags::Simple_Cardinal_Mode_6Way;
	const bool b8Way = ModeTag == FSimpleGameplayTags::Simple_Cardinal_Mode_8Way;
	const bool b10Way = ModeTag == FSimpleGameplayTags::Simple_Cardinal_Mode_10Way;

	const bool bStrafe = b4Way || b6Way || b8Way || b10Way;
	const bool bLateral = bStrafe || b2Way;
	const bool bDiagonal = b8Way || b10Way;
	const bool bAway = b6Way || b10Way;
	
	// Add with conditional visibility
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleLocomotionSet, Forward)), !b2Way);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleLocomotionSet, ForwardLeft)), bDiagonal);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleLocomotionSet, ForwardRight)), bDiagonal);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleLocomotionSet, Left)), bLateral);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleLocomotionSet, Right)), bLateral);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleLocomotionSet, LeftAway)), bAway);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleLocomotionSet, RightAway)), bAway);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleLocomotionSet, Backward)), bStrafe);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleLocomotionSet, BackwardLeft)), bDiagonal);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleLocomotionSet, BackwardRight)), bDiagonal);

	// Add CardinalType property
	TSharedPtr<IPropertyHandle> CardinalTypeProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleLocomotionSet, CardinalType));
	ChildBuilder.AddProperty(CardinalTypeProperty.ToSharedRef());
}

void FSimpleLocomotionSetPropertyCustomization::AddPropertyWithVisibility(IDetailChildrenBuilder& ChildBuilder, const TSharedPtr<IPropertyHandle>& PropertyHandle, bool bVisible)
{
	IDetailPropertyRow& PropertyRow = ChildBuilder.AddProperty(PropertyHandle.ToSharedRef());
	PropertyRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateLambda([=]()
	{
		return bVisible ? EVisibility::Visible : EVisibility::Collapsed;
	})));
}