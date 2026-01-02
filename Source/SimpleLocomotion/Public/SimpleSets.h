// Copyright (c) Jared Taylor

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SimpleTags.h"
#include "SimpleSets.generated.h"

enum class ESimpleCardinalType : uint8;

enum class ESetType : uint8
{
	None,
	AnimState,
};

/**
 * Custom FGameplayTagContainer that maintains order
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

/** Getter for Simple Animation Sets */
struct SIMPLELOCOMOTION_API FSimpleGetter
{
	static UAnimSequence* GetAnim(const FGameplayTag& KeyTag, const TMap<FGameplayTag, TObjectPtr<UAnimSequence>>& Anims,
		const TMap<FGameplayTag, FSimpleGameplayTagArray>& Fallbacks, ESetType SetType = ESetType::None)
	{
		// Return this anim if available
		if (const TObjectPtr<UAnimSequence>* Anim = Anims.Find(KeyTag))
		{
			return *Anim;
		}

		// Otherwise fallback to the closest available
		if (const FSimpleGameplayTagArray* Fallback = Fallbacks.Find(KeyTag))
		{
			for (const FGameplayTag& FallbackTag : Fallback->GetGameplayTagArray())
			{
				if (const TObjectPtr<UAnimSequence>* Anim = Anims.Find(FallbackTag))
				{
					return *Anim;
				}
			}
		}

		// If this is an AnimState and no fallbacks are provided, return the default state
		if (SetType == ESetType::AnimState)
		{
			const TObjectPtr<UAnimSequence>* Anim = Anims.Find(FSimpleTags::Simple_State_Default);
			if (ensure(Anim))  // Did we accidentally mark as AnimState?
			{
				return *Anim;
			}
		}

		// No available anim
		return nullptr;
	}
	
	static UBlendSpace* GetBlendSpace(const FGameplayTag& KeyTag, const TMap<FGameplayTag, TObjectPtr<UBlendSpace>>& BlendSpaces,
		const TMap<FGameplayTag, FSimpleGameplayTagArray>& Fallbacks, ESetType SetType = ESetType::None)
	{
		// Return this blend space if available
		if (const TObjectPtr<UBlendSpace>* BlendSpace = BlendSpaces.Find(KeyTag))
		{
			return *BlendSpace;
		}

		// Otherwise fallback to the closest available
		if (const FSimpleGameplayTagArray* Fallback = Fallbacks.Find(KeyTag))
		{
			for (const FGameplayTag& FallbackTag : Fallback->GetGameplayTagArray())
			{
				if (const TObjectPtr<UBlendSpace>* BlendSpace = BlendSpaces.Find(FallbackTag))
				{
					return *BlendSpace;
				}
			}
		}

		// If this is an AnimState and no fallbacks are provided, return the default state
		if (SetType == ESetType::AnimState)
		{
			const TObjectPtr<UBlendSpace>* BlendSpace = BlendSpaces.Find(FSimpleTags::Simple_State_Default);
			if (ensure(BlendSpace))  // Did we accidentally mark as AnimState?
			{
				return *BlendSpace;
			}
		}

		// No available blend space
		return nullptr;
	}

	/**
	 * Get the set of animations for the specified key tag
	 * 
	 * @param KeyTag The tag to look up
	 * @param Sets The map of sets to search
	 * @param Fallbacks The map of fallback tags to search if the key tag is not found
	 * @param SetType For AnimStates use an automatic fallback to default state if no fallbacks are provided
	 * @return Pointer to the set of animations, or nullptr if not found
	 */
	template<typename T>
	static const T* GetSet(const FGameplayTag& KeyTag, const TMap<FGameplayTag, T>& Sets, const TMap<FGameplayTag,
		FSimpleGameplayTagArray>& Fallbacks, ESetType SetType = ESetType::None)
	{
		// Return this set if available
		if (const T* Set = Sets.Find(KeyTag))
		{
			return Set;
		}

		// Otherwise fallback to the closest available
		if (const FSimpleGameplayTagArray* Fallback = Fallbacks.Find(KeyTag))
		{
			for (const FGameplayTag& FallbackTag : Fallback->GetGameplayTagArray())
			{
				if (const T* Set = Sets.Find(FallbackTag))
				{
					return Set;
				}
			}
		}

		// If this is an AnimState and no fallbacks are provided, return the default state
		if (SetType == ESetType::AnimState)
		{
			const T* DefaultSet = Sets.Find(FSimpleTags::Simple_State_Default);
			if (ensure(DefaultSet))  // Did we accidentally mark as AnimState?
			{
				return DefaultSet;
			}
		}

		// No available set
		return nullptr;
	}
};

/**
 * Container holding each possible cardinal animation
 * Has a property type customization that hides any that are not currently in use based on the assigned Mode
 * Contained by FSimpleGaitSet, allowing separate locomotion set for each gait mode
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStrafeLocoSet
{
	GENERATED_BODY()

	FSimpleStrafeLocoSet();

#if WITH_EDITORONLY_DATA
	// Details customization
	
	UPROPERTY()
	bool bDisableAway = false;

	UPROPERTY()
	bool bDisableInertialBlending = false;

	UPROPERTY()
	bool bDisableCardinalType = false;
	
	// ~Details customization
#endif
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Mode.Strafe"))
	FGameplayTag Mode;
	
	/** Blend time to optionally pass to SetSequenceWithInertialBlending */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	float InertialBlendTime;

	/** Moving 0º forward */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> Forward;

	/** Strafing -45º to the left */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> ForwardLeft;

	/** Strafing 45º to the right */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> ForwardRight;
	
	/** Strafing -90º to the left while pelvis faces toward the movement direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> Left;
	
	/** Strafing 90º to the right while pelvis faces toward the movement direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> Right;
	
	/** Strafing -90º to the left while pelvis faces away from the movement direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> LeftAway;
	
	/** Strafing 90º to the right while pelvis faces away from the movement direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> RightAway;
	
	/** Strafing 180º backwards */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> Backward;
	
	/** Strafing -135º to the left */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> BackwardLeft;
	
	/** Strafing 135º to the right */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> BackwardRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	ESimpleCardinalType CardinalType;

	bool operator==(const FSimpleStrafeLocoSet& Other) const
	{
		return Mode == Other.Mode &&
			CardinalType == Other.CardinalType &&
			InertialBlendTime == Other.InertialBlendTime &&
			Forward == Other.Forward &&
			ForwardLeft == Other.ForwardLeft &&
			ForwardRight == Other.ForwardRight &&
			Left == Other.Left &&
			Right == Other.Right &&
			LeftAway == Other.LeftAway &&
			RightAway == Other.RightAway &&
			Backward == Other.Backward &&
			BackwardLeft == Other.BackwardLeft &&
			BackwardRight == Other.BackwardRight;
	}

	bool operator!=(const FSimpleStrafeLocoSet& Other) const
	{
		return !(*this == Other);
	}
	
	bool IsValid() const
	{
		return *this != FSimpleStrafeLocoSet();
	}

	UAnimSequence* GetAnimation(const FGameplayTag& CardinalTag) const;
};

/**
 * Container holding each possible cardinal animation
 * Has a property type customization that hides any that are not currently in use based on the assigned Mode
 * Contained by FSimpleGaitSet, allowing separate locomotion set for each gait mode
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStrafeMontageSet
{
	GENERATED_BODY()

	FSimpleStrafeMontageSet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Mode.Strafe.1Way,Simple.Mode.Strafe.2Way,Simple.Mode.Strafe.4Way,Simple.Mode.Strafe.8Way"))
	FGameplayTag Mode;

	/** Moving 0º forward */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimMontage> Forward;

	/** Strafing -45º to the left */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimMontage> ForwardLeft;

	/** Strafing 45º to the right */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimMontage> ForwardRight;
	
	/** Strafing -90º to the left while pelvis faces toward the movement direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimMontage> Left;
	
	/** Strafing 90º to the right while pelvis faces toward the movement direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimMontage> Right;

	/** Strafing 180º backwards */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimMontage> Backward;
	
	/** Strafing -135º to the left */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimMontage> BackwardLeft;
	
	/** Strafing 135º to the right */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimMontage> BackwardRight;

	bool operator==(const FSimpleStrafeMontageSet& Other) const
	{
		return Mode == Other.Mode &&
			Forward == Other.Forward &&
			ForwardLeft == Other.ForwardLeft &&
			ForwardRight == Other.ForwardRight &&
			Left == Other.Left &&
			Right == Other.Right &&
			Backward == Other.Backward &&
			BackwardLeft == Other.BackwardLeft &&
			BackwardRight == Other.BackwardRight;
	}

	bool operator!=(const FSimpleStrafeMontageSet& Other) const
	{
		return !(*this == Other);
	}
	
	bool IsValid() const
	{
		return *this != FSimpleStrafeMontageSet();
	}

	UAnimMontage* GetMontage(const FGameplayTag& CardinalTag) const;
};

/**
 * Start moving locomotion set
 * For forward-facing movement only
 * Strafe uses the FSimpleLocoSet even for starts
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStartLocoSet
{
	GENERATED_BODY()

	FSimpleStartLocoSet();

#if WITH_EDITORONLY_DATA
	// Details customization

	UPROPERTY()
	bool bDisableCardinalType = false;
	
	// ~Details customization
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Mode.Start"))
	FGameplayTag Mode;

	/** Turning 0º forward - moving straight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> Forward;
	
	/** Turning -45º to the left */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> ForwardLeft;

	/** Turning 45º to the right */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> ForwardRight;
	
	/** Turning -90º to the left while pelvis faces toward the movement direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> Left;
	
	/** Turning 90º to the right while pelvis faces toward the movement direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> Right;
	
	/** Turning 180º backwards to the left */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> BackwardTurnLeft;

	/** Turning 180º backwards to the right */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> BackwardTurnRight;
	
	/** Turning -135º to the left */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> BackwardLeft;
	
	/** Turning 135º to the right */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> BackwardRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	ESimpleCardinalType CardinalType;

	bool operator==(const FSimpleStartLocoSet& Other) const
	{
		return Mode == Other.Mode &&
			CardinalType == Other.CardinalType &&
			Forward == Other.Forward &&
			ForwardLeft == Other.ForwardLeft &&
			ForwardRight == Other.ForwardRight &&
			Left == Other.Left &&
			Right == Other.Right &&
			BackwardLeft == Other.BackwardLeft &&
			BackwardRight == Other.BackwardRight &&
			BackwardTurnLeft == Other.BackwardTurnLeft &&
			BackwardTurnRight == Other.BackwardTurnRight;
	}

	bool operator!=(const FSimpleStartLocoSet& Other) const
	{
		return !(*this == Other);
	}

	bool IsValid() const
	{
		return *this != FSimpleStartLocoSet();
	}

	UAnimSequence* GetAnimation(const FGameplayTag& CardinalTag) const;
};

/**
 * Turning locomotion set, for turn pivots
 * For forward-facing movement only
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleTurnLocoSet
{
	GENERATED_BODY()

	FSimpleTurnLocoSet();

#if WITH_EDITORONLY_DATA
	// Details customization

	UPROPERTY()
	bool bDisableCardinalType = false;
	
	// ~Details customization
#endif
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Mode.Turn"))
	FGameplayTag Mode;

	/** e.g. 4-Way turns with a 90-degree angle, but you may want to trigger a turn with 60 to make it more sensitive, in fact 90 may never be sensitive enough to trigger */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(ClampMin="0.0", ClampMax="180", UIMin="0.0", UIMax="180.0", Delta="1", ForceUnits="degrees"))
	float AngleTolerance;

	/** Turning -45º to the left */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> ForwardLeft;

	/** Turning 45º to the right */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> ForwardRight;
	
	/** Turning -90º to the left while pelvis faces toward the movement direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> Left;
	
	/** Turning 90º to the right while pelvis faces toward the movement direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> Right;
	
	/** Turning 180º backwards to the left */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> BackwardTurnLeft;

	/** Turning 180º backwards to the right */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> BackwardTurnRight;
	
	/** Turning -135º to the left */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> BackwardLeft;
	
	/** Turning 135º to the right */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> BackwardRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	ESimpleCardinalType CardinalType;

	bool operator==(const FSimpleTurnLocoSet& Other) const
	{
		return Mode == Other.Mode &&
			CardinalType == Other.CardinalType &&
			ForwardLeft == Other.ForwardLeft &&
			ForwardRight == Other.ForwardRight &&
			Left == Other.Left &&
			Right == Other.Right &&
			BackwardLeft == Other.BackwardLeft &&
			BackwardRight == Other.BackwardRight &&
			BackwardTurnLeft == Other.BackwardTurnLeft &&
			BackwardTurnRight == Other.BackwardTurnRight;
	}

	bool operator!=(const FSimpleTurnLocoSet& Other) const
	{
		return !(*this == Other);
	}

	bool IsValid() const
	{
		return *this != FSimpleTurnLocoSet();
	}

	UAnimSequence* GetAnimation(const FGameplayTag& CardinalTag) const;
};

/** Container for transition animations */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleTransitionSet
{
	GENERATED_BODY()

	FSimpleTransitionSet()
		: Entry(nullptr)
		, Exit(nullptr)
		, Loop(nullptr)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> Entry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> Exit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<UAnimSequence> Loop;
};

/**
 * Holds FSimpleLocoSet for each gait that is in use (e.g. Walk, Run, Sprint)
 * Handles fallback when the requested gait is unavailable
 * e.g. if we request the Run gait, but we only have walk, it could fall back to the Walk set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStrafeGaitSet
{
	GENERATED_BODY()

	FSimpleStrafeGaitSet();

	/** Map tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Gait", ForceInlineRow))
	TMap<FGameplayTag, FSimpleStrafeLocoSet> GaitSets;

	/** If requested Gait is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Gait", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;

	void SetCardinalType(ESimpleCardinalType CardinalType);
};

/**
 * Holds FSimpleLocoSet for each gait that is in use (e.g. Walk, Run, Sprint)
 * Handles fallback when the requested gait is unavailable
 * e.g. if we request the Run gait, but we only have walk, it could fall back to the Walk set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStartGaitSet
{
	GENERATED_BODY()

	FSimpleStartGaitSet();

	/** Map tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Gait", ForceInlineRow))
	TMap<FGameplayTag, FSimpleStartLocoSet> GaitSets;

	/** If requested Gait is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Gait", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;

	void SetCardinalType(ESimpleCardinalType CardinalType);
};

/**
 * Holds FSimpleTurnLocoSet for each gait that is in use (e.g. Walk, Run, Sprint)
 * Handles fallback when the requested gait is unavailable
 * e.g. if we request the Run gait, but we only have walk, it could fall back to the Walk set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleTurnGaitSet
{
	GENERATED_BODY()

	FSimpleTurnGaitSet();

	/** Map tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Gait", ForceInlineRow))
	TMap<FGameplayTag, FSimpleTurnLocoSet> GaitSets;

	/** If requested Gait is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Gait", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;

	void SetCardinalType(ESimpleCardinalType CardinalType);
};

/**
 * Holds UAnimSequence for each stance that is in use (e.g. Stand, Crouch, Prone)
 * Handles fallback when the requested stance is unavailable
 * e.g. if we request the Crouch stance, but we only have Stand, it could fall back to the Stand set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStanceSet
{
	GENERATED_BODY()

	FSimpleStanceSet();

	/** Map tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance", ForceInlineRow))
	TMap<FGameplayTag, TObjectPtr<UAnimSequence>> Animations;

	/** If requested Stance is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds FSimpleLocoSet for each gait that is in use (e.g. Walk, Run, Sprint)
 * Handles fallback when the requested gait is unavailable
 * e.g. if we request the Run gait, but we only have walk, it could fall back to the Walk set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStanceToStrafeGaitSet
{
	GENERATED_BODY()

	FSimpleStanceToStrafeGaitSet();

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance", ForceInlineRow))
	TMap<FGameplayTag, FSimpleStrafeGaitSet> StanceSets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds FSimpleLocoSet for each gait that is in use (e.g. Walk, Run, Sprint)
 * Handles fallback when the requested gait is unavailable
 * e.g. if we request the Run gait, but we only have walk, it could fall back to the Walk set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStanceToStartGaitSet
{
	GENERATED_BODY()

	FSimpleStanceToStartGaitSet();

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance", ForceInlineRow))
	TMap<FGameplayTag, FSimpleStartGaitSet> StanceSets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds FSimpleTurnLocoSet for each gait that is in use (e.g. Walk, Run, Sprint)
 * Handles fallback when the requested gait is unavailable
 * e.g. if we request the Run gait, but we only have walk, it could fall back to the Walk set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStanceToTurnGaitSet
{
	GENERATED_BODY()

	FSimpleStanceToTurnGaitSet();

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance", ForceInlineRow))
	TMap<FGameplayTag, FSimpleTurnGaitSet> StanceSets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds FSimpleGaitSet for each state that is in use (e.g. Idle, Walk, Run)
 * Handles fallback when the requested state is unavailable
 * e.g. if we request the Walk gait, but we only have Idle, it could fall back to the Idle set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStateToStrafeGaitSet
{
	GENERATED_BODY()

	FSimpleStateToStrafeGaitSet();

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleStrafeGaitSet> StateSets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds FSimpleGaitSet for each state that is in use (e.g. Idle, Walk, Run)
 * Handles fallback when the requested state is unavailable
 * e.g. if we request the Walk gait, but we only have Idle, it could fall back to the Idle set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStateToStartGaitSet
{
	GENERATED_BODY()

	FSimpleStateToStartGaitSet();

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleStartGaitSet> StateSets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds FSimpleGaitSet for each state that is in use (e.g. Idle, Walk, Run)
 * Handles fallback when the requested state is unavailable
 * e.g. if we request the Walk gait, but we only have Idle, it could fall back to the Idle set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStateToTurnGaitSet
{
	GENERATED_BODY()

	FSimpleStateToTurnGaitSet();

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleTurnGaitSet> StateSets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds FSimpleGaitSet for each state that is in use (e.g. Idle, Walk, Run)
 * Handles fallback when the requested state is unavailable
 * e.g. if we request the Walk gait, but we only have Idle, it could fall back to the Idle set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStateToStanceToStrafeGaitSet
{
	GENERATED_BODY()

	FSimpleStateToStanceToStrafeGaitSet();
	
	UPROPERTY()  // Allows the use of reference in getter to avoid copying structs because blueprint cannot use ptr
	FSimpleStrafeLocoSet DummySet;

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleStanceToStrafeGaitSet> StateSets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds FSimpleGaitSet for each state that is in use (e.g. Idle, Walk, Run)
 * Handles fallback when the requested state is unavailable
 * e.g. if we request the Walk gait, but we only have Idle, it could fall back to the Idle set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStateToStanceToStartGaitSet
{
	GENERATED_BODY()

	FSimpleStateToStanceToStartGaitSet();
	
	UPROPERTY()  // Allows the use of reference in getter to avoid copying structs because blueprint cannot use ptr
	FSimpleStartLocoSet DummySet;

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleStanceToStartGaitSet> StateSets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds FSimpleGaitSet for each state that is in use (e.g. Idle, Walk, Run)
 * Handles fallback when the requested state is unavailable
 * e.g. if we request the Walk gait, but we only have Idle, it could fall back to the Idle set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStateToStanceToTurnGaitSet
{
	GENERATED_BODY()

	FSimpleStateToStanceToTurnGaitSet();
	
	UPROPERTY()  // Allows the use of reference in getter to avoid copying structs because blueprint cannot use ptr
	FSimpleTurnLocoSet DummySet;

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleStanceToTurnGaitSet> StateSets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds FSimpleTransitionSet for each stance that is in use (e.g. Stand, Crouch, Prone)
 * Handles fallback when the requested stance is unavailable
 * e.g. if we request the Crouch stance, but we only have Stand, it could fall back to the Stand set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStanceToTransitionSet
{
	GENERATED_BODY()

	FSimpleStanceToTransitionSet();

	UPROPERTY()  // Allows the use of reference in getter to avoid copying structs because blueprint cannot use ptr
	FSimpleTransitionSet DummySet;
	
	/** Map tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance", ForceInlineRow))
	TMap<FGameplayTag, FSimpleTransitionSet> StanceSets;

	/** If requested Stance is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds FSimpleStanceSet for each stance that is in use (e.g. Stand, Crouch, Prone)
 * Handles fallback when the requested stance is unavailable
 * e.g. if we request the Crouch stance, but we only have Stand, it could fall back to the Stand set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStateToStanceSet
{
	GENERATED_BODY()

	FSimpleStateToStanceSet();

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleStanceSet> StateSets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds UAnimSequence for each animation state
 * Handles fallback when the requested state is unavailable
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStateToAnimSet
{
	GENERATED_BODY()

	FSimpleStateToAnimSet();

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, TObjectPtr<UAnimSequence>> Animations;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds UBlendSpace for each animation state
 * Handles fallback when the requested state is unavailable
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStateToBlendSpaceSet
{
	GENERATED_BODY()

	FSimpleStateToBlendSpaceSet();

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, TObjectPtr<UBlendSpace>> BlendSpaces;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds FSimpleStanceTransitionSet for each stance that is in use (e.g. Stand, Crouch, Prone)
 * Handles fallback when the requested stance is unavailable
 * e.g. if we request the Crouch stance, but we only have Stand, it could fall back to the Stand set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStateToStanceToTransitionSet
{
	GENERATED_BODY()

	FSimpleStateToStanceToTransitionSet();

	UPROPERTY()  // Allows the use of reference in getter to avoid copying structs because blueprint cannot use ptr
	FSimpleTransitionSet DummySet;

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleStanceToTransitionSet> StateSets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State", ForceInlineRow))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Blueprint Getter for Simple Animation Sets
 */
UCLASS()
class SIMPLELOCOMOTION_API USimpleSets : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.Cardinal"))
	static UAnimSequence* SimpleStrafeLocoSet(const FSimpleStrafeLocoSet& Set, FGameplayTag Cardinal)
	{
		return Set.GetAnimation(Cardinal);
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.Cardinal"))
	static UAnimSequence* SimpleStartLocoSet(const FSimpleStartLocoSet& Set, FGameplayTag Cardinal)
	{
		return Set.GetAnimation(Cardinal);
	}
	
	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.Cardinal"))
	static UAnimSequence* SimpleTurnLocoSet(const FSimpleTurnLocoSet& Set, FGameplayTag Cardinal)
	{
		return Set.GetAnimation(Cardinal);
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.Gait,Simple.Cardinal"))
	static UAnimSequence* SimpleStrafeGaitSet(const FSimpleStrafeGaitSet& Set, FGameplayTag Gait, FGameplayTag Cardinal)
	{
		const auto* LocoSet = FSimpleGetter::GetSet<FSimpleStrafeLocoSet>(Gait, Set.GaitSets, Set.Fallbacks);
		return LocoSet ? LocoSet->GetAnimation(Cardinal) : nullptr;
	}
	
	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.Gait,Simple.Cardinal"))
	static UAnimSequence* SimpleStartGaitSet(const FSimpleStartGaitSet& Set, FGameplayTag Gait, FGameplayTag Cardinal)
	{
		const auto* LocoSet = FSimpleGetter::GetSet<FSimpleStartLocoSet>(Gait, Set.GaitSets, Set.Fallbacks);
		return LocoSet ? LocoSet->GetAnimation(Cardinal) : nullptr;
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.Gait,Simple.Stance,Simple.Cardinal"))
	static UAnimSequence* SimpleStanceToStrafeGaitSet(const FSimpleStanceToStrafeGaitSet& Set, FGameplayTag Stance, FGameplayTag Gait, FGameplayTag Cardinal)
	{ 
		if (const auto* GaitSet = FSimpleGetter::GetSet<FSimpleStrafeGaitSet>(Stance, Set.StanceSets, Set.Fallbacks))
		{
			const auto* LocoSet = FSimpleGetter::GetSet<FSimpleStrafeLocoSet>(Gait, GaitSet->GaitSets, GaitSet->Fallbacks);
			return LocoSet ? LocoSet->GetAnimation(Cardinal) : nullptr;
		}
		return nullptr;
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.Gait,Simple.Stance,Simple.Cardinal"))
	static UAnimSequence* SimpleStanceToStartGaitSet(const FSimpleStanceToStartGaitSet& Set, FGameplayTag Stance, FGameplayTag Gait, FGameplayTag Cardinal)
	{ 
		if (const auto* GaitSet = FSimpleGetter::GetSet<FSimpleStartGaitSet>(Stance, Set.StanceSets, Set.Fallbacks))
		{
			const auto* LocoSet = FSimpleGetter::GetSet<FSimpleStartLocoSet>(Gait, GaitSet->GaitSets, GaitSet->Fallbacks);
			return LocoSet ? LocoSet->GetAnimation(Cardinal) : nullptr;
		}
		return nullptr;
	}
	
	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.State,Simple.Gait,Simple.Cardinal"))
	static UAnimSequence* SimpleStateToStrafeGaitSet(const FSimpleStateToStrafeGaitSet& Set, FGameplayTag State, FGameplayTag Gait, FGameplayTag Cardinal)
	{
		if (const auto* GaitSet = FSimpleGetter::GetSet<FSimpleStrafeGaitSet>(State, Set.StateSets, Set.Fallbacks, ESetType::AnimState))
		{
			const auto* LocoSet = FSimpleGetter::GetSet<FSimpleStrafeLocoSet>(Gait, GaitSet->GaitSets, GaitSet->Fallbacks);
			return LocoSet ? LocoSet->GetAnimation(Cardinal) : nullptr;
		}
		return nullptr;
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.State,Simple.Gait,Simple.Cardinal"))
	static UAnimSequence* SimpleStateToStartGaitSet(const FSimpleStateToStartGaitSet& Set, FGameplayTag State, FGameplayTag Gait, FGameplayTag Cardinal)
	{
		if (const auto* GaitSet = FSimpleGetter::GetSet<FSimpleStartGaitSet>(State, Set.StateSets, Set.Fallbacks, ESetType::AnimState))
		{
			const auto* LocoSet = FSimpleGetter::GetSet<FSimpleStartLocoSet>(Gait, GaitSet->GaitSets, GaitSet->Fallbacks);
			return LocoSet ? LocoSet->GetAnimation(Cardinal) : nullptr;
		}
		return nullptr;
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.State,Simple.Stance,Simple.Gait"))
	static const FSimpleStrafeLocoSet& SimpleStateToStanceToStrafeGaitSet(const FSimpleStateToStanceToStrafeGaitSet& Set, FGameplayTag State, FGameplayTag Stance, FGameplayTag Gait)
	{
		if (const auto* StanceSet = FSimpleGetter::GetSet<FSimpleStanceToStrafeGaitSet>(State, Set.StateSets, Set.Fallbacks, ESetType::AnimState))
		{
			if (const auto* GaitSet = FSimpleGetter::GetSet<FSimpleStrafeGaitSet>(Stance, StanceSet->StanceSets, StanceSet->Fallbacks))
			{
				if (const auto* LocoSet = FSimpleGetter::GetSet<FSimpleStrafeLocoSet>(Gait, GaitSet->GaitSets, GaitSet->Fallbacks))
				{
					return *LocoSet;
				}
			}
		}
		return Set.DummySet;
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.State,Simple.Stance,Simple.Gait"))
	static const FSimpleStartLocoSet& SimpleStateToStanceToStartGaitSet(const FSimpleStateToStanceToStartGaitSet& Set, FGameplayTag State, FGameplayTag Stance, FGameplayTag Gait)
	{
		if (const auto* StanceSet = FSimpleGetter::GetSet<FSimpleStanceToStartGaitSet>(State, Set.StateSets, Set.Fallbacks, ESetType::AnimState))
		{
			if (const auto* GaitSet = FSimpleGetter::GetSet<FSimpleStartGaitSet>(Stance, StanceSet->StanceSets, StanceSet->Fallbacks))
			{
				if (const auto* LocoSet = FSimpleGetter::GetSet<FSimpleStartLocoSet>(Gait, GaitSet->GaitSets, GaitSet->Fallbacks))
				{
					return *LocoSet;
				}
			}
		}
		return Set.DummySet;
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.State,Simple.Stance,Simple.Gait"))
	static const FSimpleTurnLocoSet& SimpleStateToStanceToTurnGaitSet(const FSimpleStateToStanceToTurnGaitSet& Set, FGameplayTag State, FGameplayTag Stance, FGameplayTag Gait)
	{
		if (const auto* StanceSet = FSimpleGetter::GetSet<FSimpleStanceToTurnGaitSet>(State, Set.StateSets, Set.Fallbacks, ESetType::AnimState))
		{
			if (const auto* GaitSet = FSimpleGetter::GetSet<FSimpleTurnGaitSet>(Stance, StanceSet->StanceSets, StanceSet->Fallbacks))
			{
				if (const auto* LocoSet = FSimpleGetter::GetSet<FSimpleTurnLocoSet>(Gait, GaitSet->GaitSets, GaitSet->Fallbacks))
				{
					return *LocoSet;
				}
			}
		}
		return Set.DummySet;
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.Stance"))
	static UAnimSequence* SimpleStanceSet(const FSimpleStanceSet& Set, FGameplayTag Stance)
	{
		return FSimpleGetter::GetAnim(Stance, Set.Animations, Set.Fallbacks);
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.Stance"))
	static const FSimpleTransitionSet& SimpleStanceToTransitionSet(const FSimpleStanceToTransitionSet& Set, FGameplayTag Stance)
	{
		const auto* TransitionSet = FSimpleGetter::GetSet<FSimpleTransitionSet>(Stance, Set.StanceSets, Set.Fallbacks);
		return TransitionSet ? *TransitionSet : Set.DummySet;
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.State,Simple.Stance"))
	static UAnimSequence* SimpleStateToStanceSet(const FSimpleStateToStanceSet& Set, FGameplayTag State, FGameplayTag Stance)
	{
		const auto* StanceSet = FSimpleGetter::GetSet<FSimpleStanceSet>(State, Set.StateSets, Set.Fallbacks, ESetType::AnimState);
		return FSimpleGetter::GetAnim(Stance, StanceSet->Animations, StanceSet->Fallbacks);
	}
	
	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.State"))
	static UAnimSequence* SimpleStateToAnimSet(const FSimpleStateToAnimSet& Set, FGameplayTag State)
	{
		return FSimpleGetter::GetAnim(State, Set.Animations, Set.Fallbacks, ESetType::AnimState);
	}
	
	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.State"))
	static UBlendSpace* SimpleStateToBlendSpaceSet(const FSimpleStateToBlendSpaceSet& Set, FGameplayTag State)
	{
		return FSimpleGetter::GetBlendSpace(State, Set.BlendSpaces, Set.Fallbacks, ESetType::AnimState);
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.State,Simple.Stance"))
	static const FSimpleTransitionSet& SimpleStateToStanceToTransitionSet(const FSimpleStateToStanceToTransitionSet& Set, FGameplayTag State, FGameplayTag Stance)
	{
		if (const auto* StanceToTransitionSet = FSimpleGetter::GetSet<FSimpleStanceToTransitionSet>(State, Set.StateSets, Set.Fallbacks, ESetType::AnimState))
		{
			return SimpleStanceToTransitionSet(*StanceToTransitionSet, Stance);
		}
		return Set.DummySet;
	}
};