// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "SimpleLocomotionTypes.generated.h"

/** Call from ACharacter::Landed or equivalent */
DECLARE_DYNAMIC_DELEGATE_OneParam(FSimpleLandedSignature, const FHitResult&, Hit);

UENUM(BlueprintType)
enum class ESimpleIsValidResult : uint8
{
	Valid,
	NotValid,
};

UENUM(BlueprintType)
enum class ESimpleCardinalType : uint8
{
	Acceleration,
	AccelerationNoOffset,
	Velocity,
	VelocityNoOffset,
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
		float InVelocity = 0.f,
		float InVelocityNoOffset = 0.f,
		float InAcceleration = 0.f,
		float InAccelerationNoOffset = 0.f
		)
		: Velocity(InVelocity)
		, VelocityNoOffset(InVelocityNoOffset)
		, Acceleration(InAcceleration)
		, AccelerationNoOffset(InAccelerationNoOffset)
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
	float Velocity;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float VelocityNoOffset;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float Acceleration;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float AccelerationNoOffset;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bHasEverUpdated;

	float GetDirectionAngle(ESimpleCardinalType CardinalType) const;

	void ThreadSafeUpdate(const FSimpleMovement& World2D, const FRotator& WorldRotation, float RootYawOffset);

	void ThreadSafeUpdate_Internal(const FSimpleMovement& World2D, const FRotator& WorldRotation, float RootYawOffset);

	static float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation);
};
