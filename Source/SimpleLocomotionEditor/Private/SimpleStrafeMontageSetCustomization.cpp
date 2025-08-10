// Copyright (c) Jared Taylor


#include "SimpleStrafeMontageSetCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "SimpleTags.h"
#include "SimpleSets.h"
#include "System/SimpleLocomotionVersioning.h"

TSharedRef<IPropertyTypeCustomization> FSimpleStrafeMontageSetCustomization::MakeInstance()
{
	return MakeShared<FSimpleStrafeMontageSetCustomization>();
}

void FSimpleStrafeMontageSetCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	];
}

void FSimpleStrafeMontageSetCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	// Add Mode property
	const TSharedPtr<IPropertyHandle> ModeProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeMontageSet, Mode));
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
	const bool b8Way = ModeTag == FSimpleTags::Simple_Mode_Strafe_8Way;

	const bool bStrafe = b4Way || b8Way;
	const bool bLateral = bStrafe || b2Way;
	const bool bDiagonal = b8Way;

	// Add with conditional visibility
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeMontageSet, Forward)), !b2Way);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeMontageSet, ForwardLeft)), bDiagonal);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeMontageSet, ForwardRight)), bDiagonal);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeMontageSet, Left)), bLateral);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeMontageSet, Right)), bLateral);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeMontageSet, Backward)), bStrafe);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeMontageSet, BackwardLeft)), bDiagonal);
	AddPropertyWithVisibility(ChildBuilder, PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleStrafeMontageSet, BackwardRight)), bDiagonal);
}

void FSimpleStrafeMontageSetCustomization::AddPropertyWithVisibility(IDetailChildrenBuilder& ChildBuilder, const TSharedPtr<IPropertyHandle>& PropertyHandle, bool bVisible)
{
	IDetailPropertyRow& PropertyRow = ChildBuilder.AddProperty(PropertyHandle.ToSharedRef());
	PropertyRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateLambda([=]()
	{
		return bVisible ? EVisibility::Visible : EVisibility::Collapsed;
	})));
}