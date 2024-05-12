// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "SimpleLocomotionTypes.generated.h"

UENUM(BlueprintType)
enum class ESimpleGaitMode : uint8
{
	Walk,
	Jog,
	Sprint,
};

UENUM(BlueprintType)
enum class ESimpleStanceMode : uint8
{
	Stand,
	Crouch,
	Prone,
};

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
		const FVector Velocity2D = { Velocity.X, Velocity.Y, 0.f };
		const FVector Acceleration2D = { Acceleration.X, Acceleration.Y, 0.f };
		return FSimpleMovement { Velocity2D, Acceleration2D };
	}
};