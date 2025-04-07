// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleAnimInstance.h"

#include "SimpleAnimComponent.h"
#include "SimpleAnimInstanceProxy.h"
#include "SimpleLocomotionStatics.h"
#include "SimpleGameplayTags.h"
#include "GameFramework/Pawn.h"

#include "Logging/MessageLog.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SimpleAnimInstance)

// DEFINE_LOG_CATEGORY_STATIC(LogSimpleAnim, Log, All);

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
	Gait = FSimpleGameplayTags::Simple_Gait_Run;
	GaitSpeed = FSimpleGameplayTags::Simple_Gait_Run;
	StartGait = FSimpleGameplayTags::Simple_Gait_Run;
	StopGait = FSimpleGameplayTags::Simple_Gait_Run;
	State = FSimpleGameplayTags::Simple_State_Default;
	Stance = FSimpleGameplayTags::Simple_Stance_Stand;
}

FAnimInstanceProxy* USimpleAnimInstance::CreateAnimInstanceProxy()
{
	return new FSimpleAnimInstanceProxy(this);
}

void USimpleAnimInstance::NativeInitializeAnimation()
{
	Owner = GetOwningActor();
	PawnOwner = TryGetPawnOwner();
	OwnerComponent = Owner ? Owner->GetComponentByClass<USimpleAnimComponent>() : nullptr;

	if (!Owner || !OwnerComponent)
	{
		return;
	}

	// Bind landed delegate
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

	// Bind cardinal update delegates
	for (auto& CardinalItr : Cardinals.GetCardinals())
	{
		FSimpleCardinal& Cardinal = CardinalItr.Value;
		if (Cardinal.bEnabled && !Cardinal.UpdateDelegate.IsBoundToObject(this))
		{
			Cardinal.UpdateDelegate.BindUObject(this, &ThisClass::UpdateCardinal);
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

	State = OwnerComponent->GetSimpleAnimState();

	World.Velocity = OwnerComponent->GetSimpleVelocity();
	World.Acceleration = OwnerComponent->GetSimpleAcceleration();
	ForwardVector = Owner->GetActorForwardVector();
	RightVector = Owner->GetActorRightVector();

	PrevWorldRotation = WorldRotation;
	WorldRotation = Owner->GetActorRotation();
	ControlRotation = OwnerComponent->GetSimpleControlRotation();
	BaseAimRotation = OwnerComponent->GetSimpleBaseAimRotation();

	PrevMaxSpeed = MaxSpeed;
	MaxSpeed = OwnerComponent->GetSimpleMaxSpeed();
	MaxGaitSpeeds = OwnerComponent->GetSimpleMaxGaitSpeeds();
	LeanRate = LeanRateOverride >= 0.f ? LeanRateOverride : OwnerComponent->GetSimpleLeanRate();
	StartLeanRate = StartLeanRateOverride >= 0.f ? StartLeanRateOverride : OwnerComponent->GetSimpleStartLeanRate();

	RootYawOffset = OwnerComponent->GetSimpleRootYawOffset();

	bIsCurrentFloorWalkable = OwnerComponent->IsSimpleCurrentFloorWalkable();
	bIsMovingOnGround = OwnerComponent->GetSimpleIsMovingOnGround() && bIsCurrentFloorWalkable;
	bInAir = OwnerComponent->GetSimpleIsFalling() || !bIsCurrentFloorWalkable;
	bCanJump = OwnerComponent->GetSimpleCanJump();
	GravityZ = OwnerComponent->GetSimpleGravityZ();
	bMovementIs3D = OwnerComponent->GetSimpleMovementIs3D();

	bIsCrouched = OwnerComponent->GetSimpleIsCrouched();
	bIsProned = OwnerComponent->GetSimpleIsProned();

	bIsStrolling = OwnerComponent->GetSimpleIsStrolling();
	bIsWalking = OwnerComponent->GetSimpleIsWalking();
	bIsSprinting = OwnerComponent->GetSimpleIsSprinting();

	bWantsStrolling = OwnerComponent->GetSimpleWantsStrolling();
	bWantsWalking = OwnerComponent->GetSimpleWantsWalking();
	bWantsSprinting = OwnerComponent->GetSimpleWantsSprinting();

	bWantsLandingFrameLock = OwnerComponent->WantsFrameLockOnLanding();
	bIsMoveModeValid = OwnerComponent->GetSimpleIsMoveModeValid();
	
	bIsAnyMontagePlaying = IsAnyMontagePlaying();
	bIsPlayingNetworkedRootMotionMontage = OwnerComponent->IsPlayingNetworkedRootMotionMontage();
}

void USimpleAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaTime)
{
	if (!IsAnimValidToUpdate(DeltaTime))
	{
		return;
	}

	// Extension point
	NativeThreadSafePreUpdateMovementProperties(DeltaTime);

	// Movement properties
	Local = World.GetLocal(WorldRotation);
	Local2D = Local.Get2D();
	World2D = World.Get2D();

	PrevSpeed = Speed;
	Speed3D = Local.Velocity.Size();
	Speed2D = Local2D.Velocity.Size();
	Speed = bIsMovingOnGround ? Speed3D : Speed2D;
	const float SpeedSq = Speed * Speed;
	const float Speed2DSq = Speed2D * Speed2D;

	const float AccelMag3D = Local.Acceleration.SizeSquared();
	const float AccelMag2D = Local2D.Acceleration.SizeSquared();
	const float AccelSq = bMovementIs3D ? AccelMag3D : AccelMag2D;
	
	bHasVelocity = !FMath::IsNearlyZero(SpeedSq);
	bHasVelocity2D = !FMath::IsNearlyZero(Speed2DSq);
	bHasAcceleration = !FMath::IsNearlyZero(AccelSq);
	bHasAcceleration2D = !FMath::IsNearlyZero(AccelMag2D);

	// Rotation properties
	if (LocalRole != ROLE_SimulatedProxy)
	{
		// Control Rotation is more accurate, but BaseAimRotation is replicated to sim proxies
		BaseAimRotation = ControlRotation;
	}

	// Update cardinal properties
	// if (bWantsCardinalsUpdated)
	{
		Cardinals.ThreadSafeUpdate(World2D, WorldRotation, RootYawOffset);
	}

	// Update gait modes
	NativeThreadSafeUpdateGaitMode(DeltaTime);

	// Update stance
	NativeThreadSafeUpdateStance(DeltaTime);

	// Extension point
	NativeThreadSafePostUpdateMovementProperties(DeltaTime);

	// Lean angles
	ThreadSafeUpdateLeanAngles(DeltaTime);

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

void USimpleAnimInstance::ThreadSafeUpdateLeanAngles(float DeltaTime)
{
	if (bFirstUpdate)
	{
		// There is no valid delta on the first frame
		LeanAngle = 0.f;
		StartLeanAngle = 0.f;
	}
	else if (bWantsLeansUpdated)
	{
		// Compute lean delta
		const float YawDelta = WorldRotation.Yaw - PrevWorldRotation.Yaw;
		const float YawDeltaSpeed = YawDelta / DeltaTime;

		// 3.75 is a friendlier number than 0.0375 for designers so we divide by 100
		const float ScaledLeanRate = LeanRate / 100.f;  
		const float ScaledStartLeanRate = StartLeanRate / 100.f;

		// Apply lean rate
		LeanAngle = YawDeltaSpeed * ScaledLeanRate;
		StartLeanAngle = YawDeltaSpeed * ScaledStartLeanRate;
	}
	else
	{
		LeanAngle = 0.f;
		StartLeanAngle = 0.f;
	}
}

void USimpleAnimInstance::NativeThreadSafeUpdateGaitMode(float DeltaTime)
{
	// Start Gait Mode: Use the intended mode
	StartGait = FSimpleGameplayTags::Simple_Gait_Run;
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
	else if (bWantsStrolling)
	{
		// We will probably start strolling next frame
		StartGait = FSimpleGameplayTags::Simple_Gait_Stroll;
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
	else if (bIsStrolling)
	{
		Gait = FSimpleGameplayTags::Simple_Gait_Stroll;
	}
	else
	{
		Gait = FSimpleGameplayTags::Simple_Gait_Run;
	}
	bGaitChanged = Gait != PrevGait;

	// const float MaxSpeedStroll = MaxGaitSpeeds.GetMaxSpeed(FSimpleGameplayTags::Simple_Gait_Stroll);
	const float MaxSpeedWalk = MaxGaitSpeeds.GetMaxSpeed(FSimpleGameplayTags::Simple_Gait_Walk);
	const float MaxSpeedRun = MaxGaitSpeeds.GetMaxSpeed(FSimpleGameplayTags::Simple_Gait_Run);
	const float MaxSpeedSprint = MaxGaitSpeeds.GetMaxSpeed(FSimpleGameplayTags::Simple_Gait_Sprint);
	
	// Gait Mode at Speed: Use the current mode based on speed
	GaitSpeed = Gait;
	if (Speed < MaxSpeedWalk)
	{
		GaitSpeed = FSimpleGameplayTags::Simple_Gait_Stroll;
	}
	else if (Speed < MaxSpeedRun)
	{
		GaitSpeed = FSimpleGameplayTags::Simple_Gait_Walk;
	}
	else if (Speed < MaxSpeedSprint)
	{
		GaitSpeed = FSimpleGameplayTags::Simple_Gait_Run;
	}
	else
	{
		GaitSpeed = FSimpleGameplayTags::Simple_Gait_Sprint;
	}

	// Stop Gait Mode: Use the previous mode
	if (bHasAcceleration)
	{
		StopGait = GaitSpeed;  // We stop based on the speed we are at rather than intent
	}
}

void USimpleAnimInstance::NativeThreadSafeUpdateStance(float DeltaTime)
{
	// Stance
	const FGameplayTag PrevStance = Stance;
	if (bIsProned)
	{
		Stance = FSimpleGameplayTags::Simple_Stance_Prone;
	}
	else if (bIsCrouched)
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

float USimpleAnimInstance::GetCardinalDeadZone(const FGameplayTag& CardinalMode) const
{
	return 10.f;
}

void USimpleAnimInstance::UpdateCardinal(const FGameplayTag& CardinalMode, FSimpleCardinal& Cardinal, const FSimpleCardinals& InCardinals)
{
	// Consider not updating the properties you don't need to optimize performance!
	const float DeadZone = GetCardinalDeadZone(CardinalMode);

	Cardinal.Acceleration = USimpleLocomotionStatics::SelectSimpleCardinalFromAngle(
		CardinalMode, InCardinals.Acceleration, DeadZone, Cardinal.Acceleration, bWasMovingLastUpdate);
	
	Cardinal.Velocity = USimpleLocomotionStatics::SelectSimpleCardinalFromAngle(
		CardinalMode, InCardinals.Velocity, DeadZone, Cardinal.Velocity, bWasMovingLastUpdate);
}

float USimpleAnimInstance::GetLocomotionCardinalAngle(ESimpleCardinalType CardinalType) const
{
	return Cardinals.GetDirectionAngle(CardinalType);
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

#if WITH_EDITOR
	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
		return false;
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