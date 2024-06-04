// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleAnimInstance.h"

#include "SimpleAnimComponent.h"

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
	OwnerComponent = Owner ? Owner->GetComponentByClass<USimpleAnimComponent>() : nullptr;

	if (OwnerComponent)
	{
		if (FSimpleLandedSignature* LandedDelegatePtr = OwnerComponent->GetSimpleOnLandedDelegate())
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
	if (!IsAnimValidToUpdate(DeltaTime))
	{
		return;
	}

	bWasMovingLastUpdate = !Local2D.Velocity.IsZero();

	LocalRole = OwnerComponent->GetSimpleLocalRole();

	World.Velocity = OwnerComponent->GetSimpleVelocity();
	World.Acceleration = OwnerComponent->GetSimpleAcceleration();
	ForwardVector = Owner->GetActorForwardVector();
	RightVector = Owner->GetActorRightVector();

	PrevWorldRotation = WorldRotation;
	WorldRotation = Owner->GetActorRotation();
	ControlRotation = OwnerComponent->GetSimpleControlRotation();
	BaseAimRotation = OwnerComponent->GetSimpleBaseAimRotation();
	
	MaxSpeed = OwnerComponent->GetSimpleMaxSpeed();
	MaxGaitSpeeds = OwnerComponent->GetSimpleMaxGaitSpeeds();
	LeanRate = OwnerComponent->GetSimpleLeanRate();

	RootYawOffset = OwnerComponent->GetSimpleRootYawOffset();
	
	bIsMovingOnGround = OwnerComponent->GetSimpleIsMovingOnGround();
	bInAir = OwnerComponent->GetSimpleIsFalling();
	bCanJump = OwnerComponent->GetSimpleCanJump();
	GravityZ = OwnerComponent->GetSimpleGravityZ();
	bMovementIs3D = OwnerComponent->GetSimpleMovementIs3D();

	bIsCrouching = OwnerComponent->GetSimpleIsCrouching();
	bIsProning = OwnerComponent->GetSimpleIsProning();

	bIsWalking = OwnerComponent->GetSimpleIsWalking();
	bIsSprinting = OwnerComponent->GetSimpleIsSprinting();

	bWantsWalking = OwnerComponent->GetSimpleWantsWalking();
	bWantsSprinting = OwnerComponent->GetSimpleWantsSprinting();

	bWantsLandingFrameLock = OwnerComponent->WantsFrameLockOnLanding();
	bIsMoveModeValid = OwnerComponent->GetSimpleIsMoveModeValid();
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

	Speed3D = Local.Velocity.Size();
	Speed2D = Local2D.Velocity.Size();
	Speed = bIsMovingOnGround ? Speed3D : Speed2D;
	const float SpeedSq = Speed * Speed;

	const float AccelMag3D = Local.Acceleration.SizeSquared();
	const float AccelMag2D = Local2D.Acceleration.SizeSquared();
	const float AccelSq = bMovementIs3D ? AccelMag3D : AccelMag2D;
	
	bHasVelocity = !FMath::IsNearlyZero(SpeedSq);
	bHasAcceleration = !FMath::IsNearlyZero(AccelSq);

	// Rotation properties
	if (LocalRole != ROLE_SimulatedProxy)
	{
		// Control Rotation is more accurate, but BaseAimRotation is replicated to sim proxies
		BaseAimRotation = ControlRotation;
	}

	// Update cardinal properties
	if (bWantsCardinalsUpdated)
	{
		CardinalMovement.ThreadSafeUpdate(World2D, WorldRotation, RootYawOffset);
	}

	// Update gait modes
	NativeThreadSafeUpdateGaitMode(DeltaTime);

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

void USimpleAnimInstance::NativeThreadSafeUpdateGaitMode(float DeltaTime)
{
	// Start Gait Mode: Use the intended mode
	StartGait = ESimpleGaitMode::Jog;
	if (bWantsSprinting)
	{
		// We will probably start sprinting next frame
		StartGait = ESimpleGaitMode::Sprint;
	}
	else if (bWantsWalking)
	{
		// We will probably start walking next frame
		StartGait = ESimpleGaitMode::Walk;
	}
	
	// Gait Mode: Use the current mode
	if (bIsSprinting)
	{
		Gait = ESimpleGaitMode::Sprint;
	}
	else if (bIsWalking)
	{
		Gait = ESimpleGaitMode::Walk;
	}
	else
	{
		Gait = ESimpleGaitMode::Jog;
	}

	// Stop Gait Mode: Use the previous mode
	if (bHasAcceleration)
	{
		StopGait = Gait;
		switch(Gait)
		{
			case ESimpleGaitMode::Walk:
				// If walking and needing to stop, default to walk stop
				break;
			case ESimpleGaitMode::Jog:
				// If not at run speed, use walking stop
				if (Speed < MaxGaitSpeeds.WalkSpeed)
				{
					// Use walk speed if we haven't even reached our max walk speed yet
					StopGait = ESimpleGaitMode::Walk;
				}
				else if (Speed < MaxGaitSpeeds.JogSpeed)
				{
					// Use the gait mode that our speed is closer to
					if ((Speed - MaxGaitSpeeds.WalkSpeed) < (MaxGaitSpeeds.JogSpeed - Speed))
					{
						StopGait = ESimpleGaitMode::Walk;
					}
					else
					{
						StopGait = ESimpleGaitMode::Jog;
					}
				}
			break;
			case ESimpleGaitMode::Sprint:
				if (Speed < MaxGaitSpeeds.JogSpeed)
				{
					// Use the gait mode that our speed is closer to if we haven't even reached jog speed
					if ((Speed - MaxGaitSpeeds.WalkSpeed) < (MaxGaitSpeeds.JogSpeed - Speed))
					{
						StopGait = ESimpleGaitMode::Walk;
					}
					else
					{
						StopGait = ESimpleGaitMode::Jog;
					}
				}
				else
				{
					// Sprint always results in a sprint stop, provided we have exceeded jog speed
					StopGait = ESimpleGaitMode::Sprint;
				}
			break;
		}
	}
}

void USimpleAnimInstance::OnLanded(const FHitResult& Hit)
{
	bLandingFrameLock = true;
}

bool USimpleAnimInstance::IsAnimValidToUpdate(float DeltaTime) const
{
	const bool bValid = IsValid(Owner) && OwnerComponent;
	const bool bValidDeltaTime = DeltaTime > 1e-6f;

#if WITH_EDITORONLY_DATA
	if (!bValid && !HasAnyFlags(RF_DefaultSubObject) && GetWorld() && GetWorld()->IsGameWorld())
	{
		FString LogError;
		if (!IsValid(Owner))
		{
			LogError += "Owner is not valid";
		}
		if (!OwnerComponent)
		{
			if (!LogError.IsEmpty()) { LogError += ". "; }
			LogError += "No SimpleAnimComponent found.";
		}
		if (!LogError.IsEmpty())
		{
			OnAnimNotValidToUpdate(LogError);
		}
	}
#else
	if (!bValid)
	{
		OnAnimNotValidToUpdate("");
	}
#endif
	
	return bValid && bValidDeltaTime;
}

void USimpleAnimInstance::K2_IsAnimValidToUpdate(float DeltaTime, ESimpleIsValidResult& ResultIsValid)
{
	const bool bValid = IsAnimValidToUpdate(DeltaTime);
	ResultIsValid = bValid ? ESimpleIsValidResult::Valid : ESimpleIsValidResult::NotValid;
}

void USimpleAnimInstance::OnAnimNotValidToUpdate(FString LogError) const
{
#if WITH_EDITORONLY_DATA
	if (!ConsumedDebugMessages.Contains(LogError))  // Don't spam the message log
	{
		const_cast<USimpleAnimInstance*>(this)->ConsumedDebugMessages.Add(LogError);
		LogError = "USimpleAnimInstance: " + LogError;
		if (IsInGameThread() && SimpleAnimInstanceCVars::bPrintFailedValidationToMessageLog)
		{
			FMessageLog Log { "PIE" };
			Log.Error(FText::FromString(LogError));
		}
		else
		{
			UE_LOG(LogSimpleAnim, Error, TEXT("%s"), *LogError);
		}
	}
#endif
}