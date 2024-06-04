// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

/**
 * 
 */
class FSimpleAnimComponentDetailsCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};
