// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "SimpleLocomotionTypes.h"
#include "Components/ActorComponent.h"
#include "SimpleAnimComponent.generated.h"

#define NOT_IMPLEMENTED { NotImplemented(*FString(__FUNCTION__)); }

UCLASS(Abstract, Blueprintable, ClassGroup=(Custom))
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

	UPROPERTY(Transient, DuplicateTransient)
	TArray<FString> ConsumedDebugFunctions;
#endif
	
public:
	USimpleAnimComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void OnRegister() override;
	virtual void InitializeComponent() override;
	virtual void PostLoad() override;
	virtual void BeginPlay() override;
	
protected:
	void NotImplemented(const FString& FunctionName) const;
	
	/** This is where you cast the owner and cache it along with any other references */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	void SetUpdatedCharacter();
	virtual void SetUpdatedCharacter_Implementation() {}
	
public:
	/** AActor::GetVelocity() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	FVector GetSimpleVelocity() const;
	virtual FVector GetSimpleVelocity_Implementation() const { NOT_IMPLEMENTED return FVector::ZeroVector; };

	/** UCharacterMovementComponent::GetCurrentAcceleration() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	FVector GetSimpleAcceleration() const;
	virtual FVector GetSimpleAcceleration_Implementation() const { NOT_IMPLEMENTED return FVector::ZeroVector; };

	/** APawn::GetControlRotation() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	FRotator GetSimpleControlRotation() const;
	virtual FRotator GetSimpleControlRotation_Implementation() const { NOT_IMPLEMENTED return FRotator::ZeroRotator; };
	
	/** APawn::GetBaseAimRotation() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	FRotator GetSimpleBaseAimRotation() const;
	virtual FRotator GetSimpleBaseAimRotation_Implementation() const { NOT_IMPLEMENTED return FRotator::ZeroRotator; };

	/** UCharacterMovementComponent::GetMaxSpeed() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	float GetSimpleMaxSpeed() const;
	virtual float GetSimpleMaxSpeed_Implementation() const { NOT_IMPLEMENTED return 0.f; };

	/**
	 * UCharacterMovementComponent::MaxWalkSpeed and optional custom implementations of Walk and Sprint speeds
	 * All queried gait modes should be included, otherwise they will return 0 and could cause unexpected results
	 *
	 * EXAMPLE IMPLEMENTATION
	 * TMap<FGameplayTag, float> MaxSpeeds { { FSimpleGameplayTags::Simple_Gait_Walk, GetSimpleMaxSpeed() },
	 * { FSimpleGameplayTags::Simple_Gait_Run, GetSimpleMaxSpeed() }, { FSimpleGameplayTags::Simple_Gait_Sprint, GetSimpleMaxSpeed() } };
	 * return FSimpleGaitSpeed { MaxSpeeds };
	 */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	FSimpleGaitSpeed GetSimpleMaxGaitSpeeds() const;
	virtual FSimpleGaitSpeed GetSimpleMaxGaitSpeeds_Implementation() const { NOT_IMPLEMENTED return {}; };
	
	/** Change the rate at which the additive lean occurs optionally based on stance, gait, or other state */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	float GetSimpleLeanRate() const;
	virtual float GetSimpleLeanRate_Implementation() const { return 3.75f; };
	
	/** Used with TurnInPlace systems where usually a mesh offset is applied */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	float GetSimpleRootYawOffset() const;
	virtual float GetSimpleRootYawOffset_Implementation() const { return 0.f; };

	/** UCharacterMovementComponent::GetGravityZ()  */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	float GetSimpleGravityZ() const;
	virtual float GetSimpleGravityZ_Implementation() const { return 1.f; }

	/** UCharacterMovementComponent::CurrentFloor.IsWalkableFloor() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	bool IsSimpleCurrentFloorWalkable() const;
	virtual bool IsSimpleCurrentFloorWalkable_Implementation() const { return true; }

	/** UCharacterMovementComponent::IsMovingOnGround() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	bool GetSimpleIsMovingOnGround() const;
	virtual bool GetSimpleIsMovingOnGround_Implementation() const { return true; }

	/** UCharacterMovementComponent::IsFalling() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	bool GetSimpleIsFalling() const;
	virtual bool GetSimpleIsFalling_Implementation() const { return false; }

	/** ACharacter::CanJump(). Implementation required if using WantsFrameLockOnLanding() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	bool GetSimpleCanJump() const;
	virtual bool GetSimpleCanJump_Implementation() const { return false; }

	/** ACharacter::bIsCrouched */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	bool GetSimpleIsCrouching() const;
	virtual bool GetSimpleIsCrouching_Implementation() const { return false; }

	/** e.g. AStrollCharacter::IsStrolling() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	bool GetSimpleIsStrolling() const;
	virtual bool GetSimpleIsStrolling_Implementation() const { return false; }
	
	/** e.g. AWalkCharacter::IsWalking() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	bool GetSimpleIsWalking() const;
	virtual bool GetSimpleIsWalking_Implementation() const { return false; }

	/** e.g. ASprintCharacter::IsSprinting() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	bool GetSimpleIsSprinting() const;
	virtual bool GetSimpleIsSprinting_Implementation() const { return false; }

	/** e.g. UStrollMovement::bWantsToStroll && USprintMovement::CanStroll() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	bool GetSimpleWantsStrolling() const;
	virtual bool GetSimpleWantsStrolling_Implementation() const { return false; }
	
	/** e.g. UWalkMovement::bWantsToWalk && USprintMovement::CanWalk() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	bool GetSimpleWantsWalking() const;
	virtual bool GetSimpleWantsWalking_Implementation() const { return false; }

	/** e.g. USprintMovement::bWantsToSprint && USprintMovement::CanSprint() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	bool GetSimpleWantsSprinting() const;
	virtual bool GetSimpleWantsSprinting_Implementation() const { return false; }

	/** e.g. UCharacterMovementComponent::IsSwimming() || UCharacterMovementComponent::IsFlying() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	bool GetSimpleMovementIs3D() const;
	virtual bool GetSimpleMovementIs3D_Implementation() const { return false; }
	
	/** UCharacterMovementComponent::MovementMode != MOVE_None */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	bool GetSimpleIsMoveModeValid() const;
	virtual bool GetSimpleIsMoveModeValid_Implementation() const { return true; };

	/** e.g. AMantleCharacter::IsMantling() */
	virtual bool WantsFrameLockOnLanding() const { return false; }

	/** AActor::GetLocalRole() */
	UFUNCTION(BlueprintNativeEvent, Category=Animation)
	ENetRole GetSimpleLocalRole() const;
	virtual ENetRole GetSimpleLocalRole_Implementation() const { NOT_IMPLEMENTED return ROLE_None; };
	
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