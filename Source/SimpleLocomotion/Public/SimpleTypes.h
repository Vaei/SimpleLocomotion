// Copyright (c) Jared Taylor

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "SimpleTypes.generated.h"

class UAnimSequence;

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
	Velocity				UMETA(Tooltip="Local Space Velocity"),
};

/**
 * Parameters for a "Blend Poses by Bool" operation.
 * Encapsulates the timings, blend curve, and blend type used to control transitions
 * between a "true" and "false" animation pose.
 *
 * @see USimpleStatics::BlendByBool()
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FBlendByBool
{
	GENERATED_BODY()

	FBlendByBool(float InTrueBlendTime = 0.1f, float InFalseBlendTime = 0.1f,
		EAlphaBlendOption InBlendType = EAlphaBlendOption::HermiteCubic, UCurveFloat* InCustomBlendCurve = nullptr)
		: TrueBlendTime(InTrueBlendTime)
		, FalseBlendTime(InFalseBlendTime)
		, BlendType(InBlendType)
		, CustomBlendCurve(InCustomBlendCurve)
	{}

	/** Blend time (in seconds) when transitioning from false → true. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	float TrueBlendTime = 0.1f;

	/** Blend time (in seconds) when transitioning from true → false. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	float FalseBlendTime = 0.1f;

	/** Blend curve type used for interpolation (e.g., linear, cubic, etc.). */
	UPROPERTY(EditAnywhere, Category=BlendType, meta=(FoldProperty))
	EAlphaBlendOption BlendType = EAlphaBlendOption::HermiteCubic;

	/** Optional custom blend curve. Overrides BlendType if provided. */
	UPROPERTY(EditAnywhere, Category=BlendType, meta=(FoldProperty))
	TObjectPtr<UCurveFloat> CustomBlendCurve = nullptr;
};

/**
 * Runtime state for a single "Blend by Bool" operation.
 * Stores blending progress, weight, and timing information for use across frames.
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FBlendByBoolState
{
	GENERATED_BODY()

	/** Internal alpha blend controller that handles smooth interpolation between weights. */
	UPROPERTY(BlueprintReadOnly, Category=Animation)
	FAlphaBlend Blend;

	/** Whether the bool input was active in the previous frame (used to detect state changes). */
	UPROPERTY(BlueprintReadOnly, Category=Animation)
	bool bWasActive = false;

	/** Current blended weight after the last update (0.0 to 1.0). */
	UPROPERTY(BlueprintReadOnly, Category=Animation)
	float Weight = 0.f;

	/** Remaining time (in seconds) until the current blend completes. */
	UPROPERTY(BlueprintReadOnly, Category=Animation)
	float RemainingTime = 0.f;

	/** Alpha value at the start of the most recent blend transition. */
	UPROPERTY(BlueprintReadOnly, Category=Animation)
	float StartAlpha = 0.f;

	/** Whether this state has been initialized. Prevents using uninitialized blend values. */
	UPROPERTY(BlueprintReadOnly, Category=Animation)
	bool bInitialized = false;

	/**
	 * Initializes the blend state for the first time.
	 * @param bActive - Initial bool value (true/false pose selection).
	 * @param Params  - Blend parameters (times, curves, blend type).
	 */
	void Initialize(bool bActive, const FBlendByBool& Params);

	/**
	 * Updates the blend state based on the current bool value.
	 * @param bActive - Current bool value (true or false pose active).
	 * @param Params - Blend parameters (times, curves, blend type).
	 * @param DeltaTime - Time step in seconds since the last update.
	 * @return The new blended weight after updating (0.0 to 1.0).
	 */
	float Update(bool bActive, const FBlendByBool& Params, float DeltaTime);
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

	/** Used for climbing walls, ladders, etc. */
	UPROPERTY(BlueprintReadOnly, Category=Properties, meta=(GameplayTagFilter="Simple.Cardinal"))
	FGameplayTag AccelerationWall;

	/** Used for climbing walls, ladders, etc. */
	UPROPERTY(BlueprintReadOnly, Category=Properties, meta=(GameplayTagFilter="Simple.Cardinal"))
	FGameplayTag VelocityWall;

	/** Bound in USimpleAnimInstance::NativeInitializeAnimation */
	FSimpleCardinalUpdate UpdateDelegate;

	FGameplayTag GetCardinal(ESimpleCardinalType CardinalType, bool bOnWall = false) const;
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
		float InAcceleration = 0.f,
		float InVelocity3D = 0.f,
		float InAcceleration3D = 0.f
		)
		: Velocity(InVelocity)
		, Acceleration(InAcceleration)
		, VelocityWall(InVelocity3D)
		, AccelerationWall(InAcceleration3D)
		, bHasEverUpdated(false)
		, bHasCachedCardinals(false)
	{
		ConstructDefaultCardinals();
	}
	
	FSimpleCardinals(
		const FSimpleMovement& World2D,
		const FSimpleMovement& World,
		const FRotator& WorldRotation,
		bool bOnWall
		)
		: bHasEverUpdated(false)
		, bHasCachedCardinals(false)
	{
		ConstructDefaultCardinals();
		ThreadSafeUpdate_Internal(World2D, World, WorldRotation, bOnWall);
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
	float VelocityWall;

	UPROPERTY(BlueprintReadOnly, Category=Properties)
	float AccelerationWall;
	
	UPROPERTY(BlueprintReadOnly, Category=Properties)
	bool bHasEverUpdated;

	/**
	 * Get current cardinal based on Simple.Mode and CardinalType, e.g. Simple.Mode.Strafe.1Way and ESimpleCardinalType::Acceleration
	 * @return Simple.Cardinal, e.g. Simple.Cardinal.Forward.Left
	 */
	FGameplayTag GetCurrentCardinal(const FGameplayTag& CardinalModeTag, ESimpleCardinalType CardinalType, bool bOnWall = false) const;

	FGameplayTag GetCurrentCardinal(const struct FSimpleStrafeLocoSet& LocomotionSet, bool bOnWall = false) const;
	FGameplayTag GetCurrentCardinal(const struct FSimpleStrafeLocoSet* LocomotionSet, bool bOnWall = false) const;
	FGameplayTag GetCurrentCardinal(const struct FSimpleStartLocoSet& LocomotionSet, bool bOnWall = false) const;
	FGameplayTag GetCurrentCardinal(const struct FSimpleStartLocoSet* LocomotionSet, bool bOnWall = false) const;
	FGameplayTag GetCurrentCardinal(const struct FSimpleTurnLocoSet& LocomotionSet, bool bOnWall = false) const;
	FGameplayTag GetCurrentCardinal(const struct FSimpleTurnLocoSet* LocomotionSet, bool bOnWall = false) const;
	
	float GetDirectionAngle(ESimpleCardinalType CardinalType, bool bHorizontal = true) const;

	void ThreadSafeUpdate(const FSimpleMovement& World2D, const FSimpleMovement& World, const FRotator& WorldRotation, bool bOnWall = false);

	static float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation);
	static float CalculateDirectionWall(const FVector& Velocity, const FRotator& BaseRotation);

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

	void ThreadSafeUpdate_Internal(const FSimpleMovement& World2D, const FSimpleMovement& World, const FRotator& WorldRotation, bool bOnWall = false);
};

