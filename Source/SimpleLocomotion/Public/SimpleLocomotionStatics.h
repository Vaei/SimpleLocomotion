// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleLocomotionStatics.generated.h"

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
	UFUNCTION(BlueprintPure, Category=Animation)
	static FGameplayTag SelectCardinalFromAngle(const FGameplayTag& CardinalMode, float Angle, float DeadZone, const FGameplayTag& CurrentDirection, bool bWasMovingLastUpdate);

	static UAnimSequence* GetSimpleLocomotionAnimation(const FSimpleLocomotionSet* LocomotionSet, const FSimpleCardinals& Cardinals);

	UFUNCTION(BlueprintPure, Category=Animation, meta=(Categories="Simple.Gait"))
	static UAnimSequence* GetSimpleLocomotionAnimation(const FSimpleLocomotionSet& LocomotionSet, const FSimpleCardinals& Cardinals);

	UFUNCTION(BlueprintPure, Category=Animation, meta=(Categories="Simple.Gait"))
	static UAnimSequence* GetSimpleLocomotionAnimationFromTag(const FSimpleLocomotionSet& LocomotionSet, FGameplayTag CardinalTag);
	
	UFUNCTION(BlueprintPure, Category=Animation, meta=(Categories="Simple.Gait"))
	static UAnimSequence* GetSimpleLocomotionAnimationFromGaitSet(const FSimpleGaitSet& GaitSet, FGameplayTag GaitTag, FGameplayTag CardinalTag);
	
	UFUNCTION(BlueprintPure, Category=Animation, meta=(Categories="Simple.Gait"))
	static FSimpleLocomotionSet GetSimpleLocomotionSet(const FSimpleGaitSet& GaitSet, FGameplayTag GaitTag, bool& bValid);
};
