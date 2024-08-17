﻿// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleLocomotionTypes.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(SimpleLocomotionTypes)

float FSimpleCardinalMovement::GetDirectionAngle(ESimpleCardinalType CardinalType) const
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

void FSimpleCardinalMovement::ThreadSafeUpdate(const FSimpleMovement& World2D, const FRotator& WorldRotation, float RootYawOffset)
{
	ThreadSafeUpdate_Internal(World2D, WorldRotation, RootYawOffset);
	bHasEverUpdated = true;
}

void FSimpleCardinalMovement::ThreadSafeUpdate_Internal(const FSimpleMovement& World2D, const FRotator& WorldRotation, float RootYawOffset)
{
	VelocityNoOffset = CalculateDirection(World2D.Velocity, WorldRotation);
	Velocity = VelocityNoOffset - RootYawOffset;

	AccelerationNoOffset = CalculateDirection(World2D.Acceleration, WorldRotation);
	Acceleration = AccelerationNoOffset - RootYawOffset;
}

float FSimpleCardinalMovement::CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation)
{
	// Copied from UKismetAnimationLibrary::CalculateDirection
	
	if (!Velocity.IsNearlyZero())
	{
		const FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		const FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		const FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		const FVector NormalizedVel = Velocity.GetSafeNormal2D();

		// get a cos(alpha) of forward vector vs velocity
		const float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
		// now get the alpha and convert to degree
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		const float RightCosAngle = FVector::DotProduct(RightVector, NormalizedVel);
		if (RightCosAngle < 0)
		{
			ForwardDeltaDegree *= -1;
		}

		return ForwardDeltaDegree;
	}

	return 0.f;
}