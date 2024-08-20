// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "SimpleLocomotionTypes.generated.h"

/** Call from ACharacter::Landed or equivalent */
DECLARE_DYNAMIC_DELEGATE_OneParam(FSimpleLandedSignature, const FHitResult&, Hit);

DECLARE_DELEGATE_ThreeParams(FSimpleCardinalUpdate, const FGameplayTag&, struct FSimpleCardinal&, const struct FSimpleCardinals&);

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
	AccelerationNoOffset	UMETA(Tooltip="Local Space Acceleration with RootYawOffset negated"),
	Velocity				UMETA(Tooltip="Local Space Velocity"),
	VelocityNoOffset		UMETA(Tooltip="Local Space Velocity with RootYawOffset negated"),
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

	float GetMaxSpeed(const FGameplayTag& Gait);
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
 * Represents a single Cardinal Mode (FSimpleGameplayTags "Simple.Mode")
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Properties, meta=(Categories="Simple.Cardinal", EditCondition="bEnabled", EditConditionHides))
	FGameplayTagContainer Tags;

	UPROPERTY(BlueprintReadOnly, Category=Properties, meta=(Categories="Simple.Cardinal"))
	FGameplayTag Acceleration;

	UPROPERTY(BlueprintReadOnly, Category=Properties, meta=(Categories="Simple.Cardinal"))
	FGameplayTag AccelerationNoOffset;

	UPROPERTY(BlueprintReadOnly, Category=Properties, meta=(Categories="Simple.Cardinal"))
	FGameplayTag Velocity;

	UPROPERTY(BlueprintReadOnly, Category=Properties, meta=(Categories="Simple.Cardinal"))
	FGameplayTag VelocityNoOffset;

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
		float InVelocityNoOffset = 0.f,
		float InAcceleration = 0.f,
		float InAccelerationNoOffset = 0.f
		)
		: Velocity(InVelocity)
		, VelocityNoOffset(InVelocityNoOffset)
		, Acceleration(InAcceleration)
		, AccelerationNoOffset(InAccelerationNoOffset)
		, bHasEverUpdated(false)
		, bHasCachedCardinals(false)
	{
		ConstructDefaultCardinals();
	}
	
	FSimpleCardinals(
		const FSimpleMovement& World2D,
		const FRotator& WorldRotation,
		float RootYawOffset
		)
		: bHasEverUpdated(false)
		, bHasCachedCardinals(false)
	{
		ConstructDefaultCardinals();
		ThreadSafeUpdate_Internal(World2D, WorldRotation, RootYawOffset);
	}

	void ConstructDefaultCardinals();
	static FGameplayTagContainer ConstructCardinalTags_1Way();
	static FGameplayTagContainer ConstructCardinalTags_2Way();
	static FGameplayTagContainer ConstructCardinalTags_4Way();
	static FGameplayTagContainer ConstructCardinalTags_6Way();
	static FGameplayTagContainer ConstructCardinalTags_8Way();
	static FGameplayTagContainer ConstructCardinalTags_10Way();

	void SetCardinalEnabled(const FGameplayTag& CardinalMode, bool bEnabled);

	UPROPERTY(BlueprintReadOnly, Category=Properties)
	float Velocity;

	UPROPERTY(BlueprintReadOnly, Category=Properties)
	float VelocityNoOffset;
	
	UPROPERTY(BlueprintReadOnly, Category=Properties)
	float Acceleration;

	UPROPERTY(BlueprintReadOnly, Category=Properties)
	float AccelerationNoOffset;

	UPROPERTY(BlueprintReadOnly, Category=Properties)
	bool bHasEverUpdated;

	/** Get current cardinal based on Simple.Mode and CardinalType */
	FGameplayTag GetCurrentCardinal(FGameplayTag Mode, ESimpleCardinalType CardinalType) const;

	FGameplayTag GetCurrentCardinal(const struct FSimpleLocomotionSet& LocomotionSet) const;
	
	float GetDirectionAngle(ESimpleCardinalType CardinalType) const;

	void ThreadSafeUpdate(const FSimpleMovement& World2D, const FRotator& WorldRotation, float RootYawOffset);

	static float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation);

	void CacheCardinals();

	const TMap<FGameplayTag, FSimpleCardinal>& GetCardinals(ESimpleCardinalCache Mode = ESimpleCardinalCache::CachedEnabledOnly) const;
	TMap<FGameplayTag, FSimpleCardinal>& GetCardinals(ESimpleCardinalCache Mode = ESimpleCardinalCache::CachedEnabledOnly);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category=Properties)
	bool bHasCachedCardinals;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Properties, meta=(Categories="Simple.Mode"))
	TMap<FGameplayTag, FSimpleCardinal> Cardinals;

	/** Cache only the cardinals that are enabled */
	UPROPERTY(BlueprintReadOnly, Category=Properties, meta=(Categories="Simple.Mode"))
	TMap<FGameplayTag, FSimpleCardinal> CachedCardinals;

	void ThreadSafeUpdate_Internal(const FSimpleMovement& World2D, const FRotator& WorldRotation, float RootYawOffset);
};

/**
 * Container holding each possible cardinal animation
 * Has a property type customization that hides any that are not currently in use based on the assigned Mode
 * Contained by FSimpleGaitSet, allowing separate locomotion set for each gait mode
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleLocomotionSet
{
	GENERATED_BODY()

	FSimpleLocomotionSet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(Categories="Simple.Mode"))
	FGameplayTag Mode;

	/** Moving 0º forward */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	UAnimSequence* Forward;

	/** Strafing -45º to the left */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	UAnimSequence* ForwardLeft;

	/** Strafing 45º to the right */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	UAnimSequence* ForwardRight;
	
	/** Strafing -90º to the left while pelvis faces toward the movement direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	UAnimSequence* Left;
	
	/** Strafing 90º to the right while pelvis faces toward the movement direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	UAnimSequence* Right;
	
	/** Strafing -90º to the left while pelvis faces away from the movement direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	UAnimSequence* LeftAway;
	
	/** Strafing 90º to the right while pelvis faces away from the movement direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	UAnimSequence* RightAway;
	
	/** Strafing 180º backwards */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	UAnimSequence* Backward;
	
	/** Strafing -135º to the left */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	UAnimSequence* BackwardLeft;
	
	/** Strafing 135º to the right */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	UAnimSequence* BackwardRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	ESimpleCardinalType CardinalType;

	UAnimSequence* GetAnimation(const FGameplayTag& Cardinal) const;
};

/**
 * FGameplayTagContainer does not maintain order
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleGameplayTagArray
{
	GENERATED_BODY()

	/**
	 * Add the specified tag to the container
	 *
	 * @param TagToAdd Tag to add to the container
	 */
	void AddTag(const FGameplayTag& TagToAdd)
	{
		if (!Tags.Contains(TagToAdd))
		{
			Tags.Add(TagToAdd);
		}
	}

	/**
	 * Add the specified tag to the container without checking for uniqueness
	 *
	 * @param TagToAdd Tag to add to the container
	 * 
	 * Useful when building container from another data struct (TMap for example)
	 */
	void AddTagFast(const FGameplayTag& TagToAdd)
	{
		Tags.Add(TagToAdd);
	}
	
	/** Gets the explicit list of gameplay tags */
	const TArray<FGameplayTag>& GetGameplayTagArray() const { return Tags; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TArray<FGameplayTag> Tags;
};

/**
 * Holds FSimpleLocomotionSet for each gait that is in use (e.g. Walk, Run, Sprint)
 * Handles fallback when the requested gait is unavailable
 * e.g. if we request the Run gait, but we only have walk, it could fall back to the Walk set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleGaitSet
{
	GENERATED_BODY()

	FSimpleGaitSet();

	/** FSimpleGameplayTags::Simple_Gait_ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(Categories="Simple.Gait"))
	TMap<FGameplayTag, FSimpleLocomotionSet> Sets;

	/** If requested Gait is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(Categories="Simple.Gait"))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;

	void SetCardinalType(ESimpleCardinalType CardinalType);

	FSimpleLocomotionSet* GetLocomotionSet(const FGameplayTag& GaitTag);
	const FSimpleLocomotionSet* GetLocomotionSet(const FGameplayTag& GaitTag) const;
};