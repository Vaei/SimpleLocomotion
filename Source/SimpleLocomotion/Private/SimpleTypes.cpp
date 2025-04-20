// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleTypes.h"

#include "SimpleTags.h"
#include "SimpleSets.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(SimpleTypes)

DEFINE_LOG_CATEGORY_STATIC(LogSimpleTypes, Log, All);

float FSimpleGaitSpeed::GetMaxSpeed(const FGameplayTag& GaitTag)
{
	if (const float* MaxSpeed = MaxSpeeds.Find(GaitTag))
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
		case ESimpleCardinalType::Velocity: return Velocity;
	}
	return FGameplayTag::EmptyTag;
}

void FSimpleCardinals::ConstructDefaultCardinals()
{
	// 1-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleTags::Simple_Mode_Strafe_1Way);
		Cardinal.Tags = ConstructCardinalTags_1Way();
		Cardinal.bEnabled = false;
	}
	// 2-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleTags::Simple_Mode_Strafe_2Way);
		Cardinal.Tags = ConstructCardinalTags_2Way();
		Cardinal.bEnabled = false;
	}
	// 4-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleTags::Simple_Mode_Strafe_4Way);
		Cardinal.Tags = ConstructCardinalTags_4Way();
		Cardinal.bEnabled = true;
	}
	// 6-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleTags::Simple_Mode_Strafe_6Way);
		Cardinal.Tags = ConstructCardinalTags_6Way();
		Cardinal.bEnabled = false;
	}
	// 8-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleTags::Simple_Mode_Strafe_8Way);
		Cardinal.Tags = ConstructCardinalTags_8Way();
		Cardinal.bEnabled = false;
	}
	// 10-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleTags::Simple_Mode_Strafe_10Way);
		Cardinal.Tags = ConstructCardinalTags_10Way();
		Cardinal.bEnabled = false;
	}

	// Start 1-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleTags::Simple_Mode_Start_1Way);
		Cardinal.Tags = ConstructCardinalTags_Start_1Way();
		Cardinal.bEnabled = true;
	}
	// Start 2-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleTags::Simple_Mode_Start_2Way);
		Cardinal.Tags = ConstructCardinalTags_Start_2Way();
		Cardinal.bEnabled = false;
	}
	// Start 4-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleTags::Simple_Mode_Start_4Way);
		Cardinal.Tags = ConstructCardinalTags_Start_4Way();
		Cardinal.bEnabled = false;
	}
	// Start 8-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleTags::Simple_Mode_Start_8Way);
		Cardinal.Tags = ConstructCardinalTags_Start_8Way();
		Cardinal.bEnabled = false;
	}

	// Turn 1-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleTags::Simple_Mode_Turn_1Way);
		Cardinal.Tags = ConstructCardinalTags_Turn_1Way();
		Cardinal.bEnabled = false;
	}
	// Turn 4-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleTags::Simple_Mode_Turn_4Way);
		Cardinal.Tags = ConstructCardinalTags_Turn_4Way();
		Cardinal.bEnabled = false;
	}
	// Turn 8-Way
	{
		FSimpleCardinal& Cardinal = Cardinals.Add(FSimpleTags::Simple_Mode_Turn_8Way);
		Cardinal.Tags = ConstructCardinalTags_Turn_8Way();
		Cardinal.bEnabled = false;
	}
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_1Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_2Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Right);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_4Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Right);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Backward);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_6Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Right);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Left_Away);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Right_Away);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Backward);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_8Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward_Right);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Right);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Backward);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Backward_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Backward_Right);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_10Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward_Right);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Right);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Left_Away);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Right_Away);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Backward);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Backward_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Backward_Right);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_Start_1Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_Start_2Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_BackwardTurnLeft);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_BackwardTurnRight);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_Start_4Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Right);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_BackwardTurnLeft);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_BackwardTurnRight);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_Start_8Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward_Right);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Right);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_BackwardTurnLeft);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_BackwardTurnRight);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Backward_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Backward_Right);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_Turn_1Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_BackwardTurnLeft);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_BackwardTurnRight);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_Turn_4Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Right);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_BackwardTurnLeft);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_BackwardTurnRight);
	return Tags;
}

FGameplayTagContainer FSimpleCardinals::ConstructCardinalTags_Turn_8Way()
{
	FGameplayTagContainer Tags;
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Forward_Right);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Right);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_BackwardTurnLeft);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_BackwardTurnRight);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Backward_Left);
	Tags.AddTagFast(FSimpleTags::Simple_Cardinal_Backward_Right);
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
	ensure(false);  // This cardinal hasn't been enabled on the layer!
	return FGameplayTag::EmptyTag;
}

FGameplayTag FSimpleCardinals::GetCurrentCardinal(const FSimpleStrafeLocoSet& LocomotionSet) const
{
	return GetCurrentCardinal(LocomotionSet.Mode, LocomotionSet.CardinalType);
}

FGameplayTag FSimpleCardinals::GetCurrentCardinal(const FSimpleStrafeLocoSet* LocomotionSet) const
{
	if (LocomotionSet)
	{
		return GetCurrentCardinal(LocomotionSet->Mode, LocomotionSet->CardinalType);
	}
	return FGameplayTag::EmptyTag;
}

FGameplayTag FSimpleCardinals::GetCurrentCardinal(const FSimpleStartLocoSet& LocomotionSet) const
{
	return GetCurrentCardinal(LocomotionSet.Mode, LocomotionSet.CardinalType);
}

FGameplayTag FSimpleCardinals::GetCurrentCardinal(const FSimpleStartLocoSet* LocomotionSet) const
{
	if (LocomotionSet)
	{
		return GetCurrentCardinal(LocomotionSet->Mode, LocomotionSet->CardinalType);
	}
	return FGameplayTag::EmptyTag;
}

FGameplayTag FSimpleCardinals::GetCurrentCardinal(const FSimpleTurnLocoSet* LocomotionSet) const
{
	if (LocomotionSet)
	{
		return GetCurrentCardinal(LocomotionSet->Mode, LocomotionSet->CardinalType);
	}
	return FGameplayTag::EmptyTag;
}

FGameplayTag FSimpleCardinals::GetCurrentCardinal(const FSimpleTurnLocoSet& LocomotionSet) const
{
	return GetCurrentCardinal(LocomotionSet.Mode, LocomotionSet.CardinalType);
}

float FSimpleCardinals::GetDirectionAngle(ESimpleCardinalType CardinalType) const
{
	switch (CardinalType)
	{
		case ESimpleCardinalType::Acceleration: return Acceleration;
		case ESimpleCardinalType::Velocity: return Velocity;
	}
	return 0.f;
}

void FSimpleCardinals::ThreadSafeUpdate(const FSimpleMovement& World2D, const FRotator& WorldRotation)
{
	ThreadSafeUpdate_Internal(World2D, WorldRotation);

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

void FSimpleCardinals::ThreadSafeUpdate_Internal(const FSimpleMovement& World2D, const FRotator& WorldRotation)
{
	if (!bHasCachedCardinals)
	{
		CacheCardinals();
	}
	
	Velocity = CalculateDirection(World2D.Velocity, WorldRotation);
	Acceleration = CalculateDirection(World2D.Acceleration, WorldRotation);
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

void FSimpleCardinals::UpdateCardinals(const TMap<FGameplayTag, FSimpleCardinal>& NewCardinals)
{
	Cardinals = NewCardinals;
	CacheCardinals();
}

void FSimpleCardinals::CacheCardinals()
{
	CachedCardinals.Reset();
	for (auto& CardinalItr : Cardinals)
	{
		const FSimpleCardinal& Cardinal = CardinalItr.Value;
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