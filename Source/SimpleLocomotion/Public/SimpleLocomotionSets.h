// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "SimpleLocomotionSets.generated.h"

enum class ESimpleCardinalType : uint8;

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
	static UAnimSequence* GetAnim(const FGameplayTag& KeyTag, const TMap<FGameplayTag, UAnimSequence*>& Anims, const TMap<FGameplayTag, FSimpleGameplayTagArray>& Fallbacks)
	{
		// Return this anim if available
		if (UAnimSequence* const* Anim = Anims.Find(KeyTag))
		{
			return *Anim;
		}

		// Otherwise fallback to the closest available
		if (const FSimpleGameplayTagArray* Fallback = Fallbacks.Find(KeyTag))
		{
			for (const FGameplayTag& Tag : Fallback->GetGameplayTagArray())
			{
				if (UAnimSequence* const* Anim = Anims.Find(Tag))
				{
					return *Anim;
				}
			}
		}

		// No available anim
		return nullptr;
	}

	template<typename T>
	static const T* GetSet(const FGameplayTag& KeyTag, const TMap<FGameplayTag, T>& Sets, const TMap<FGameplayTag, FSimpleGameplayTagArray>& Fallbacks)
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
struct SIMPLELOCOMOTION_API FSimpleLocomotionSet
{
	GENERATED_BODY()

	FSimpleLocomotionSet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Mode"))
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
	UAnimSequence* Entry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	UAnimSequence* Exit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	UAnimSequence* Loop;
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

	/** Map tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Gait"))
	TMap<FGameplayTag, FSimpleLocomotionSet> Sets;

	/** If requested Gait is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Gait"))
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance"))
	TMap<FGameplayTag, UAnimSequence*> Animations;

	/** If requested Stance is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance"))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds FSimpleLocomotionSet for each gait that is in use (e.g. Walk, Run, Sprint)
 * Handles fallback when the requested gait is unavailable
 * e.g. if we request the Run gait, but we only have walk, it could fall back to the Walk set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStanceToGaitSet
{
	GENERATED_BODY()

	FSimpleStanceToGaitSet();

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance"))
	TMap<FGameplayTag, FSimpleGaitSet> Sets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance"))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds FSimpleGaitSet for each state that is in use (e.g. Idle, Walk, Run)
 * Handles fallback when the requested state is unavailable
 * e.g. if we request the Walk gait, but we only have Idle, it could fall back to the Idle set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStateToGaitSet
{
	GENERATED_BODY()

	FSimpleStateToGaitSet();

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State"))
	TMap<FGameplayTag, FSimpleGaitSet> Sets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State"))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Holds FSimpleGaitSet for each state that is in use (e.g. Idle, Walk, Run)
 * Handles fallback when the requested state is unavailable
 * e.g. if we request the Walk gait, but we only have Idle, it could fall back to the Idle set
 */
USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleStanceToStateToGaitSet
{
	GENERATED_BODY()

	FSimpleStanceToStateToGaitSet();

	/** Maps tags to sets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State"))
	TMap<FGameplayTag, FSimpleStanceToGaitSet> Sets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State"))
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance"))
	TMap<FGameplayTag, FSimpleTransitionSet> Sets;

	/** If requested Stance is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.Stance"))
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State"))
	TMap<FGameplayTag, FSimpleStanceSet> Sets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State"))
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State"))
	TMap<FGameplayTag, FSimpleStanceToTransitionSet> Sets;

	/** If requested State is not available, fallback to the next match. Order represents priority */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation, meta=(GameplayTagFilter="Simple.State"))
	TMap<FGameplayTag, FSimpleGameplayTagArray> Fallbacks;
};

/**
 * Blueprint Getter for Simple Animation Sets
 */
UCLASS()
class SIMPLELOCOMOTION_API USimpleLocomotionSets : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.Cardinal"))
	static UAnimSequence* SimpleLocomotionSet(const FSimpleLocomotionSet& Set, FGameplayTag Cardinal)
	{
		return Set.GetAnimation(Cardinal);
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.Gait,Simple.Cardinal"))
	static UAnimSequence* SimpleGaitSet(const FSimpleGaitSet& Set, FGameplayTag Gait, FGameplayTag Cardinal)
	{
		const FSimpleLocomotionSet* LocoSet = FSimpleGetter::GetSet<FSimpleLocomotionSet>(Gait, Set.Sets, Set.Fallbacks);
		return LocoSet ? LocoSet->GetAnimation(Cardinal) : nullptr;
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.Gait,Simple.Stance,Simple.Cardinal"))
	static UAnimSequence* SimpleStanceToGaitSet(const FSimpleStanceToGaitSet& Set, FGameplayTag Stance, FGameplayTag Gait, FGameplayTag Cardinal)
	{ 
		const FSimpleGaitSet* GaitSet = FSimpleGetter::GetSet<FSimpleGaitSet>(Stance, Set.Sets, Set.Fallbacks);
		const FSimpleLocomotionSet* LocoSet = FSimpleGetter::GetSet<FSimpleLocomotionSet>(Gait, GaitSet->Sets, GaitSet->Fallbacks);
		return LocoSet ? LocoSet->GetAnimation(Cardinal) : nullptr;
	}
	
	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.State,Simple.Gait,Simple.Cardinal"))
	static UAnimSequence* SimpleStateToGaitSet(const FSimpleStateToGaitSet& Set, FGameplayTag State, FGameplayTag Gait, FGameplayTag Cardinal)
	{
		const FSimpleGaitSet* GaitSet = FSimpleGetter::GetSet<FSimpleGaitSet>(State, Set.Sets, Set.Fallbacks);
		const FSimpleLocomotionSet* LocoSet = FSimpleGetter::GetSet<FSimpleLocomotionSet>(Gait, GaitSet->Sets, GaitSet->Fallbacks);
		return LocoSet ? LocoSet->GetAnimation(Cardinal) : nullptr;
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.State,Simple.Stance,Simple.Gait,Simple.Cardinal"))
	static UAnimSequence* SimpleStateToStanceToGaitSet(const FSimpleStanceToStateToGaitSet& Set, FGameplayTag State, FGameplayTag Stance, FGameplayTag Gait, FGameplayTag Cardinal)
	{
		const FSimpleStanceToGaitSet* StanceToGaitSet = FSimpleGetter::GetSet<FSimpleStanceToGaitSet>(State, Set.Sets, Set.Fallbacks);
		const FSimpleGaitSet* GaitSet = FSimpleGetter::GetSet<FSimpleGaitSet>(Stance, StanceToGaitSet->Sets, StanceToGaitSet->Fallbacks);
		const FSimpleLocomotionSet* LocoSet = FSimpleGetter::GetSet<FSimpleLocomotionSet>(Gait, GaitSet->Sets, GaitSet->Fallbacks);
		return LocoSet ? LocoSet->GetAnimation(Cardinal) : nullptr;
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.Stance"))
	static UAnimSequence* SimpleStanceSet(const FSimpleStanceSet& Set, FGameplayTag Stance)
	{
		return FSimpleGetter::GetAnim(Stance, Set.Animations, Set.Fallbacks);
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.Stance"))
	static const FSimpleTransitionSet& SimpleStanceToTransitionSet(const FSimpleStanceToTransitionSet& Set, FGameplayTag Stance)
	{
		const FSimpleTransitionSet* TransitionSet = FSimpleGetter::GetSet<FSimpleTransitionSet>(Stance, Set.Sets, Set.Fallbacks);
		return TransitionSet ? *TransitionSet : Set.DummySet;
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.State,Simple.Stance"))
	static UAnimSequence* SimpleStateToStanceSet(const FSimpleStateToStanceSet& Set, FGameplayTag State, FGameplayTag Stance)
	{
		const FSimpleStanceSet* StanceSet = FSimpleGetter::GetSet<FSimpleStanceSet>(State, Set.Sets, Set.Fallbacks);
		return FSimpleGetter::GetAnim(Stance, StanceSet->Animations, StanceSet->Fallbacks);
	}

	UFUNCTION(BlueprintPure, Category=SimpleLocomotion, meta=(BlueprintThreadSafe, Keywords="Get,Getter", GameplayTagFilter="Simple.State,Simple.Stance"))
	static const FSimpleTransitionSet& SimpleStateToStanceToTransitionSet(const FSimpleStateToStanceToTransitionSet& Set, FGameplayTag State, FGameplayTag Stance)
	{
		if (const FSimpleStanceToTransitionSet* StanceToTransitionSet = FSimpleGetter::GetSet<FSimpleStanceToTransitionSet>(State, Set.Sets, Set.Fallbacks))
		{
			return SimpleStanceToTransitionSet(*StanceToTransitionSet, Stance);
		}
		return Set.DummySet;
	}
};