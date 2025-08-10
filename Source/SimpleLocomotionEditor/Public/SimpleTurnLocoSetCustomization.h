// Copyright (c) Jared Taylor

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class SIMPLELOCOMOTIONEDITOR_API FSimpleTurnLocoSetCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

protected:
	static void AddPropertyWithVisibility(IDetailChildrenBuilder& ChildBuilder, const TSharedPtr<IPropertyHandle>& PropertyHandle, bool bVisible);
};
