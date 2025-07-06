// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "SimpleTypes.generated.h"

/** Call from ACharacter::Landed or equivalent */
DECLARE_DYNAMIC_DELEGATE_OneParam(FSimpleLandedSignature, const FHitResult&, Hit);

DECLARE_DELEGATE_ThreeParams(FSimpleCardinalUpdate, const FGameplayTag&, struct FSimpleCardinal&, const struct FSimpleCardinals&);

class UAnimSequence;

UENUM(BlueprintType)
enum class ESimpleIsValidResult : uint8
{
	Valid,
	NotValid,
};

UENUM(BlueprintType)
enum class ESimpleCardinalCache : uint8
{
	CachedEnabledOnly,
	IncludeDisabled,
};

UENUM(BlueprintType)
enum class ESimpleCardinalType : uint8
{
	Acceleration			UMETA(Tooltip="Local Space Acceleration"),
	Velocity				UMETA(Tooltip="Local Space Velocity"),
};

/**
 * Container holding max speeds for each gait mode in use
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleGaitSpeed
{
	GENERATED_BODY()
	
	FSimpleGaitSpeed()
	{}

	FSimpleGaitSpeed(const TMap<FGameplayTag, float>& InMaxSpeeds)
		: MaxSpeeds(InMaxSpeeds)
	{}

	UPROPERTY(BlueprintReadOnly, Category=Properties)
	TMap<FGameplayTag, float> MaxSpeeds;

	float GetMaxSpeed(const FGameplayTag& GaitTag);
};

/**
 * Velocity and Acceleration data handling pertaining to locomotion movement
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleMovement
{
	GENERATED_BODY()
	
	FSimpleMovement(
		const FVector& InVelocity = FVector::ZeroVector,
		const FVector& InAcceleration = FVector::ZeroVector
		)
		: Velocity(InVelocity)
		, Acceleration(InAcceleration)
	{}
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	FVector Velocity;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	FVector Acceleration;

	FSimpleMovement Get2D() const
	{
		const FVector Velocity2D = Velocity * FVector{ 1.f, 1.f, 0.f };
		const FVector Acceleration2D = Acceleration * FVector{ 1.f, 1.f, 0.f };
		return FSimpleMovement { Velocity2D, Acceleration2D };
	}
	
	FSimpleMovement GetLocal(const FRotator& WorldRotation) const
	{
		return FSimpleMovement {
			WorldRotation.UnrotateVector(Velocity), WorldRotation.UnrotateVector(Acceleration)
		};
	}

	float GetSpeed() const
	{
		return Velocity.Size();
	}
};

/**
 * Represents a single Cardinal Mode (FSimpleGameplayTags "Simple.State")
 * Directional cardinal states, data, and update delegate handling
 * Contained by FSimpleCardinals
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleCardinal
{
	GENERATED_BODY()

	FSimpleCardinal()
		: bEnabled(false)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Properties)
	bool bEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Properties, meta=(GameplayTagFilter="Simple.Cardinal", EditCondition="bEnabled", EditConditionHides))
	FGameplayTagContainer Tags;

	UPROPERTY(BlueprintReadOnly, Category=Properties, meta=(GameplayTagFilter="Simple.Cardinal"))
	FGameplayTag Acceleration;

	UPROPERTY(BlueprintReadOnly, Category=Properties, meta=(GameplayTagFilter="Simple.Cardinal"))
	FGameplayTag Velocity;

	/** Bound in USimpleAnimInstance::NativeInitializeAnimation */
	FSimpleCardinalUpdate UpdateDelegate;

	FGameplayTag GetCardinal(ESimpleCardinalType CardinalType) const;
};

/**
 * Maintains data pertaining to each cardinal mode that is in use
 * Responsible for updating the data and converting it to a usable result
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleCardinals
{
	GENERATED_BODY()
	
	FSimpleCardinals(
		float InVelocity = 0.f,
		float InAcceleration = 0.f
		)
		: Velocity(InVelocity)
		, Acceleration(InAcceleration)
		, bHasEverUpdated(false)
		, bHasCachedCardinals(false)
	{
		ConstructDefaultCardinals();
	}
	
	FSimpleCardinals(
		const FSimpleMovement& World2D,
		const FRotator& WorldRotation
		)
		: bHasEverUpdated(false)
		, bHasCachedCardinals(false)
	{
		ConstructDefaultCardinals();
		ThreadSafeUpdate_Internal(World2D, WorldRotation);
	}

	void ConstructDefaultCardinals(bool bEnableDefaultCardinals = true);
	
	static FGameplayTagContainer ConstructCardinalTags_1Way();
	static FGameplayTagContainer ConstructCardinalTags_2Way();
	static FGameplayTagContainer ConstructCardinalTags_4Way();
	static FGameplayTagContainer ConstructCardinalTags_6Way();
	static FGameplayTagContainer ConstructCardinalTags_8Way();
	static FGameplayTagContainer ConstructCardinalTags_10Way();
	
	static FGameplayTagContainer ConstructCardinalTags_Start_1Way();
	static FGameplayTagContainer ConstructCardinalTags_Start_2Way();
	static FGameplayTagContainer ConstructCardinalTags_Start_4Way();
	static FGameplayTagContainer ConstructCardinalTags_Start_8Way();
	
	static FGameplayTagContainer ConstructCardinalTags_Turn_1Way();
	static FGameplayTagContainer ConstructCardinalTags_Turn_4Way();
	static FGameplayTagContainer ConstructCardinalTags_Turn_8Way();

	void SetCardinalEnabled(const FGameplayTag& CardinalModeTag, bool bEnabled);

	UPROPERTY(BlueprintReadOnly, Category=Properties)
	float Velocity;

	UPROPERTY(BlueprintReadOnly, Category=Properties)
	float Acceleration;

	UPROPERTY(BlueprintReadOnly, Category=Properties)
	bool bHasEverUpdated;

	/**
	 * Get current cardinal based on Simple.Mode and CardinalType, e.g. Simple.Mode.Strafe.1Way and ESimpleCardinalType::Acceleration
	 * @return Simple.Cardinal, e.g. Simple.Cardinal.Forward.Left
	 */
	FGameplayTag GetCurrentCardinal(const FGameplayTag& CardinalModeTag, ESimpleCardinalType CardinalType) const;

	FGameplayTag GetCurrentCardinal(const struct FSimpleStrafeLocoSet& LocomotionSet) const;
	FGameplayTag GetCurrentCardinal(const struct FSimpleStrafeLocoSet* LocomotionSet) const;
	FGameplayTag GetCurrentCardinal(const struct FSimpleStartLocoSet& LocomotionSet) const;
	FGameplayTag GetCurrentCardinal(const struct FSimpleStartLocoSet* LocomotionSet) const;
	FGameplayTag GetCurrentCardinal(const struct FSimpleTurnLocoSet& LocomotionSet) const;
	FGameplayTag GetCurrentCardinal(const struct FSimpleTurnLocoSet* LocomotionSet) const;
	
	float GetDirectionAngle(ESimpleCardinalType CardinalType) const;

	void ThreadSafeUpdate(const FSimpleMovement& World2D, const FRotator& WorldRotation);

	static float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation);

	void UpdateCardinals(const TMap<FGameplayTag, FSimpleCardinal>& NewCardinals);
	const TMap<FGameplayTag, FSimpleCardinal>& GetDefaultCardinals() const { return Cardinals; }
	
	void CacheCardinals();

	const TMap<FGameplayTag, FSimpleCardinal>& GetCardinals(ESimpleCardinalCache Mode = ESimpleCardinalCache::CachedEnabledOnly) const;
	TMap<FGameplayTag, FSimpleCardinal>& GetCardinals(ESimpleCardinalCache Mode = ESimpleCardinalCache::CachedEnabledOnly);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category=Properties)
	bool bHasCachedCardinals;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Properties, meta=(GameplayTagFilter="Simple.Mode"))
	TMap<FGameplayTag, FSimpleCardinal> Cardinals;

	/** Cache only the cardinals that are enabled */
	UPROPERTY(BlueprintReadOnly, Category=Properties, meta=(GameplayTagFilter="Simple.Mode"))
	TMap<FGameplayTag, FSimpleCardinal> CachedCardinals;

	void ThreadSafeUpdate_Internal(const FSimpleMovement& World2D, const FRotator& WorldRotation);
};

