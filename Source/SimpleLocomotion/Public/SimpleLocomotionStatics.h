// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleLocomotionStatics.generated.h"

enum class ESimpleCardinalType : uint8;
struct FSimpleCardinals;
struct FSimpleLocomotionSet;
struct FSimpleGaitSet;
/**
 * 
 */
UCLASS()
class SIMPLELOCOMOTION_API USimpleLocomotionStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	static FGameplayTag GetSimpleCardinal(const FSimpleCardinals& Cardinals, const FSimpleLocomotionSet& LocomotionSet);
	
	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	static FGameplayTag GetSimpleCardinalForTag(const FSimpleCardinals& Cardinals, FGameplayTag CardinalMode, ESimpleCardinalType CardinalType);

	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	static FGameplayTag SelectSimpleCardinalFromAngle(const FGameplayTag& CardinalMode, float Angle, float DeadZone, const FGameplayTag& CurrentDirection, bool bWasMovingLastUpdate);

	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	static FGameplayTag GetSimpleOppositeCardinal(const FGameplayTag& CardinalTag);

	UFUNCTION(BlueprintPure, Category=Animation, meta=(Categories="Simple.Gait", BlueprintThreadSafe))
	static FSimpleLocomotionSet GetSimpleLocomotionSet(const FSimpleGaitSet& GaitSet, FGameplayTag GaitTag, bool& bValid);

	UFUNCTION(BlueprintPure, Category=Animation, meta=(Categories="Simple.Cardinal", BlueprintThreadSafe))
	static UAnimSequence* GetSimpleAnimation(const FSimpleLocomotionSet& LocomotionSet, FGameplayTag CardinalTag, bool& bValid);
};
