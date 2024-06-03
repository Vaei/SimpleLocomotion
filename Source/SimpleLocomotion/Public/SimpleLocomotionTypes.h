// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "SimpleLocomotionTypes.generated.h"

UENUM(BlueprintType)
enum class ESimpleIsValidResult : uint8
{
	Valid,
	NotValid,
};

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
struct SIMPLELOCOMOTION_API FSimpleGaitSpeed
{
	GENERATED_BODY()
	
	FSimpleGaitSpeed(
		float InWalkSpeed = 0.f,
		float InJogSpeed = 0.f,
		float InSprintSpeed = 0.f
		)
		: WalkSpeed(InWalkSpeed)
		, JogSpeed(InJogSpeed)
		, SprintSpeed(InSprintSpeed)
	{}

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float WalkSpeed;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float JogSpeed;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float SprintSpeed;
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
		return FSimpleMovement {
			WorldRotation.UnrotateVector(Velocity), WorldRotation.UnrotateVector(Acceleration)
		};
	}

	float GetSpeed() const
	{
		return Velocity.Size();
	}
};

USTRUCT(BlueprintType)
struct SIMPLELOCOMOTION_API FSimpleCardinalMovement
{
	GENERATED_BODY()
	
	FSimpleCardinalMovement(
		float InVelocityDirectionAngle = 0.f,
		float InVelocityDirectionAngleWithOffset = 0.f,
		float InAccelerationDirectionAngle = 0.f,
		float InAccelerationDirectionAngleWithOffset = 0.f
		)
		: VelocityDirectionAngle(InVelocityDirectionAngle)
		, VelocityDirectionAngleWithOffset(InVelocityDirectionAngleWithOffset)
		, AccelerationDirectionAngle(InAccelerationDirectionAngle)
		, AccelerationDirectionAngleWithOffset(InAccelerationDirectionAngleWithOffset)
		, bHasEverUpdated(false)
	{}
	
	FSimpleCardinalMovement(
		const FSimpleMovement& World2D,
		const FRotator& WorldRotation,
		float RootYawOffset
		)
		: bHasEverUpdated(false)
	{
		ThreadSafeUpdate_Internal(World2D, WorldRotation, RootYawOffset);
	}

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float VelocityDirectionAngle;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float VelocityDirectionAngleWithOffset;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float AccelerationDirectionAngle;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float AccelerationDirectionAngleWithOffset;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bHasEverUpdated;

	void ThreadSafeUpdate(const FSimpleMovement& World2D, const FRotator& WorldRotation, float RootYawOffset);

	void ThreadSafeUpdate_Internal(const FSimpleMovement& World2D, const FRotator& WorldRotation, float RootYawOffset);

	static float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation);
};
