// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "SimpleLocomotionTypes.h"
#include "Components/ActorComponent.h"
#include "SimpleAnimComponent.generated.h"


/** Call from ACharacter::Landed or equivalent */
DECLARE_DYNAMIC_DELEGATE_OneParam(FSimpleLandedSignature, const FHitResult&, Hit);

UCLASS(Abstract, ClassGroup=(Custom))
class SIMPLELOCOMOTION_API USimpleAnimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	/** Assist designers by cleaning out categories they don't need to see */
	UPROPERTY(EditDefaultsOnly, Category=Editor)
	TArray<FName> HideEditorCategories = { "Sockets", "Tags", "ComponentTick", "ComponentReplication",
		"Activation", "Cooking", "Events", "AssetUserData", "Replication", "Navigation" };
	
	UPROPERTY(EditDefaultsOnly, Category=Editor)
	TArray<FName> ImportantEditorCategories = {};
#endif
	
public:
	USimpleAnimComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void OnRegister() override;
	virtual void InitializeComponent() override;
	virtual void PostLoad() override;

protected:
	/** This is where you cast the owner and cache it along with any other references */
	virtual void SetUpdatedCharacter() {}
	
public:
	/** AActor::GetVelocity() */
	virtual FVector GetSimpleVelocity() const PURE_VIRTUAL(, return FVector::ZeroVector;);

	/** UCharacterMovementComponent::GetCurrentAcceleration() */
	virtual FVector GetSimpleAcceleration() const PURE_VIRTUAL(, return FVector::ZeroVector;);

	/** APawn::GetControlRotation() */
	virtual FRotator GetSimpleControlRotation() const PURE_VIRTUAL(, return FRotator::ZeroRotator;);
	
	/** APawn::GetBaseAimRotation() */
	virtual FRotator GetSimpleBaseAimRotation() const PURE_VIRTUAL(, return FRotator::ZeroRotator;);

	/** UCharacterMovementComponent::GetMaxSpeed() */
	virtual float GetSimpleMaxSpeed() const PURE_VIRTUAL(, return 0.f;);

	/** UCharacterMovementComponent::MaxWalkSpeed and optional custom implementations of Walk and Sprint speeds */
	virtual FSimpleGaitSpeed GetSimpleMaxGaitSpeeds() const PURE_VIRTUAL(, return 0.f;);
	
	/** Change the rate at which the additive lean occurs optionally based on stance, gait, or other state */
	virtual float GetSimpleLeanRate() const { return 3.75f; };
	
	/** Used with TurnInPlace systems where usually a mesh offset is applied */
	virtual float GetSimpleRootYawOffset() const { return 0.f; };

	/** UCharacterMovementComponent::GetGravityZ()  */
	virtual float GetSimpleGravityZ() const { return 1.f; }
	
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
	
	/** e.g. UWalkMovement::bWantsToWalk && USprintMovement::CanWalk() */
	virtual bool GetSimpleWantsWalking() const { return false; }

	/** e.g. USprintMovement::bWantsToSprint && USprintMovement::CanSprint() */
	virtual bool GetSimpleWantsSprinting() const { return false; }

	/** e.g. UCharacterMovementComponent::IsSwimming() || UCharacterMovementComponent::IsFlying() */
	virtual bool GetSimpleMovementIs3D() const { return false; }
	
	/** UCharacterMovementComponent::MovementMode != MOVE_None */
	virtual bool GetSimpleIsMoveModeValid() const { return true; };

	/** e.g. AMantleCharacter::IsMantling() */
	virtual bool WantsFrameLockOnLanding() const { return false; }

	/** AActor::GetLocalRole() */
	virtual ENetRole GetSimpleLocalRole() const PURE_VIRTUAL(, return ROLE_None;)
	
	/**
	 * Add to your actor that owns this component:
	 * 
	 * FSimpleLandedSignature SimpleLandedDelegate;
	 *
	 * Then override on your component:
	 * 
	 * virtual FSimpleLandedSignature* GetSimpleOnLandedDelegate() override final { return &Owner->SimpleLandedDelegate; }
	 *
	 * When they land, e.g. ACharacter::Landed():
	 *
	 * Super::Landed(Hit);
	 * (void)ThisComponent->AnimLandedDelegate.ExecuteIfBound(Hit);  // (void) uses the return value so IDE doesn't complain
	 */
	virtual FSimpleLandedSignature* GetSimpleOnLandedDelegate() { return nullptr; }
};