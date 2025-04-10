// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleLocomotionStatics.h"

#include "SimpleGameplayTags.h"
#include "SimpleLocomotionTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SimpleLocomotionStatics)

FGameplayTag USimpleLocomotionStatics::GetSimpleStrafeCardinal(const FSimpleCardinals& Cardinals, const FSimpleStrafeLocomotionSet& Set)
{
	return Cardinals.GetCurrentCardinal(Set);
}

FGameplayTag USimpleLocomotionStatics::GetSimpleStartCardinal(const FSimpleCardinals& Cardinals,
	const FSimpleStartLocomotionSet& Set)
{
	return Cardinals.GetCurrentCardinal(Set);
}

FGameplayTag USimpleLocomotionStatics::GetSimpleTurnCardinal(const FSimpleCardinals& Cardinals,
	const FSimpleTurnLocomotionSet& Set)
{
	return Cardinals.GetCurrentCardinal(Set);
}

FGameplayTag USimpleLocomotionStatics::GetSimpleCardinalForTag(const FSimpleCardinals& Cardinals, FGameplayTag CardinalMode, ESimpleCardinalType CardinalType)
{
	return Cardinals.GetCurrentCardinal(CardinalMode, CardinalType);
}

FGameplayTag USimpleLocomotionStatics::SelectSimpleCardinalFromAngle(const FGameplayTag& CardinalMode, float Angle, float DeadZone, const FGameplayTag& CurrentDirection, bool bWasMovingLastUpdate)
{
	if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Strafe_1Way)
	{
		return FSimpleGameplayTags::Simple_Cardinal_Forward;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Strafe_2Way)
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
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Strafe_4Way)
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
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Strafe_6Way)
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
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Strafe_8Way)
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
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Strafe_10Way)
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
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Start_1Way)
	{
		return FSimpleGameplayTags::Simple_Cardinal_Forward;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Start_2Way)
	{
		const float AngleAbs = FMath::Abs(Angle);
		if (AngleAbs <= 90.f)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Forward;
		}
		const bool bRight = Angle >= 0.f;
		return bRight ? FSimpleGameplayTags::Simple_Cardinal_BackwardTurnRight : FSimpleGameplayTags::Simple_Cardinal_BackwardTurnLeft;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Start_4Way)
	{
		const FGameplayTag BasicCardinal = SelectBasicCardinalFromAngle(FSimpleGameplayTags::Simple_Mode_Strafe_4Way, Angle);  // No dead zone for starts
		if (BasicCardinal == FSimpleGameplayTags::Simple_Cardinal_Backward)
		{
			const bool bRight = Angle >= 0.f;
			return bRight ? FSimpleGameplayTags::Simple_Cardinal_BackwardTurnRight : FSimpleGameplayTags::Simple_Cardinal_BackwardTurnLeft;
		}
		return BasicCardinal;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Start_8Way)
	{
		const FGameplayTag BasicCardinal = SelectBasicCardinalFromAngle(FSimpleGameplayTags::Simple_Mode_Strafe_8Way, Angle);  // No dead zone for starts
		if (BasicCardinal == FSimpleGameplayTags::Simple_Cardinal_Backward)
		{
			const bool bRight = Angle >= 0.f;
			return bRight ? FSimpleGameplayTags::Simple_Cardinal_BackwardTurnRight : FSimpleGameplayTags::Simple_Cardinal_BackwardTurnLeft;
		}
		return BasicCardinal;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Turn_1Way)
	{
		const bool bRight = Angle >= 0.f;
		return bRight ? FSimpleGameplayTags::Simple_Cardinal_BackwardTurnRight : FSimpleGameplayTags::Simple_Cardinal_BackwardTurnLeft;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Turn_4Way)
	{
		const FGameplayTag BasicCardinal = SelectBasicCardinalFromAngle(FSimpleGameplayTags::Simple_Mode_Strafe_4Way, Angle);  // No dead zone for turns
		if (BasicCardinal == FSimpleGameplayTags::Simple_Cardinal_Forward)
		{
			const bool bRight = Angle >= 0.f;
			return bRight ? FSimpleGameplayTags::Simple_Cardinal_Right : FSimpleGameplayTags::Simple_Cardinal_Left;
		}
		else if (BasicCardinal == FSimpleGameplayTags::Simple_Cardinal_Backward)
		{
			const bool bRight = Angle >= 0.f;
			return bRight ? FSimpleGameplayTags::Simple_Cardinal_BackwardTurnRight : FSimpleGameplayTags::Simple_Cardinal_BackwardTurnLeft;
		}
		return BasicCardinal;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Turn_8Way)
	{
		const FGameplayTag BasicCardinal = SelectBasicCardinalFromAngle(FSimpleGameplayTags::Simple_Mode_Strafe_8Way, Angle);  // No dead zone for turns
		if (BasicCardinal == FSimpleGameplayTags::Simple_Cardinal_Forward)
		{
			const bool bRight = Angle >= 0.f;
			return bRight ? FSimpleGameplayTags::Simple_Cardinal_Forward_Right : FSimpleGameplayTags::Simple_Cardinal_Forward_Left;
		}
		else if (BasicCardinal == FSimpleGameplayTags::Simple_Cardinal_Backward)
		{
			const bool bRight = Angle >= 0.f;
			return bRight ? FSimpleGameplayTags::Simple_Cardinal_BackwardTurnRight : FSimpleGameplayTags::Simple_Cardinal_BackwardTurnLeft;
		}
		return BasicCardinal;
	}

	ensure(false);
	return FGameplayTag::EmptyTag;
}

FGameplayTag USimpleLocomotionStatics::SelectBasicCardinalFromAngle(const FGameplayTag& CardinalMode, float Angle)
{
	if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Strafe_1Way)
	{
		return FSimpleGameplayTags::Simple_Cardinal_Forward;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Strafe_2Way)
	{
		// Right
		if (Angle > 0.f)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Right;
		}

		// Left
		return FSimpleGameplayTags::Simple_Cardinal_Left;
	}	
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Strafe_4Way)
	{
		const float AngleAbs = FMath::Abs(Angle);

		// Forward
		if (AngleAbs <= 45.f)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Forward;
		}

		// Backward
		if (AngleAbs >= 135.f)
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
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Strafe_6Way)
	{
		const float AngleAbs = FMath::Abs(Angle);

		// Forward
		if (AngleAbs <= 45.f)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Forward;
		}

		// Backward
		if (AngleAbs >= 135.f)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Backward;
		}

		// Left and Right
		const bool bRight = Angle >= 0.f;
		if (AngleAbs <= 90.f)
		{
			return bRight ? FSimpleGameplayTags::Simple_Cardinal_Right : FSimpleGameplayTags::Simple_Cardinal_Left;
		}
		return bRight ? FSimpleGameplayTags::Simple_Cardinal_Right_Away : FSimpleGameplayTags::Simple_Cardinal_Left_Away;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Strafe_8Way)
	{
		const float AngleAbs = FMath::Abs(Angle);

		// Forward
		if (AngleAbs <= 22.5f)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Forward;
		}

		// Backward
		if (AngleAbs >= 157.5f)
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
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Strafe_10Way)
	{
		const float AngleAbs = FMath::Abs(Angle);

		// Forward
		if (AngleAbs <= 22.5f)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Forward;
		}

		// Backward
		if (AngleAbs >= 157.5f)
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
		if (AngleAbs <= 90.f)
		{
			return bRight ? FSimpleGameplayTags::Simple_Cardinal_Right : FSimpleGameplayTags::Simple_Cardinal_Left;
		}
		return bRight ? FSimpleGameplayTags::Simple_Cardinal_Right_Away : FSimpleGameplayTags::Simple_Cardinal_Left_Away;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Start_1Way)
	{
		return FSimpleGameplayTags::Simple_Cardinal_Forward;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Start_2Way)
	{
		const float AngleAbs = FMath::Abs(Angle);
		if (AngleAbs <= 90.f)
		{
			return FSimpleGameplayTags::Simple_Cardinal_Forward;
		}
		const bool bRight = Angle >= 0.f;
		return bRight ? FSimpleGameplayTags::Simple_Cardinal_BackwardTurnRight : FSimpleGameplayTags::Simple_Cardinal_BackwardTurnLeft;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Start_4Way) 
	{
		const FGameplayTag BasicCardinal = SelectBasicCardinalFromAngle(FSimpleGameplayTags::Simple_Mode_Strafe_4Way, Angle);
		if (BasicCardinal == FSimpleGameplayTags::Simple_Cardinal_Backward)
		{
			const bool bRight = Angle >= 0.f;
			return bRight ? FSimpleGameplayTags::Simple_Cardinal_BackwardTurnRight : FSimpleGameplayTags::Simple_Cardinal_BackwardTurnLeft;
		}
		return BasicCardinal;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Start_8Way)
	{
		const FGameplayTag BasicCardinal = SelectBasicCardinalFromAngle(FSimpleGameplayTags::Simple_Mode_Strafe_8Way, Angle);
		if (BasicCardinal == FSimpleGameplayTags::Simple_Cardinal_Backward)
		{
			const bool bRight = Angle >= 0.f;
			return bRight ? FSimpleGameplayTags::Simple_Cardinal_BackwardTurnRight : FSimpleGameplayTags::Simple_Cardinal_BackwardTurnLeft;
		}
		return BasicCardinal;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Turn_1Way)
	{
		const bool bRight = Angle >= 0.f;
		return bRight ? FSimpleGameplayTags::Simple_Cardinal_BackwardTurnRight : FSimpleGameplayTags::Simple_Cardinal_BackwardTurnLeft;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Turn_4Way)
	{
		const FGameplayTag BasicCardinal = SelectBasicCardinalFromAngle(FSimpleGameplayTags::Simple_Mode_Strafe_4Way, Angle);
		if (BasicCardinal == FSimpleGameplayTags::Simple_Cardinal_Backward)
		{
			const bool bRight = Angle >= 0.f;
			return bRight ? FSimpleGameplayTags::Simple_Cardinal_BackwardTurnRight : FSimpleGameplayTags::Simple_Cardinal_BackwardTurnLeft;
		}
		return BasicCardinal;
	}
	else if (CardinalMode == FSimpleGameplayTags::Simple_Mode_Turn_8Way)
	{
		const FGameplayTag BasicCardinal = SelectBasicCardinalFromAngle(FSimpleGameplayTags::Simple_Mode_Strafe_8Way, Angle);
		if (BasicCardinal == FSimpleGameplayTags::Simple_Cardinal_Backward)
		{
			const bool bRight = Angle >= 0.f;
			return bRight ? FSimpleGameplayTags::Simple_Cardinal_BackwardTurnRight : FSimpleGameplayTags::Simple_Cardinal_BackwardTurnLeft;
		}
		return BasicCardinal;
	}

	ensure(false);
	return FGameplayTag::EmptyTag;
}

FGameplayTag USimpleLocomotionStatics::GetSimpleOppositeCardinal(const FGameplayTag& CardinalTag)
{
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Forward) { return FSimpleGameplayTags::Simple_Cardinal_Backward; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Forward_Left) { return FSimpleGameplayTags::Simple_Cardinal_Backward_Right; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Forward_Right) { return FSimpleGameplayTags::Simple_Cardinal_Backward_Left; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Left) { return FSimpleGameplayTags::Simple_Cardinal_Right; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Right) { return FSimpleGameplayTags::Simple_Cardinal_Left; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Left_Away) { return FSimpleGameplayTags::Simple_Cardinal_Right_Away; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Right_Away) { return FSimpleGameplayTags::Simple_Cardinal_Left_Away; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Backward) { return FSimpleGameplayTags::Simple_Cardinal_Forward; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Backward_Left) { return FSimpleGameplayTags::Simple_Cardinal_Forward_Right; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Backward_Right) { return FSimpleGameplayTags::Simple_Cardinal_Forward_Left; }
	return FGameplayTag::EmptyTag;
}

bool USimpleLocomotionStatics::IsMovementTurningRight(const FVector& Acceleration, const FVector& Velocity,
	const FVector& ActorRightVector)
{
	const FVector VelDir = Velocity.GetSafeNormal();
	const FVector SideBasis = (VelDir ^ ActorRightVector) ^ VelDir;
	const float Dot = (Acceleration.GetSafeNormal() | SideBasis);
	return Dot >= 0.f;
}
