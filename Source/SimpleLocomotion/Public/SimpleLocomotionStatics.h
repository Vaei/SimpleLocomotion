// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SimpleLocomotionSets.h"
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
	/** @return Simple.Cardinal, e.g. Simple.Cardinal.Forward.Left */
	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	static FGameplayTag GetSimpleStrafeCardinal(const FSimpleCardinals& Cardinals, const FSimpleStrafeLocomotionSet& Set);

	/** @return Simple.Cardinal, e.g. Simple.Cardinal.Forward.Left */
	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	static FGameplayTag GetSimpleStartCardinal(const FSimpleCardinals& Cardinals, const FSimpleStartLocomotionSet& Set);
	
	/** @return Simple.Cardinal, e.g. Simple.Cardinal.Forward.Left */
	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	static FGameplayTag GetSimpleTurnCardinal(const FSimpleCardinals& Cardinals, const FSimpleTurnLocomotionSet& Set);

	/** @return Simple.Cardinal, e.g. Simple.Cardinal.Forward.Left */
	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	static FGameplayTag GetSimpleCardinalForTag(const FSimpleCardinals& Cardinals, FGameplayTag CardinalMode,
		ESimpleCardinalType CardinalType);

	/**
	 * Commonly used for splitting a float angle into cardinal sections, e.g. 90.0f becomes Right - Takes a Simple.Mode and outputs Simple.Cardinal
	 * @param CardinalMode - The Simple.Mode to use for the cardinal direction
	 * @param Angle - The angle to convert to a cardinal direction
	 * @param DeadZone - The deadzone to use for the angle
	 * @param CurrentDirection - The current Simple.Cardinal direction of the character
	 * @param bWasMovingLastUpdate - Whether the character was moving last update
	 * @return The Simple.Cardinal direction based on the angle
	 */
	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe, GameplayTagFilter="Simple.Mode"))
	static FGameplayTag SelectSimpleCardinalFromAngle(const FGameplayTag& CardinalMode, float Angle, float DeadZone,
		const FGameplayTag& CurrentDirection, bool bWasMovingLastUpdate);

	/**
	 * Commonly used for splitting a float angle into cardinal turn sections for forward start and pivot anims, e.g. -178.0f becomes BackwardTurnLeft, 75.0f becomes Right
	 * @warning You probably don't want to use this for starts; use the existing SelectSimpleCardinalFromAngle() function instead and then convert to BackwardToLeft/Right based on the angle being positive or negative
	 * @note Only supports 4-way and 8-way cardinal modes
	 * @param CardinalMode - The Simple.Mode to use for the cardinal direction
	 * @param Angle - The angle to convert to a cardinal direction
	 * @return The Simple.Cardinal direction based on the angle
	 */
	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe, GameplayTagFilter="Simple.Mode"))
	static FGameplayTag SelectBasicCardinalFromAngle(const FGameplayTag& CardinalMode, float Angle);
	
	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	static FGameplayTag GetSimpleOppositeCardinal(const FGameplayTag& CardinalTag);

	/**
	 * Downgrade a cardinal to a basic cardinal, e.g. Simple.Cardinal.Left.Away to Simple.Cardinal.Left
	 * Downgrade a turn backwards cardinal to a strafe backwards cardinal, e.g. Simple.Cardinal.BackwardTurnLeft to Simple.Cardinal.Backward
	 * Downgrade an 8-way cardinal to a 4-way cardinal, e.g. Simple.Cardinal.Forward.Left to Simple.Cardinal.Forward
	 * @warning Does not downgrade strafe cardinals to 1-way
	 * @return The downgraded cardinal tag, or FGameplayTag::EmptyTag if no downgrade occurred
	 */
	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	static FGameplayTag GetDowngradedCardinal(const FGameplayTag& CardinalTag);

	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	static bool IsMovementTurningRight(const FVector& Acceleration, const FVector& Velocity, const FVector& ActorRightVector);
};
