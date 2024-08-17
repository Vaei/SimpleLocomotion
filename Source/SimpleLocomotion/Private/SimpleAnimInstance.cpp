// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleAnimInstance.h"

#include "SimpleAnimComponent.h"
#include "SimpleAnimInstanceProxy.h"
#include "SimpleLocomotionTags.h"

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
	
	static bool bPrintInvalidGameplayTagStates = true;
	FAutoConsoleVariableRef CVarPrintInvalidGameplayTagStates(
		TEXT("a.SimpleAnim.PrintInvalidGameplayTagStates"),
		bPrintInvalidGameplayTagStates,
		TEXT("Prints invalid gameplay tag states to output log."),
		ECVF_Default);
}


USimpleAnimInstance::USimpleAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Gait = FSimpleGameplayTags::Simple_Gait_Jog;
	StartGait = FSimpleGameplayTags::Simple_Gait_Jog;
	StopGait = FSimpleGameplayTags::Simple_Gait_Jog;
	Stance = FSimpleGameplayTags::Simple_Stance_Stand;
}

FAnimInstanceProxy* USimpleAnimInstance::CreateAnimInstanceProxy()
{
	return new FSimpleAnimInstanceProxy(this);
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
	LeanRate = LeanRateOverride >= 0.f ? LeanRateOverride : OwnerComponent->GetSimpleLeanRate();

	RootYawOffset = OwnerComponent->GetSimpleRootYawOffset();

	bIsCurrentFloorWalkable = OwnerComponent->IsSimpleCurrentFloorWalkable();
	bIsMovingOnGround = OwnerComponent->GetSimpleIsMovingOnGround() && bIsCurrentFloorWalkable;
	bInAir = OwnerComponent->GetSimpleIsFalling() || !bIsCurrentFloorWalkable;
	bCanJump = OwnerComponent->GetSimpleCanJump();
	GravityZ = OwnerComponent->GetSimpleGravityZ();
	bMovementIs3D = OwnerComponent->GetSimpleMovementIs3D();

	bIsCrouching = OwnerComponent->GetSimpleIsCrouching();

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

	PrevSpeed = Speed;
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

	// Update stance
	NativeThreadSafeUpdateStance(DeltaTime);

	// Extension point
	NativeThreadSafePostUpdateMovementProperties(DeltaTime);

	if (bFirstUpdate)
	{
		// There is no valid delta on the first frame
		LeanAngle = 0.f;
	}
	else if (bWantsLeansUpdated)
	{
		const float YawDelta = WorldRotation.Yaw - PrevWorldRotation.Yaw;
		const float YawDeltaSpeed = YawDelta / DeltaTime;
		const float ScaledLeanRate = LeanRate / 100.f;  // 3.75 is a friendlier number than 0.0375 for designers
		LeanAngle = YawDeltaSpeed * ScaledLeanRate;
	}
	else
	{
		LeanAngle = 0.f;
	}

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
	NativeThreadSafeUpdateFalling(DeltaTime);

	// Extension point
	NativeThreadSafeUpdateAnimationPreCompletion(DeltaTime);

	bFirstUpdate = false;
}

void USimpleAnimInstance::NativeThreadSafeUpdateGaitMode(float DeltaTime)
{
	// Start Gait Mode: Use the intended mode
	StartGait = FSimpleGameplayTags::Simple_Gait_Jog;
	if (bWantsSprinting)
	{
		// We will probably start sprinting next frame
		StartGait = FSimpleGameplayTags::Simple_Gait_Sprint;
	}
	else if (bWantsWalking)
	{
		// We will probably start walking next frame
		StartGait = FSimpleGameplayTags::Simple_Gait_Walk;
	}

	// Gait Mode: Use the current mode
	const FGameplayTag PrevGait = Gait;
	if (bIsSprinting)
	{
		Gait = FSimpleGameplayTags::Simple_Gait_Sprint;
	}
	else if (bIsWalking)
	{
		Gait = FSimpleGameplayTags::Simple_Gait_Walk;
	}
	else
	{
		Gait = FSimpleGameplayTags::Simple_Gait_Jog;
	}
	bGaitChanged = Gait != PrevGait;

	// Stop Gait Mode: Use the previous mode
	if (bHasAcceleration)
	{
		StopGait = Gait;
		if (Gait == FSimpleGameplayTags::Simple_Gait_Walk)
		{
			// If walking and needing to stop, default to walk stop, don't need to do anything here
		}
		else if (Gait == FSimpleGameplayTags::Simple_Gait_Jog)
		{
			// If not at run speed, use walking stop
			if (Speed < MaxGaitSpeeds.WalkSpeed)
			{
				// Use walk speed if we haven't even reached our max walk speed yet
				StopGait = FSimpleGameplayTags::Simple_Gait_Walk;
			}
			else if (Speed < MaxGaitSpeeds.JogSpeed)
			{
				// Use the gait mode that our speed is closer to
				if ((Speed - MaxGaitSpeeds.WalkSpeed) < (MaxGaitSpeeds.JogSpeed - Speed))
				{
					StopGait = FSimpleGameplayTags::Simple_Gait_Walk;
				}
				else
				{
					StopGait = FSimpleGameplayTags::Simple_Gait_Jog;
				}
			}
		}
		else if (Gait == FSimpleGameplayTags::Simple_Gait_Sprint)
		{
			if (Speed < MaxGaitSpeeds.JogSpeed)
			{
				// Use the gait mode that our speed is closer to if we haven't even reached jog speed
				if ((Speed - MaxGaitSpeeds.WalkSpeed) < (MaxGaitSpeeds.JogSpeed - Speed))
				{
					StopGait = FSimpleGameplayTags::Simple_Gait_Walk;
				}
				else
				{
					StopGait = FSimpleGameplayTags::Simple_Gait_Jog;
				}
			}
			else
			{
				// Sprint always results in a sprint stop, provided we have exceeded jog speed
				StopGait = FSimpleGameplayTags::Simple_Gait_Sprint;
			}
		}
		else if (SimpleAnimInstanceCVars::bPrintInvalidGameplayTagStates)
		{
			FSimpleAnimInstanceProxy& Proxy = GetProxyOnAnyThread<FSimpleAnimInstanceProxy>();
			Proxy.AddPendingMessage(FString::Printf(TEXT("Invalid Gait Mode: %s"), *Gait.ToString()));
		}
	}
}

void USimpleAnimInstance::NativeThreadSafeUpdateStance(float DeltaTime)
{
	// Stance
	const FGameplayTag PrevStance = Stance;
	if (bIsCrouching)
	{
		Stance = FSimpleGameplayTags::Simple_Stance_Crouch;
	}
	else
	{
		Stance = FSimpleGameplayTags::Simple_Stance_Stand;
	}
	bStanceChanged = Stance != PrevStance;
}

void USimpleAnimInstance::NativeThreadSafeUpdateFalling(float DeltaTime)
{
	// Jumping and falling
	bIsJumping = false;
	bIsFalling = false;
	if (bInAir && !bLandingFrameLock)
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
}

void USimpleAnimInstance::NativePostEvaluateAnimation()
{
	FSimpleAnimInstanceProxy& Proxy = GetProxyOnAnyThread<FSimpleAnimInstanceProxy>();
	for (auto& PendingLogs : Proxy.GetPendingMessageLogs())
	{
		FMessageLog MsgLog { *PendingLogs.Key };
		MsgLog.Error(FText::FromString(PendingLogs.Value));
	}

	Proxy.ResetPendingMessageLogs();
}

void USimpleAnimInstance::OnLanded(const FHitResult& Hit)
{
	bLandingFrameLock = true;
}

float USimpleAnimInstance::GetLocomotionCardinalAngle(ESimpleCardinalType CardinalType) const
{
	return CardinalMovement.GetDirectionAngle(CardinalType);
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
			FSimpleAnimInstanceProxy& Proxy = const_cast<USimpleAnimInstance*>(this)->GetProxyOnAnyThread<FSimpleAnimInstanceProxy>();
			Proxy.AddPendingMessage(LogError);
		}
	}
#endif
}