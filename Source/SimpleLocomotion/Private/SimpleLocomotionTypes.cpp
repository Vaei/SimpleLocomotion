// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleLocomotionTypes.h"

#include "SimpleGameplayTags.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(SimpleLocomotionTypes)

DEFINE_LOG_CATEGORY_STATIC(LogSimpleTypes, Log, All);

float FSimpleGaitSpeed::GetMaxSpeed(const FGameplayTag& GaitTag)
{
	if (float* MaxSpeed = MaxSpeeds.Find(GaitTag))
	{
		return *MaxSpeed;
	}
	else if (MaxSpeeds.Num() == 0)
	{
		// Not initialized
		return 0.f;
	}
	ensureMsgf(false, TEXT("[ %s ] Requested Gait { %s } does not exist. Ensure this gait mode is handled by USimpleAnimComponent::GetSimpleMaxGaitSpeeds()"), *FString(__FUNCTION__), *GaitTag.ToString());
	return 0.f;
}

FGameplayTag FSimpleCardinal::GetCardinal(ESimpleCardinalType CardinalType) const
{
	switch (CardinalType)
	{
		case ESimpleCardinalType::Acceleration: return Acceleration;
		case ESimpleCardinalType::AccelerationNoOffset: return AccelerationNoOffset;
		case ESimpleCardinalType::Velocity: return Velocity;
		case ESimpleCardinalType::VelocityNoOffset: return VelocityNoOffset;
	}
	return FGameplayTag::EmptyTag;
}

void FSimpleCardinals::ConstructDefaultCardinals()
{
	// 1-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleGameplayTags::Simple_Cardinal_Mode_1Way);
		Cardinal.Tags = ConstructCardinalTags_1Way();
		Cardinal.bEnabled = true;
	}
	// 2-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleGameplayTags::Simple_Cardinal_Mode_2Way);
		Cardinal.Tags = ConstructCardinalTags_2Way();
		Cardinal.bEnabled = false;
	}
	// 4-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleGameplayTags::Simple_Cardinal_Mode_4Way);
		Cardinal.Tags = ConstructCardinalTags_4Way();
		Cardinal.bEnabled = false;
	}
	// 6-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleGameplayTags::Simple_Cardinal_Mode_6Way);
		Cardinal.Tags = ConstructCardinalTags_6Way();
		Cardinal.bEnabled = false;
	}
	// 8-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleGameplayTags::Simple_Cardinal_Mode_8Way);
		Cardinal.Tags = ConstructCardinalTags_8Way();
		Cardinal.bEnabled = false;
	}
	// 10-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleGameplayTags::Simple_Cardinal_Mode_10Way);
		Cardinal.Tags = ConstructCardinalTags_10Way();
		Cardinal.bEnabled = false;
	}
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_1Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Forward);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_2Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Left);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Right);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_4Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Forward);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Left);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Right);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Backward);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_6Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Forward);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Left);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Right);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Left_Away);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Right_Away);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Backward);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_8Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Forward);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Forward_Left);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Forward_Right);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Left);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Right);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Backward);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Backward_Left);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Backward_Right);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_10Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Forward);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Forward_Left);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Forward_Right);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Left);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Right);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Left_Away);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Right_Away);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Backward);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Backward_Left);
	Tags.AddTagFast(FSimpleGameplayTags::Simple_Cardinal_Backward_Right);
	return Tags;
}

void FSimpleCardinals::SetCardinalEnabled(const FGameplayTag& CardinalModeTag, bool bEnabled)
{
	if (FSimpleCardinal* Mode = Cardinals.Find(CardinalModeTag))
	{
		Mode->bEnabled = bEnabled;
	}
}

FGameplayTag FSimpleCardinals::GetCurrentCardinal(const FGameplayTag& CardinalModeTag, ESimpleCardinalType CardinalType) const
{
	if (CardinalModeTag == FGameplayTag::EmptyTag)
	{
		return FGameplayTag::EmptyTag;
	}
	if (const FSimpleCardinal* MatchingCardinal = GetCardinals().Find(CardinalModeTag))
	{
		if (LIKELY(ensure(MatchingCardinal->bEnabled)))  // Probably shouldn't have been cached if it can be false
		{
			return MatchingCardinal->GetCardinal(CardinalType);
		}
	}
	else
	{
		UE_LOG(LogSimpleTypes, Warning, TEXT("[ %s ] wants Cardinal { %s } but it has not been enabled"), *FString(__FUNCTION__), *CardinalModeTag.ToString());
	}
	return FGameplayTag::EmptyTag;
}

FGameplayTag FSimpleCardinals::GetCurrentCardinal(const FSimpleLocomotionSet& LocomotionSet) const
{
	return GetCurrentCardinal(LocomotionSet.Mode, LocomotionSet.CardinalType);
}

FGameplayTag FSimpleCardinals::GetCurrentCardinal(const FSimpleLocomotionSet* LocomotionSet) const
{
	if (LocomotionSet)
	{
		return GetCurrentCardinal(LocomotionSet->Mode, LocomotionSet->CardinalType);
	}
	return FGameplayTag::EmptyTag;
}

float FSimpleCardinals::GetDirectionAngle(ESimpleCardinalType CardinalType) const
{
	switch (CardinalType)
	{
		case ESimpleCardinalType::Acceleration: return Acceleration;
		case ESimpleCardinalType::AccelerationNoOffset: return AccelerationNoOffset;
		case ESimpleCardinalType::Velocity: return Velocity;
		case ESimpleCardinalType::VelocityNoOffset: return VelocityNoOffset;
	}
	return 0.f;
}

void FSimpleCardinals::ThreadSafeUpdate(const FSimpleMovement& World2D, const FRotator& WorldRotation, float RootYawOffset)
{
	ThreadSafeUpdate_Internal(World2D, WorldRotation, RootYawOffset);

	// Update all cardinals - these are bound in USimpleAnimInstance::NativeInitializeAnimation
	for (auto& CardinalItr : GetCardinals())
	{
		FSimpleCardinal& Cardinal = CardinalItr.Value;
		if (LIKELY(ensure(Cardinal.bEnabled)))  // Probably shouldn't have been cached if it can be false
		{
			const FGameplayTag& ModeTag = CardinalItr.Key;
			ensure(Cardinal.UpdateDelegate.ExecuteIfBound(ModeTag, Cardinal, *this));
		}
	}
	
	bHasEverUpdated = true;
}

void FSimpleCardinals::ThreadSafeUpdate_Internal(const FSimpleMovement& World2D, const FRotator& WorldRotation, float RootYawOffset)
{
	if (!bHasCachedCardinals)
	{
		CacheCardinals();
	}
	
	VelocityNoOffset = CalculateDirection(World2D.Velocity, WorldRotation);
	Velocity = VelocityNoOffset - RootYawOffset;

	AccelerationNoOffset = CalculateDirection(World2D.Acceleration, WorldRotation);
	Acceleration = AccelerationNoOffset - RootYawOffset;
}

FSimpleLocomotionSet::FSimpleLocomotionSet()
	: Mode(FSimpleGameplayTags::Simple_Cardinal_Mode_4Way)
	, Forward(nullptr)
	, ForwardLeft(nullptr)
	, ForwardRight(nullptr)
	, Left(nullptr)
	, Right(nullptr)
	, LeftAway(nullptr)
	, RightAway(nullptr)
	, Backward(nullptr)
	, BackwardLeft(nullptr)
	, BackwardRight(nullptr)
	, CardinalType(ESimpleCardinalType::VelocityNoOffset)
{}

UAnimSequence* FSimpleLocomotionSet::GetAnimation(const FGameplayTag& CardinalTag) const
{
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Forward)			{ return Forward; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Forward_Left)		{ return ForwardLeft; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Forward_Right)	{ return ForwardRight; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Left)				{ return Left; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Right)			{ return Right; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Left_Away)		{ return LeftAway; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Right_Away)		{ return RightAway; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Backward)			{ return Backward; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Backward_Left)	{ return BackwardLeft; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Backward_Right)	{ return BackwardRight; }
	ensure(false);
	return nullptr;
}

FSimpleGaitSet::FSimpleGaitSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_Gait_Walk);

	// Walking should fall back to run if unavailable, otherwise sprint
	FSimpleGameplayTagArray& WalkFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Gait_Walk);
	WalkFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Run);
	WalkFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Sprint);

	// Run should fall back to walk if unavailable, otherwise sprint
	FSimpleGameplayTagArray& RunFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Gait_Run);
	RunFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Walk);
	RunFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Sprint);
	
	// Sprint should fall back to run if unavailable, otherwise walk
	FSimpleGameplayTagArray& SprintFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Gait_Sprint);
	SprintFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Run);
	SprintFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Walk);
}

void FSimpleGaitSet::SetCardinalType(ESimpleCardinalType CardinalType)
{
	for (auto& GaitItr : Sets)
	{
		FSimpleLocomotionSet& LocoSet = GaitItr.Value;
		LocoSet.CardinalType = CardinalType;
	}
}

FSimpleLocomotionSet* FSimpleGaitSet::GetLocomotionSet(const FGameplayTag& GaitTag)
{
	// Return this gait set if available
	if (FSimpleLocomotionSet* Set = Sets.Find(GaitTag))
	{
		return Set;
	}

	// Otherwise fallback to the closest available
	if (const FSimpleGameplayTagArray* Fallback = Fallbacks.Find(GaitTag))
	{
		for (const FGameplayTag& Tag : Fallback->GetGameplayTagArray())
		{
			if (FSimpleLocomotionSet* Set = Sets.Find(Tag))
			{
				return Set;
			}
		}
	}

	// No available gait set
	return nullptr;
}

const FSimpleLocomotionSet* FSimpleGaitSet::GetLocomotionSet(const FGameplayTag& GaitTag) const
{
	// Return this gait set if available
	if (const FSimpleLocomotionSet* Set = Sets.Find(GaitTag))
	{
		return Set;
	}

	// Otherwise fallback to the closest available
	if (const FSimpleGameplayTagArray* Fallback = Fallbacks.Find(GaitTag))
	{
		for (const FGameplayTag& Tag : Fallback->GetGameplayTagArray())
		{
			if (const FSimpleLocomotionSet* Set = Sets.Find(Tag))
			{
				return Set;
			}
		}
	}

	// No available gait set
	return nullptr;
}

float FSimpleCardinals::CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation)
{
	// Copied from UKismetAnimationLibrary::CalculateDirection
	
	if (!Velocity.IsNearlyZero())
	{
		const FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		const FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		const FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		const FVector NormalizedVel = Velocity.GetSafeNormal2D();

		// get a cos(alpha) of forward vector vs velocity
		const float ForwardCosAngle = static_cast<float>(FVector::DotProduct(ForwardVector, NormalizedVel));
		// now get the alpha and convert to degree
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		const float RightCosAngle = static_cast<float>(FVector::DotProduct(RightVector, NormalizedVel));
		if (RightCosAngle < 0.f)
		{
			ForwardDeltaDegree *= -1.f;
		}

		return ForwardDeltaDegree;
	}

	return 0.f;
}

void FSimpleCardinals::CacheCardinals()
{
	CachedCardinals.Reset();
	for (auto& CardinalItr : Cardinals)
	{
		FSimpleCardinal& Cardinal = CardinalItr.Value;
		if (Cardinal.bEnabled)
		{
			CachedCardinals.Add(CardinalItr);
		}
	}
	bHasCachedCardinals = true;
}

const TMap<FGameplayTag, FSimpleCardinal>& FSimpleCardinals::GetCardinals(ESimpleCardinalCache Mode) const
{
	if (!bHasCachedCardinals && Mode == ESimpleCardinalCache::CachedEnabledOnly)
	{
		FSimpleCardinals* MutableThis = const_cast<FSimpleCardinals*>(this);
		MutableThis->CacheCardinals();
	}
	return Mode == ESimpleCardinalCache::IncludeDisabled ? Cardinals : CachedCardinals;
}

TMap<FGameplayTag, FSimpleCardinal>& FSimpleCardinals::GetCardinals(ESimpleCardinalCache Mode)
{
	if (!bHasCachedCardinals && Mode == ESimpleCardinalCache::CachedEnabledOnly)
	{
		CacheCardinals();
	}
	return Mode == ESimpleCardinalCache::IncludeDisabled ? Cardinals : CachedCardinals;
}