// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "SimpleLocomotionTypes.h"
#include "Animation/AnimInstance.h"
#include "SimpleAnimInstance.generated.h"

class ISimpleAnimInterface;

/**
 * 
 */
UCLASS()
class SIMPLELOCOMOTION_API USimpleAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Transient, DuplicateTransient, Category=References)
	AActor* Owner = nullptr;

	ISimpleAnimInterface* OwnerInterface = nullptr;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	FSimpleMovement World;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	FSimpleMovement World2D;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	FSimpleMovement Local;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	FSimpleMovement Local2D;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	FVector ForwardVector = FVector::ForwardVector;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	FVector RightVector = FVector::RightVector;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	FRotator WorldRotation = FRotator::ZeroRotator;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	FRotator PrevWorldRotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	FRotator ControlRotation = FRotator::ZeroRotator;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	FRotator BaseAimRotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float MaxSpeed = 0.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float LeanRate = 0.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float LeanAngle = 0.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float GravityZ = 1.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float TimeToJumpApex = 0.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bIsMovingOnGround = true;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bInAir = false;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bIsFalling = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bIsJumping = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bCanJump = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bMovementIs3D = false;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bIsCrouching = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bIsProning = false;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bIsWalking = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bIsSprinting = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bIsMoveModeValid = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	bool bHasAcceleration = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	bool bHasVelocity = false;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	ESimpleGaitMode Gait = ESimpleGaitMode::Jog;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	ESimpleStanceMode Stance = ESimpleStanceMode::Stand;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	bool bStanceChanged = false;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	bool bGaitChanged = false;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	TEnumAsByte<ENetRole> LocalRole = ROLE_AutonomousProxy;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=System)
	bool bFirstUpdate = true;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=System)
	bool bWasMovingLastUpdate = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=System)
	bool bWantsLandingFrameLock = false;
	
	/** This prevents poor blending with systems such as mantling */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=System)
	bool bLandingFrameLock = false;
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaTime) override;

	virtual void NativeThreadSafePostUpdateMovementProperties(float DeltaTime) {}
	virtual void NativeThreadSafePreUpdateInAirProperties(float DeltaTime) {}
	virtual void NativeThreadSafeUpdateAnimationPreCompletion(float DeltaTime) {}
	
protected:
	UFUNCTION()
	virtual void OnLanded(const FHitResult& Hit);
	
	virtual bool IsAnimValidToUpdate(float DeltaTime);
};
