// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleStrafeLocomotionSetPropertyCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "SimpleTags.h"
#include "SimpleLocomotionSets.h"
#include "System/SimpleLocomotionVersioning.h"

TSharedRef<IPropertyTypeCustomization> FSimpleStrafeLocomotionSetPropertyCustomization::MakeInstance()
{
	return MakeShared<FSimpleStrafeLocomotionSetPropertyCustomization>();
}

void FSimpleStrafeLocomotionSetPropertyCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	];
}

void FSimpleStrafeLocomotionSetPropertyCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	// Add Mode property
	const TSharedPtr<IPropertyHandle> ModeProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeLocomotionSet, Mode));
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
	const bool b2Way = ModeTag == FSimpleTags::Simple_Mode_Strafe_2Way;
	const bool b4Way = ModeTag == FSimpleTags::Simple_Mode_Strafe_4Way;
	const bool b6Way = ModeTag == FSimpleTags::Simple_Mode_Strafe_6Way;
	const bool b8Way = ModeTag == FSimpleTags::Simple_Mode_Strafe_8Way;
	const bool b10Way = ModeTag == FSimpleTags::Simple_Mode_Strafe_10Way;

	const bool bStrafe = b4Way || b6Way || b8Way || b10Way;
	const bool bLateral = bStrafe || b2Way;
	const bool bDiagonal = b8Way || b10Way;
	const bool bAway = b6Way || b10Way;
	
	// Add with conditional visibility
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeLocomotionSet, InertialBlendTime)), true);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeLocomotionSet, Forward)), !b2Way);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeLocomotionSet, ForwardLeft)), bDiagonal);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeLocomotionSet, ForwardRight)), bDiagonal);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeLocomotionSet, Left)), bLateral);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeLocomotionSet, Right)), bLateral);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeLocomotionSet, LeftAway)), bAway);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeLocomotionSet, RightAway)), bAway);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeLocomotionSet, Backward)), bStrafe);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeLocomotionSet, BackwardLeft)), bDiagonal);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeLocomotionSet, BackwardRight)), bDiagonal);

	// Add CardinalType property
	const TSharedPtr<IPropertyHandle> CardinalTypeProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeLocomotionSet, CardinalType));
	ChildBuilder.AddProperty(CardinalTypeProperty.ToSharedRef());
}

void FSimpleStrafeLocomotionSetPropertyCustomization::AddPropertyWithVisibility(IDetailChildrenBuilder& ChildBuilder, const TSharedPtr<IPropertyHandle>& PropertyHandle, bool bVisible)
{
	IDetailPropertyRow& PropertyRow = ChildBuilder.AddProperty(PropertyHandle.ToSharedRef());
	PropertyRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateLambda([=]()
	{
		return bVisible ? EVisibility::Visible : EVisibility::Collapsed;
	})));
}