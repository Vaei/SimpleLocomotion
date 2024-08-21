// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleLocomotionStatics.h"

#include "SimpleLocomotionTags.h"
#include "SimpleLocomotionTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SimpleLocomotionStatics)

FGameplayTag USimpleLocomotionStatics::GetCurrentCardinal(const FSimpleCardinals& Cardinals, const FSimpleLocomotionSet& LocomotionSet)
{
	return Cardinals.GetCurrentCardinal(LocomotionSet);
}

FGameplayTag USimpleLocomotionStatics::GetCurrentCardinalForTag(const FSimpleCardinals& Cardinals, FGameplayTag CardinalMode, ESimpleCardinalType CardinalType)
{
	return Cardinals.GetCurrentCardinal(CardinalMode, CardinalType);
}

FGameplayTag USimpleLocomotionStatics::SelectCardinalFromAngle(const FGameplayTag& CardinalMode, float Angle, float DeadZone, const FGameplayTag& CurrentDirection, bool bWasMovingLastUpdate)
{
	if (CardinalMode == FSimpleGameplayTags::Simple_Mode_1Way)
	{
		return FSimpleGameplayTags::Simple_Cardinal_Forward;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_2Way)
	{
		if (bWasMovingLastUpdate)
		{
			// Apply dead-zone when trying to change directions to make it harder, so we don't rapidly toggle directions
			if (CurrentDirection == FSimpleGameplayTags::Simple_Cardinal_Left)
			{
				DeadZone *= -1.f;
			}
		}

		// Right
		if (Angle > 0.f + DeadZone)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Right;
		}

		// Left
		return FSimpleGameplayTags::Simple_Cardinal_Left;
	}	
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_4Way)
	{
		const float AngleAbs = FMath::Abs(Angle);
		float DeadZoneFwd = DeadZone;
		float DeadZoneBwd = DeadZone;

		if (bWasMovingLastUpdate)
		{
			// If moving forward or backward, double the dead zone
			// Because it should be harder to leave fwd when moving fwd already
			// When moving left or right the dead zone will be smaller, so we don't rapidly toggle direction shifts

			if (CurrentDirection == FSimpleGameplayTags::Simple_Cardinal_Forward)
			{
				DeadZoneFwd *= 2.f;
			}
			else if (CurrentDirection == FSimpleGameplayTags::Simple_Cardinal_Backward)
			{
				DeadZoneBwd *= 2.f;
			}
		}

		// Forward
		if (AngleAbs <= 45.f + DeadZoneFwd)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Forward;
		}

		// Backward
		if (AngleAbs >= 135.f - DeadZoneBwd)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Backward;
		}

		// Right
		if (Angle > 0.f)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Right;
		}

		// Left
		return FSimpleGameplayTags::Simple_Cardinal_Left;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_6Way)
	{
		const float AngleAbs = FMath::Abs(Angle);
		float DeadZoneFwd = DeadZone;
		float DeadZoneBwd = DeadZone;

		if (bWasMovingLastUpdate)
		{
			// If moving forward or backward, double the dead zone
			// Because it should be harder to leave fwd when moving fwd already
			// When moving left or right the dead zone will be smaller so we don't rapidly toggle direction shifts

			if (CurrentDirection == FSimpleGameplayTags::Simple_Cardinal_Forward)
			{
				DeadZoneFwd *= 2.f;
			}
			else if (CurrentDirection == FSimpleGameplayTags::Simple_Cardinal_Backward)
			{
				DeadZoneBwd *= 2.f;
			}
		}

		// Forward
		if (AngleAbs <= 45.f + DeadZoneFwd)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Forward;
		}

		// Backward
		if (AngleAbs >= 135.f - DeadZoneBwd)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Backward;
		}

		// Left and Right
		const bool bRight = Angle >= 0.f;
		if (AngleAbs <= 90.f + DeadZoneFwd)
		{
			return bRight ? FSimpleGameplayTags::Simple_Cardinal_Right : FSimpleGameplayTags::Simple_Cardinal_Left;
		}
		return bRight ? FSimpleGameplayTags::Simple_Cardinal_Right_Away : FSimpleGameplayTags::Simple_Cardinal_Left_Away;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_8Way)
	{
		const float AngleAbs = FMath::Abs(Angle);
		float DeadZoneFwd = DeadZone;
		float DeadZoneBwd = DeadZone;
	
		if (bWasMovingLastUpdate)
		{
			// If moving forward or backward, double the dead zone
			// Because it should be harder to leave fwd when moving fwd already
			// When moving left or right the dead zone will be smaller so we don't rapidly toggle direction shifts

			if (CurrentDirection == FSimpleGameplayTags::Simple_Cardinal_Forward)
			{
				DeadZoneFwd *= 2.f;
			}
			else if (CurrentDirection == FSimpleGameplayTags::Simple_Cardinal_Backward)
			{
				DeadZoneBwd *= 2.f;
			}
		}

		// Forward
		if (AngleAbs <= 22.5f + DeadZoneFwd)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Forward;
		}

		// Backward
		if (AngleAbs >= 157.5f - DeadZoneBwd)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Backward;
		}

		// Diagonal Fwd
		if (AngleAbs <= 67.5f)
		{
			return Angle > 0.f ? FSimpleGameplayTags::Simple_Cardinal_Forward_Right : FSimpleGameplayTags::Simple_Cardinal_Forward_Left;
		}

		// Diagonal Bwd
		if (AngleAbs >= 112.5f)
		{
			return Angle > 0.f ? FSimpleGameplayTags::Simple_Cardinal_Backward_Right : FSimpleGameplayTags::Simple_Cardinal_Backward_Left;
		}

		// Right
		if (Angle > 0.f)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Right;
		}

		// Left
		return FSimpleGameplayTags::Simple_Cardinal_Left;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_10Way)
	{
		const float AngleAbs = FMath::Abs(Angle);
		float DeadZoneFwd = DeadZone;
		float DeadZoneBwd = DeadZone;
	
		if (bWasMovingLastUpdate)
		{
			// If moving forward or backward, double the dead zone
			// Because it should be harder to leave fwd when moving fwd already
			// When moving left or right the dead zone will be smaller so we don't rapidly toggle direction shifts

			if (CurrentDirection == FSimpleGameplayTags::Simple_Cardinal_Forward)
			{
				DeadZoneFwd *= 2.f;
			}
			else if (CurrentDirection == FSimpleGameplayTags::Simple_Cardinal_Backward)
			{
				DeadZoneBwd *= 2.f;
			}
		}

		// Forward
		if (AngleAbs <= 22.5f + DeadZoneFwd)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Forward;
		}

		// Backward
		if (AngleAbs >= 157.5f - DeadZoneBwd)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Backward;
		}

		// Diagonal Fwd
		if (AngleAbs <= 67.5f)
		{
			return Angle > 0.f ? FSimpleGameplayTags::Simple_Cardinal_Forward_Right : FSimpleGameplayTags::Simple_Cardinal_Forward_Left;
		}

		// Diagonal Bwd
		if (AngleAbs >= 112.5f)
		{
			return Angle > 0.f ? FSimpleGameplayTags::Simple_Cardinal_Backward_Right : FSimpleGameplayTags::Simple_Cardinal_Backward_Left;
		}

		// Left and Right
		const bool bRight = Angle >= 0.f;
		if (AngleAbs <= 90.f + DeadZoneFwd)
		{
			return bRight ? FSimpleGameplayTags::Simple_Cardinal_Right : FSimpleGameplayTags::Simple_Cardinal_Left;
		}
		return bRight ? FSimpleGameplayTags::Simple_Cardinal_Right_Away : FSimpleGameplayTags::Simple_Cardinal_Left_Away;
	}
	return FGameplayTag::EmptyTag;
}

FGameplayTag USimpleLocomotionStatics::GetOppositeCardinal(const FGameplayTag& Cardinal)
{
	if (Cardinal == FSimpleGameplayTags::Simple_Cardinal_Forward) { return FSimpleGameplayTags::Simple_Cardinal_Backward; }
	if (Cardinal == FSimpleGameplayTags::Simple_Cardinal_Forward_Left) { return FSimpleGameplayTags::Simple_Cardinal_Backward_Right; }
	if (Cardinal == FSimpleGameplayTags::Simple_Cardinal_Forward_Right) { return FSimpleGameplayTags::Simple_Cardinal_Backward_Left; }
	if (Cardinal == FSimpleGameplayTags::Simple_Cardinal_Left) { return FSimpleGameplayTags::Simple_Cardinal_Right; }
	if (Cardinal == FSimpleGameplayTags::Simple_Cardinal_Right) { return FSimpleGameplayTags::Simple_Cardinal_Left; }
	if (Cardinal == FSimpleGameplayTags::Simple_Cardinal_Left_Away) { return FSimpleGameplayTags::Simple_Cardinal_Right_Away; }
	if (Cardinal == FSimpleGameplayTags::Simple_Cardinal_Right_Away) { return FSimpleGameplayTags::Simple_Cardinal_Left_Away; }
	if (Cardinal == FSimpleGameplayTags::Simple_Cardinal_Backward) { return FSimpleGameplayTags::Simple_Cardinal_Forward; }
	if (Cardinal == FSimpleGameplayTags::Simple_Cardinal_Backward_Left) { return FSimpleGameplayTags::Simple_Cardinal_Forward_Right; }
	if (Cardinal == FSimpleGameplayTags::Simple_Cardinal_Backward_Right) { return FSimpleGameplayTags::Simple_Cardinal_Forward_Left; }
	return FGameplayTag::EmptyTag;
}

FSimpleLocomotionSet USimpleLocomotionStatics::GetSimpleLocomotionSet(const FSimpleGaitSet& GaitSet, FGameplayTag GaitTag, bool& bValid)
{
	bValid = false;
	if (const FSimpleLocomotionSet* LocomotionSet = GaitSet.GetLocomotionSet(GaitTag))
	{
		bValid = true;
		return *LocomotionSet;
	}
	return {};
}