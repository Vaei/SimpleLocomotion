// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "SimpleLocomotionTypes.h"
#include "UObject/Interface.h"
#include "SimpleAnimInterface.generated.h"

/** Call from ACharacter::Landed or equivalent */
DECLARE_DYNAMIC_DELEGATE_OneParam(FSimpleLandedSignature, const FHitResult&, Hit);

UINTERFACE()
class USimpleAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPLELOCOMOTION_API ISimpleAnimInterface
{
	GENERATED_BODY()

public:
	/** AActor::GetVelocity() */
	virtual FVector GetSimpleVelocity() const = 0;

	/** UCharacterMovementComponent::GetCurrentAcceleration() */
	virtual FVector GetSimpleAcceleration() const = 0;

	/** APawn::GetControlRotation() */
	virtual FRotator GetSimpleControlRotation() const = 0;
	
	/** APawn::GetBaseAimRotation() */
	virtual FRotator GetSimpleBaseAimRotation() const = 0;

	/** UCharacterMovementComponent::GetMaxSpeed()  */
	virtual float GetSimpleMaxSpeed() const = 0;
	
	/** Change the rate at which the additive lean occurs optionally based on stance, gait, or other state */
	virtual float GetSimpleLeanRate() const { return 3.75f; };

	/** UCharacterMovementComponent::IsMovingOnGround() */
	virtual bool GetSimpleIsMovingOnGround() const { return true; }

	/** UCharacterMovementComponent::IsFalling() */
	virtual bool GetSimpleIsFalling() const { return false; }

	/** ACharacter::CanJump(). Implementation required if using WantsFrameLockOnLanding() */
	virtual bool GetSimpleCanJump() const { return false; }

	/** ACharacter::bIsCrouched */
	virtual bool GetSimpleIsCrouching() const { return false; }

	/** e.g. AProneCharacter::bIsProned */
	virtual bool GetSimpleIsProning() const { return false; }

	/** e.g. AWalkCharacter::IsWalking() */
	virtual bool GetSimpleIsWalking() const { return false; }

	/** e.g. ASprintCharacter::IsSprinting() */
	virtual bool GetSimpleIsSprinting() const { return false; }

	/** UCharacterMovementComponent::MovementMode != MOVE_None */
	virtual bool GetSimpleIsMoveModeValid() const = 0;

	/** e.g. AMantleCharacter::IsMantling() */
	virtual bool WantsFrameLockOnLanding() const { return false; }

	/** AActor::GetLocalRole() */
	virtual ENetRole GetSimpleLocalRole() const { return ROLE_AutonomousProxy; }
	
	/**
	 * Add to your actor that implements this interface:
	 * 
	 * FSimpleLandedSignature SimpleLandedDelegate;
	 * virtual FSimpleLandedSignature* GetSimpleOnLandedDelegate() override final { return &SimpleLandedDelegate; }
	 *
	 * When they land, e.g. ACharacter::Landed():
	 *
	 * (void)AnimLandedDelegate.ExecuteIfBound(Hit);  // (void) uses the return value so IDE doesn't complain
	 */
	virtual FSimpleLandedSignature* GetSimpleOnLandedDelegate() = 0;
};
