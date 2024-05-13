// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleAnimInstance.h"

#include "SimpleAnimInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SimpleAnimInstance)

DEFINE_LOG_CATEGORY_STATIC(LogSimpleAnim, Log, All);

namespace SimpleAnimInstanceCVars
{
#if WITH_EDITOR
	static bool bPrintFailedValidationToMessageLog = true;
	FAutoConsoleVariableRef CVarPrintFailedValidationToMessageLog(
		TEXT("a.SimpleAnim.PrintFailedValidationToMessageLog"),
		bPrintFailedValidationToMessageLog,
		TEXT("Prints failed validation to message log instead of output log."),
		ECVF_Default);
#endif
}


void USimpleAnimInstance::NativeInitializeAnimation()
{
	Owner = TryGetPawnOwner();
	OwnerInterface = Owner ? Cast<ISimpleAnimInterface>(Owner) : nullptr;

	if (OwnerInterface)
	{
		if (FSimpleLandedSignature* LandedDelegatePtr = OwnerInterface->GetSimpleOnLandedDelegate())
		{
			if (LandedDelegatePtr->IsBoundToObject(this))
			{
				LandedDelegatePtr->Unbind();
			}
			LandedDelegatePtr->BindDynamic(this, &ThisClass::OnLanded);
		}
	}
	
	bFirstUpdate = true;
}

void USimpleAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
#if WITH_EDITOR
	// Interfaces are not UPROPERTY and edge cases see it getting garbage collected (eg. live coding)
	if (IsValid(Owner) && !OwnerInterface)
	{
		OwnerInterface = Owner ? Cast<ISimpleAnimInterface>(Owner) : nullptr;
	}
#endif
	
	if (!IsAnimValidToUpdate(DeltaTime))
	{
		return;
	}

	bWasMovingLastUpdate = !Local2D.Velocity.IsZero();

	LocalRole = OwnerInterface->GetSimpleLocalRole();

	World.Velocity = OwnerInterface->GetSimpleVelocity();
	World.Acceleration = OwnerInterface->GetSimpleAcceleration();
	ForwardVector = Owner->GetActorForwardVector();
	RightVector = Owner->GetActorRightVector();

	PrevWorldRotation = WorldRotation;
	WorldRotation = Owner->GetActorRotation();
	ControlRotation = OwnerInterface->GetSimpleControlRotation();
	BaseAimRotation = OwnerInterface->GetSimpleBaseAimRotation();
	
	MaxSpeed = OwnerInterface->GetSimpleMaxSpeed();
	LeanRate = OwnerInterface->GetSimpleLeanRate();
	
	bIsMovingOnGround = OwnerInterface->GetSimpleIsMovingOnGround();
	bInAir = OwnerInterface->GetSimpleIsFalling();
	bCanJump = OwnerInterface->GetSimpleCanJump();
	GravityZ = OwnerInterface->GetSimpleGravityZ();
	bMovementIs3D = OwnerInterface->GetSimpleMovementIs3D();

	bIsCrouching = OwnerInterface->GetSimpleIsCrouching();
	bIsProning = OwnerInterface->GetSimpleIsProning();

	bIsWalking = OwnerInterface->GetSimpleIsWalking();
	bIsSprinting = OwnerInterface->GetSimpleIsSprinting();

	bWantsLandingFrameLock = OwnerInterface->WantsFrameLockOnLanding();
	bIsMoveModeValid = OwnerInterface->GetSimpleIsMoveModeValid();
}

void USimpleAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaTime)
{
	if (!IsAnimValidToUpdate(DeltaTime))
	{
		return;
	}

	// Movement properties
	Local = World.GetLocal(WorldRotation);
	Local2D = Local.Get2D();
	World2D = World.Get2D();

	const FSimpleMovement& MoveBasis = bMovementIs3D ? Local : Local2D;
	bHasVelocity = !FMath::IsNearlyZero(MoveBasis.Velocity.SizeSquared());
	bHasAcceleration = !FMath::IsNearlyZero(MoveBasis.Acceleration.SizeSquared());

	// Rotation properties
	if (LocalRole != ROLE_SimulatedProxy)
	{
		// Control Rotation is more accurate, but BaseAimRotation is replicated to sim proxies
		BaseAimRotation = ControlRotation;
	}

	// Extension point
	NativeThreadSafePostUpdateMovementProperties(DeltaTime);

	if (bFirstUpdate)
	{
		// There is no valid delta on the first frame
		LeanAngle = 0.f;
	}
	else
	{
		const float YawDelta = WorldRotation.Yaw - PrevWorldRotation.Yaw;
		const float YawDeltaSpeed = YawDelta / DeltaTime;
		const float ScaledLeanRate = LeanRate / 100.f;  // 3.75 is a friendlier number than 0.0375 for designers
		LeanAngle = YawDeltaSpeed * ScaledLeanRate;
	}

	// Gait
	const ESimpleGaitMode PrevGait = Gait;
	Gait = ESimpleGaitMode::Jog;
	if (bIsSprinting)
	{
		Gait = ESimpleGaitMode::Sprint;
	}
	else if (bIsWalking)
	{
		Gait = ESimpleGaitMode::Walk;
	}
	bGaitChanged = Gait != PrevGait;

	// Stance
	const ESimpleStanceMode PrevStance = Stance;
	Stance = ESimpleStanceMode::Stand;
	if (bIsProning)
	{
		Stance = ESimpleStanceMode::Prone;
	}
	else if (bIsCrouching)
	{
		Stance = ESimpleStanceMode::Crouch;
	}
	bStanceChanged = Stance != PrevStance;

	// Extension point
	NativeThreadSafePreUpdateInAirProperties(DeltaTime);
	
	// Landing Frame Lock
	if (bWantsLandingFrameLock && bIsMoveModeValid)
	{
		bLandingFrameLock = true;
	}
	
	if (bLandingFrameLock && bIsMovingOnGround && bCanJump)
	{
		// OnLanded doesn't always trigger after mantling
		bLandingFrameLock = false;
	}

	// Jumping and falling
	bIsJumping = false;
	bIsFalling = false;
	if (bIsFalling && !bLandingFrameLock)
	{
		if (World.Velocity.Z > 0.f)
		{
			bIsJumping = true;
		}
		else
		{
			bIsFalling = true;
		}
	}

	TimeToJumpApex = bIsJumping ? -World.Velocity.Z / GravityZ : 0.f;

	// Extension point
	NativeThreadSafeUpdateAnimationPreCompletion(DeltaTime);

	bFirstUpdate = false;
}

void USimpleAnimInstance::OnLanded(const FHitResult& Hit)
{
	bLandingFrameLock = true;
}

bool USimpleAnimInstance::IsAnimValidToUpdate(float DeltaTime) const
{
	const bool bValidDeltaTime = DeltaTime > 1e-6f;
	const bool bValid = IsValid(Owner) && OwnerInterface && bValidDeltaTime;

#if WITH_EDITOR
	if (!bValid && bValidDeltaTime && GetWorld() && GetWorld()->IsGameWorld())
	{
		FString LogString;
		if (!IsValid(Owner))
		{
			LogString += "Owner is not valid";
		}
		if (!OwnerInterface)
		{
			if (!LogString.IsEmpty()) { LogString += ". "; }
			LogString += "No SimpleAnimInterface found.";
		}
		if (!LogString.IsEmpty())
		{
			LogString = "USimpleAnimInstance: " + LogString;
			if (IsInGameThread() && SimpleAnimInstanceCVars::bPrintFailedValidationToMessageLog)
			{
				FMessageLog Log { "PIE" };
				Log.Error(FText::FromString(LogString));
			}
			else
			{
				UE_LOG(LogSimpleAnim, Error, TEXT("%s"), *LogString);
			}
		}
	}
#endif
	
	return bValid;
}

void USimpleAnimInstance::K2_IsAnimValidToUpdate(float DeltaTime, ESimpleIsValidResult& ResultIsValid)
{
	const bool bValid = IsAnimValidToUpdate(DeltaTime);
	ResultIsValid = bValid ? ESimpleIsValidResult::Valid : ESimpleIsValidResult::NotValid;
}