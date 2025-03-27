// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "SimpleLocomotionTypes.h"
#include "Animation/AnimInstance.h"
#include "SimpleAnimInstance.generated.h"

class USimpleAnimComponent;

/**
 * 
 */
UCLASS()
class SIMPLELOCOMOTION_API USimpleAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Transient, DuplicateTransient, Category=References)
	TObjectPtr<AActor> Owner = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Transient, DuplicateTransient, Category=References)
	TObjectPtr<APawn> PawnOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient, DuplicateTransient, Category=References)
	TObjectPtr<USimpleAnimComponent> OwnerComponent = nullptr;

#if WITH_EDITORONLY_DATA
	/** Assist designers by cleaning out categories they don't need to see */
	UPROPERTY(EditDefaultsOnly, Category=Editor)
	TArray<FName> HideEditorCategories = { "RootMotion", "Notifies", "Montage" };
	
	UPROPERTY(EditDefaultsOnly, Category=Editor)
	TArray<FName> ImportantEditorCategories = {};
#endif

public:
	/** Required for CardinalMovement to update. If true, NativeThreadSafeUpdateAnimation() calls CardinalMovement.ThreadSafeUpdate() */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	bool bWantsCardinalsUpdated = true;

	/** Required for LeanAngle to update. If true, NativeThreadSafeUpdateAnimation() updates LeanAngle. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	bool bWantsLeansUpdated = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Properties)
	FSimpleCardinals Cardinals;
	
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
	float Speed3D = 0.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float Speed2D = 0.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float Speed = 0.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float PrevSpeed = 0.f;

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
	FSimpleGaitSpeed MaxGaitSpeeds = {};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float MaxSpeed = 0.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float LeanRate = 0.f;

	/** If set, will use this instead of LeanRate */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category=Properties)
	float LeanRateOverride = -1.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float LeanAngle = 0.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float RootYawOffset = 0.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float GravityZ = 1.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	float TimeToJumpApex = 0.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bIsCurrentFloorWalkable = true;

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
	bool bIsCrouched = false;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bIsProned = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bIsStrolling = false;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bIsWalking = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bIsSprinting = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bWantsStrolling = false;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bWantsWalking = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bWantsSprinting = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Properties)
	bool bIsMoveModeValid = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	bool bHasAcceleration = false;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	bool bHasAcceleration2D = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	bool bHasVelocity = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	bool bHasVelocity2D = false;
	
public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	FGameplayTag Gait;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	FGameplayTag StartGait;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	FGameplayTag StopGait;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	FGameplayTag State;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=State)
	FGameplayTag Stance;

protected:
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

	/** This prevents poor blending with systems such as mantling */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=System)
	bool bIsAnyMontagePlaying = false;
	
	/** This prevents poor blending with systems such as mantling */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=System)
	bool bIsPlayingNetworkedRootMotionMontage = false;
	
public:
	USimpleAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaTime) override;

	virtual void NativeThreadSafePreUpdateMovementProperties(float DeltaTime) {}
	virtual void NativeThreadSafeUpdateGaitMode(float DeltaTime);
	virtual void NativeThreadSafeUpdateStance(float DeltaTime);
	virtual void NativeThreadSafePostUpdateMovementProperties(float DeltaTime) {}
	virtual void NativeThreadSafeUpdateFalling(float DeltaTime);
	virtual void NativeThreadSafePreUpdateInAirProperties(float DeltaTime) {}
	virtual void NativeThreadSafeUpdateAnimationPreCompletion(float DeltaTime) {}

	virtual void NativePostEvaluateAnimation() override;

protected:
	void ComputeSlowStopGait(float MaxSpeedStroll, float MaxSpeedWalk, float MaxSpeedRun);

	UFUNCTION()
	virtual void OnLanded(const FHitResult& Hit);
	
	virtual float GetCardinalDeadZone(const FGameplayTag& CardinalMode) const;

	void UpdateCardinal(const FGameplayTag& CardinalMode, FSimpleCardinal& Cardinal, const FSimpleCardinals& InCardinals);
	
	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	float GetLocomotionCardinalAngle(ESimpleCardinalType CardinalType) const;

	UFUNCTION(BlueprintPure, Category=Animation, meta=(BlueprintThreadSafe))
	virtual bool IsAnimValidToUpdate(float DeltaTime) const;
	
	UFUNCTION(BlueprintCallable, Category=Animation, meta=(BlueprintThreadSafe, ExpandEnumAsExecs="ResultIsValid", DisplayName="Is Anim Valid to Update"))
	virtual void K2_IsAnimValidToUpdate(float DeltaTime, ESimpleIsValidResult& ResultIsValid);

	virtual void OnAnimNotValidToUpdate(FString LogError) const;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient, DuplicateTransient)
	TArray<FString> ConsumedDebugMessages;
#endif
};
