// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SimpleLocomotionTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleLocomotionStatics.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLELOCOMOTION_API USimpleLocomotionStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	static FGameplayTag GetSimpleCardinal(const FSimpleCardinals& Cardinals, const FSimpleLocomotionSet& Set);
	
	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	static FGameplayTag GetSimpleCardinalForTag(const FSimpleCardinals& Cardinals, FGameplayTag CardinalMode,
		ESimpleCardinalType CardinalType);

	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	static FGameplayTag SelectSimpleCardinalFromAngle(const FGameplayTag& CardinalMode, float Angle, float DeadZone,
		const FGameplayTag& CurrentDirection, bool bWasMovingLastUpdate);

	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	static FGameplayTag GetSimpleOppositeCardinal(const FGameplayTag& CardinalTag);
};
