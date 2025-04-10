// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleLocomotionSets.h"

#include "SimpleGameplayTags.h"
#include "SimpleLocomotionTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SimpleLocomotionSets)


FSimpleStrafeLocomotionSet::FSimpleStrafeLocomotionSet()
	: Mode(FSimpleGameplayTags::Simple_Mode_Strafe_4Way)
	, InertialBlendTime(0.2f)
	, Forward(nullptr)
	, ForwardLeft(nullptr)
	, ForwardRight(nullptr)
	, Left(nullptr)
	, Right(nullptr)
	, LeftAway(nullptr)
	, RightAway(nullptr)
	, Backward(nullptr)
	, BackwardLeft(nullptr)
	, BackwardRight(nullptr)
	, CardinalType(ESimpleCardinalType::Velocity)
{}

UAnimSequence* FSimpleStrafeLocomotionSet::GetAnimation(const FGameplayTag& CardinalTag) const
{
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Forward)			{ return Forward; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Forward_Left)	{ return ForwardLeft; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Forward_Right)	{ return ForwardRight; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Left)			{ return Left; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Right)			{ return Right; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Left_Away)		{ return LeftAway; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Right_Away)		{ return RightAway; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Backward)		{ return Backward; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Backward_Left)	{ return BackwardLeft; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Backward_Right)	{ return BackwardRight; }
	ensure(false);  // Did you enable the cardinals you are trying to use?
	return nullptr;
}

FSimpleStartLocomotionSet::FSimpleStartLocomotionSet()
	: Mode(FSimpleGameplayTags::Simple_Mode_Start_1Way)
	, Forward(nullptr)
	, ForwardLeft(nullptr)
	, ForwardRight(nullptr)
	, Left(nullptr)
	, Right(nullptr)
	, BackwardTurnLeft(nullptr)
	, BackwardTurnRight(nullptr)
	, BackwardLeft(nullptr)
	, BackwardRight(nullptr)
	, CardinalType(ESimpleCardinalType::Acceleration)
{}

UAnimSequence* FSimpleStartLocomotionSet::GetAnimation(const FGameplayTag& CardinalTag) const
{
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Forward)				{ return Forward; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Forward_Left)		{ return ForwardLeft; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Forward_Right)		{ return ForwardRight; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Left)				{ return Left; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Right)				{ return Right; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_BackwardTurnLeft)	{ return BackwardTurnLeft; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_BackwardTurnRight)	{ return BackwardTurnRight; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Backward_Left)		{ return BackwardLeft; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Backward_Right)		{ return BackwardRight; }
	ensure(false);  // Did you enable the cardinals you are trying to use?
	return nullptr;
}

FSimpleTurnLocomotionSet::FSimpleTurnLocomotionSet()
	: Mode(FSimpleGameplayTags::Simple_Mode_Turn_1Way)
	, AngleTolerance(30.f)
	, ForwardLeft(nullptr)
	, ForwardRight(nullptr)
	, Left(nullptr)
	, Right(nullptr)
	, BackwardTurnLeft(nullptr)
	, BackwardTurnRight(nullptr)
	, BackwardLeft(nullptr)
	, BackwardRight(nullptr)
	, CardinalType(ESimpleCardinalType::Acceleration)
{}

UAnimSequence* FSimpleTurnLocomotionSet::GetAnimation(const FGameplayTag& CardinalTag) const
{
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Forward_Left)		{ return ForwardLeft; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Forward_Right)		{ return ForwardRight; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Left)				{ return Left; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Right)				{ return Right; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_BackwardTurnLeft)	{ return BackwardTurnLeft; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_BackwardTurnRight)	{ return BackwardTurnRight; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Backward_Left)		{ return BackwardLeft; }
	if (CardinalTag == FSimpleGameplayTags::Simple_Cardinal_Backward_Right)		{ return BackwardRight; }
	ensure(false);  // Did you enable the cardinals you are trying to use?
	return nullptr;
}

FSimpleStrafeGaitSet::FSimpleStrafeGaitSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_Gait_Run);

	// Strolling should fall back to walk if unavailable, otherwise run, otherwise sprint
	FSimpleGameplayTagArray& StrollFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Gait_Stroll);
	StrollFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Walk);
	StrollFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Run);
	StrollFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Sprint);

	// Walking should fall back to run if unavailable, otherwise stroll, otherwise sprint
	FSimpleGameplayTagArray& WalkFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Gait_Walk);
	WalkFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Run);
	WalkFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Stroll);
	WalkFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Sprint);

	// Run should fall back to walk if unavailable, otherwise sprint, otherwise stroll
	FSimpleGameplayTagArray& RunFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Gait_Run);
	RunFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Walk);
	RunFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Sprint);
	RunFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Stroll);
	
	// Sprint should fall back to run if unavailable, otherwise walk, otherwise stroll
	FSimpleGameplayTagArray& SprintFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Gait_Sprint);
	SprintFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Run);
	SprintFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Walk);
	SprintFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Stroll);
}

void FSimpleStrafeGaitSet::SetCardinalType(ESimpleCardinalType CardinalType)
{
	for (auto& GaitItr : Sets)
	{
		FSimpleStrafeLocomotionSet& LocoSet = GaitItr.Value;
		LocoSet.CardinalType = CardinalType;
	}
}

FSimpleStartGaitSet::FSimpleStartGaitSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_Gait_Run);

	// Strolling should fall back to walk if unavailable, otherwise run, otherwise sprint
	FSimpleGameplayTagArray& StrollFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Gait_Stroll);
	StrollFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Walk);
	StrollFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Run);
	StrollFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Sprint);

	// Walking should fall back to run if unavailable, otherwise stroll, otherwise sprint
	FSimpleGameplayTagArray& WalkFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Gait_Walk);
	WalkFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Run);
	WalkFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Stroll);
	WalkFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Sprint);

	// Run should fall back to walk if unavailable, otherwise sprint, otherwise stroll
	FSimpleGameplayTagArray& RunFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Gait_Run);
	RunFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Walk);
	RunFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Sprint);
	RunFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Stroll);
	
	// Sprint should fall back to run if unavailable, otherwise walk, otherwise stroll
	FSimpleGameplayTagArray& SprintFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Gait_Sprint);
	SprintFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Run);
	SprintFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Walk);
	SprintFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Stroll);
}

void FSimpleStartGaitSet::SetCardinalType(ESimpleCardinalType CardinalType)
{
	for (auto& GaitItr : Sets)
	{
		FSimpleStartLocomotionSet& LocoSet = GaitItr.Value;
		LocoSet.CardinalType = CardinalType;
	}
}

FSimpleTurnGaitSet::FSimpleTurnGaitSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_Gait_Run);

	// Strolling should fall back to walk if unavailable, otherwise run, otherwise sprint
	FSimpleGameplayTagArray& StrollFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Gait_Stroll);
	StrollFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Walk);
	StrollFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Run);
	StrollFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Sprint);

	// Walking should fall back to run if unavailable, otherwise stroll, otherwise sprint
	FSimpleGameplayTagArray& WalkFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Gait_Walk);
	WalkFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Run);
	WalkFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Stroll);
	WalkFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Sprint);

	// Run should fall back to walk if unavailable, otherwise sprint, otherwise stroll
	FSimpleGameplayTagArray& RunFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Gait_Run);
	RunFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Walk);
	RunFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Sprint);
	RunFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Stroll);
	
	// Sprint should fall back to run if unavailable, otherwise walk, otherwise stroll
	FSimpleGameplayTagArray& SprintFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Gait_Sprint);
	SprintFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Run);
	SprintFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Walk);
	SprintFallback.AddTagFast(FSimpleGameplayTags::Simple_Gait_Stroll);
}

void FSimpleTurnGaitSet::SetCardinalType(ESimpleCardinalType CardinalType)
{
	for (auto& GaitItr : Sets)
	{
		FSimpleTurnLocomotionSet& LocoSet = GaitItr.Value;
		LocoSet.CardinalType = CardinalType;
	}
}

FSimpleStanceSet::FSimpleStanceSet()
{
	Animations.Add(FSimpleGameplayTags::Simple_Stance_Stand);
	
	// Crouch falls back to standing
	FSimpleGameplayTagArray& CrouchFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Stance_Crouch);
	CrouchFallback.AddTagFast(FSimpleGameplayTags::Simple_Stance_Stand);

	// Prone falls back to crouch
	FSimpleGameplayTagArray& ProneFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Stance_Prone);
	ProneFallback.AddTagFast(FSimpleGameplayTags::Simple_Stance_Crouch);
}

FSimpleStanceToStrafeGaitSet::FSimpleStanceToStrafeGaitSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_Stance_Stand);

	// Crouch falls back to standing
	FSimpleGameplayTagArray& CrouchFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Stance_Crouch);
	CrouchFallback.AddTagFast(FSimpleGameplayTags::Simple_Stance_Stand);

	// Prone falls back to crouch
	FSimpleGameplayTagArray& ProneFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Stance_Prone);
	ProneFallback.AddTagFast(FSimpleGameplayTags::Simple_Stance_Crouch);
}

FSimpleStanceToStartGaitSet::FSimpleStanceToStartGaitSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_Stance_Stand);

	// Crouch falls back to standing
	FSimpleGameplayTagArray& CrouchFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Stance_Crouch);
	CrouchFallback.AddTagFast(FSimpleGameplayTags::Simple_Stance_Stand);

	// Prone falls back to crouch
	FSimpleGameplayTagArray& ProneFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Stance_Prone);
	ProneFallback.AddTagFast(FSimpleGameplayTags::Simple_Stance_Crouch);
}

FSimpleStanceToTurnGaitSet::FSimpleStanceToTurnGaitSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_Stance_Stand);

	// Crouch falls back to standing
	FSimpleGameplayTagArray& CrouchFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Stance_Crouch);
	CrouchFallback.AddTagFast(FSimpleGameplayTags::Simple_Stance_Stand);

	// Prone falls back to crouch
	FSimpleGameplayTagArray& ProneFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Stance_Prone);
	ProneFallback.AddTagFast(FSimpleGameplayTags::Simple_Stance_Crouch);
}

FSimpleStateToStrafeGaitSet::FSimpleStateToStrafeGaitSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_State_Default);
}

FSimpleStateToStartGaitSet::FSimpleStateToStartGaitSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_State_Default);
}

FSimpleStateToTurnGaitSet::FSimpleStateToTurnGaitSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_State_Default);
}

FSimpleStateToStanceToStrafeGaitSet::FSimpleStateToStanceToStrafeGaitSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_State_Default);
}

FSimpleStateToStanceToStartGaitSet::FSimpleStateToStanceToStartGaitSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_State_Default);
}

FSimpleStateToStanceToTurnGaitSet::FSimpleStateToStanceToTurnGaitSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_State_Default);
}

FSimpleStanceToTransitionSet::FSimpleStanceToTransitionSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_Stance_Stand);
	
	// Crouch falls back to standing
	FSimpleGameplayTagArray& CrouchFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Stance_Crouch);
	CrouchFallback.AddTagFast(FSimpleGameplayTags::Simple_Stance_Stand);

	// Prone falls back to crouch
	FSimpleGameplayTagArray& ProneFallback = Fallbacks.Add(FSimpleGameplayTags::Simple_Stance_Prone);
	ProneFallback.AddTagFast(FSimpleGameplayTags::Simple_Stance_Crouch);
}

FSimpleStateToStanceSet::FSimpleStateToStanceSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_State_Default);
}

FSimpleStateToStanceToTransitionSet::FSimpleStateToStanceToTransitionSet()
{
	Sets.Add(FSimpleGameplayTags::Simple_State_Default);
}
