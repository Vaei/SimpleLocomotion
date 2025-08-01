// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SimpleAnimInstanceBase.generated.h"

/**
 * Simply receives details customization, nothing more
 * Layers don't typically have any logic, but we still want to be able to mark categories as important or hidden
 */
UCLASS()
class SIMPLELOCOMOTION_API USimpleAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	
#if WITH_EDITORONLY_DATA
	/** Assist designers by cleaning out categories they don't need to see */
	UPROPERTY(EditDefaultsOnly, Category=Editor)
	TArray<FName> HideEditorCategories = { "RootMotion", "Notifies", "Montage" };
	
	UPROPERTY(EditDefaultsOnly, Category=Editor)
	TArray<FName> ImportantEditorCategories = {};
#endif
};
