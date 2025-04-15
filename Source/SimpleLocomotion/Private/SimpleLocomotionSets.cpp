// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleLocomotionSets.h"

#include "SimpleTags.h"
#include "SimpleLocomotionTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SimpleLocomotionSets)


FSimpleStrafeLocomotionSet::FSimpleStrafeLocomotionSet()
	: Mode(FSimpleTags::Simple_Mode_Strafe_4Way)
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
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Forward)			{ return Forward; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Forward_Left)	{ return ForwardLeft; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Forward_Right)	{ return ForwardRight; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Left)			{ return Left; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Right)			{ return Right; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Left_Away)		{ return LeftAway; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Right_Away)		{ return RightAway; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Backward)		{ return Backward; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Backward_Left)	{ return BackwardLeft; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Backward_Right)	{ return BackwardRight; }
	ensure(false);  // Did you enable the cardinals you are trying to use?
	return nullptr;
}

FSimpleStartLocomotionSet::FSimpleStartLocomotionSet()
	: Mode(FSimpleTags::Simple_Mode_Start_1Way)
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
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Forward)				{ return Forward; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Forward_Left)		{ return ForwardLeft; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Forward_Right)		{ return ForwardRight; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Left)				{ return Left; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Right)				{ return Right; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_BackwardTurnLeft)	{ return BackwardTurnLeft; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_BackwardTurnRight)	{ return BackwardTurnRight; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Backward_Left)		{ return BackwardLeft; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Backward_Right)		{ return BackwardRight; }
	ensure(false);  // Did you enable the cardinals you are trying to use?
	return nullptr;
}

FSimpleTurnLocomotionSet::FSimpleTurnLocomotionSet()
	: Mode(FSimpleTags::Simple_Mode_Turn_1Way)
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
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Forward_Left)		{ return ForwardLeft; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Forward_Right)		{ return ForwardRight; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Left)				{ return Left; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Right)				{ return Right; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_BackwardTurnLeft)	{ return BackwardTurnLeft; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_BackwardTurnRight)	{ return BackwardTurnRight; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Backward_Left)		{ return BackwardLeft; }
	if (CardinalTag == FSimpleTags::Simple_Cardinal_Backward_Right)		{ return BackwardRight; }
	ensure(false);  // Did you enable the cardinals you are trying to use?
	return nullptr;
}

FSimpleStrafeGaitSet::FSimpleStrafeGaitSet()
{
	Sets.Add(FSimpleTags::Simple_Gait_Run);

	// Strolling should fall back to walk if unavailable, otherwise run, otherwise sprint
	FSimpleGameplayTagArray& StrollFallback = Fallbacks.Add(FSimpleTags::Simple_Gait_Stroll);
	StrollFallback.AddTagFast(FSimpleTags::Simple_Gait_Walk);
	StrollFallback.AddTagFast(FSimpleTags::Simple_Gait_Run);
	StrollFallback.AddTagFast(FSimpleTags::Simple_Gait_Sprint);

	// Walking should fall back to run if unavailable, otherwise stroll, otherwise sprint
	FSimpleGameplayTagArray& WalkFallback = Fallbacks.Add(FSimpleTags::Simple_Gait_Walk);
	WalkFallback.AddTagFast(FSimpleTags::Simple_Gait_Run);
	WalkFallback.AddTagFast(FSimpleTags::Simple_Gait_Stroll);
	WalkFallback.AddTagFast(FSimpleTags::Simple_Gait_Sprint);

	// Run should fall back to walk if unavailable, otherwise sprint, otherwise stroll
	FSimpleGameplayTagArray& RunFallback = Fallbacks.Add(FSimpleTags::Simple_Gait_Run);
	RunFallback.AddTagFast(FSimpleTags::Simple_Gait_Walk);
	RunFallback.AddTagFast(FSimpleTags::Simple_Gait_Sprint);
	RunFallback.AddTagFast(FSimpleTags::Simple_Gait_Stroll);
	
	// Sprint should fall back to run if unavailable, otherwise walk, otherwise stroll
	FSimpleGameplayTagArray& SprintFallback = Fallbacks.Add(FSimpleTags::Simple_Gait_Sprint);
	SprintFallback.AddTagFast(FSimpleTags::Simple_Gait_Run);
	SprintFallback.AddTagFast(FSimpleTags::Simple_Gait_Walk);
	SprintFallback.AddTagFast(FSimpleTags::Simple_Gait_Stroll);
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
	Sets.Add(FSimpleTags::Simple_Gait_Run);

	// Strolling should fall back to walk if unavailable, otherwise run, otherwise sprint
	FSimpleGameplayTagArray& StrollFallback = Fallbacks.Add(FSimpleTags::Simple_Gait_Stroll);
	StrollFallback.AddTagFast(FSimpleTags::Simple_Gait_Walk);
	StrollFallback.AddTagFast(FSimpleTags::Simple_Gait_Run);
	StrollFallback.AddTagFast(FSimpleTags::Simple_Gait_Sprint);

	// Walking should fall back to run if unavailable, otherwise stroll, otherwise sprint
	FSimpleGameplayTagArray& WalkFallback = Fallbacks.Add(FSimpleTags::Simple_Gait_Walk);
	WalkFallback.AddTagFast(FSimpleTags::Simple_Gait_Run);
	WalkFallback.AddTagFast(FSimpleTags::Simple_Gait_Stroll);
	WalkFallback.AddTagFast(FSimpleTags::Simple_Gait_Sprint);

	// Run should fall back to walk if unavailable, otherwise sprint, otherwise stroll
	FSimpleGameplayTagArray& RunFallback = Fallbacks.Add(FSimpleTags::Simple_Gait_Run);
	RunFallback.AddTagFast(FSimpleTags::Simple_Gait_Walk);
	RunFallback.AddTagFast(FSimpleTags::Simple_Gait_Sprint);
	RunFallback.AddTagFast(FSimpleTags::Simple_Gait_Stroll);
	
	// Sprint should fall back to run if unavailable, otherwise walk, otherwise stroll
	FSimpleGameplayTagArray& SprintFallback = Fallbacks.Add(FSimpleTags::Simple_Gait_Sprint);
	SprintFallback.AddTagFast(FSimpleTags::Simple_Gait_Run);
	SprintFallback.AddTagFast(FSimpleTags::Simple_Gait_Walk);
	SprintFallback.AddTagFast(FSimpleTags::Simple_Gait_Stroll);
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
	Sets.Add(FSimpleTags::Simple_Gait_Run);

	// Strolling should fall back to walk if unavailable, otherwise run, otherwise sprint
	FSimpleGameplayTagArray& StrollFallback = Fallbacks.Add(FSimpleTags::Simple_Gait_Stroll);
	StrollFallback.AddTagFast(FSimpleTags::Simple_Gait_Walk);
	StrollFallback.AddTagFast(FSimpleTags::Simple_Gait_Run);
	StrollFallback.AddTagFast(FSimpleTags::Simple_Gait_Sprint);

	// Walking should fall back to run if unavailable, otherwise stroll, otherwise sprint
	FSimpleGameplayTagArray& WalkFallback = Fallbacks.Add(FSimpleTags::Simple_Gait_Walk);
	WalkFallback.AddTagFast(FSimpleTags::Simple_Gait_Run);
	WalkFallback.AddTagFast(FSimpleTags::Simple_Gait_Stroll);
	WalkFallback.AddTagFast(FSimpleTags::Simple_Gait_Sprint);

	// Run should fall back to walk if unavailable, otherwise sprint, otherwise stroll
	FSimpleGameplayTagArray& RunFallback = Fallbacks.Add(FSimpleTags::Simple_Gait_Run);
	RunFallback.AddTagFast(FSimpleTags::Simple_Gait_Walk);
	RunFallback.AddTagFast(FSimpleTags::Simple_Gait_Sprint);
	RunFallback.AddTagFast(FSimpleTags::Simple_Gait_Stroll);
	
	// Sprint should fall back to run if unavailable, otherwise walk, otherwise stroll
	FSimpleGameplayTagArray& SprintFallback = Fallbacks.Add(FSimpleTags::Simple_Gait_Sprint);
	SprintFallback.AddTagFast(FSimpleTags::Simple_Gait_Run);
	SprintFallback.AddTagFast(FSimpleTags::Simple_Gait_Walk);
	SprintFallback.AddTagFast(FSimpleTags::Simple_Gait_Stroll);
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
	Animations.Add(FSimpleTags::Simple_Stance_Stand);
	
	// Crouch falls back to standing
	FSimpleGameplayTagArray& CrouchFallback = Fallbacks.Add(FSimpleTags::Simple_Stance_Crouch);
	CrouchFallback.AddTagFast(FSimpleTags::Simple_Stance_Stand);

	// Prone falls back to crouch
	FSimpleGameplayTagArray& ProneFallback = Fallbacks.Add(FSimpleTags::Simple_Stance_Prone);
	ProneFallback.AddTagFast(FSimpleTags::Simple_Stance_Crouch);
}

FSimpleStanceToStrafeGaitSet::FSimpleStanceToStrafeGaitSet()
{
	Sets.Add(FSimpleTags::Simple_Stance_Stand);

	// Crouch falls back to standing
	FSimpleGameplayTagArray& CrouchFallback = Fallbacks.Add(FSimpleTags::Simple_Stance_Crouch);
	CrouchFallback.AddTagFast(FSimpleTags::Simple_Stance_Stand);

	// Prone falls back to crouch
	FSimpleGameplayTagArray& ProneFallback = Fallbacks.Add(FSimpleTags::Simple_Stance_Prone);
	ProneFallback.AddTagFast(FSimpleTags::Simple_Stance_Crouch);
}

FSimpleStanceToStartGaitSet::FSimpleStanceToStartGaitSet()
{
	Sets.Add(FSimpleTags::Simple_Stance_Stand);

	// Crouch falls back to standing
	FSimpleGameplayTagArray& CrouchFallback = Fallbacks.Add(FSimpleTags::Simple_Stance_Crouch);
	CrouchFallback.AddTagFast(FSimpleTags::Simple_Stance_Stand);

	// Prone falls back to crouch
	FSimpleGameplayTagArray& ProneFallback = Fallbacks.Add(FSimpleTags::Simple_Stance_Prone);
	ProneFallback.AddTagFast(FSimpleTags::Simple_Stance_Crouch);
}

FSimpleStanceToTurnGaitSet::FSimpleStanceToTurnGaitSet()
{
	Sets.Add(FSimpleTags::Simple_Stance_Stand);

	// Crouch falls back to standing
	FSimpleGameplayTagArray& CrouchFallback = Fallbacks.Add(FSimpleTags::Simple_Stance_Crouch);
	CrouchFallback.AddTagFast(FSimpleTags::Simple_Stance_Stand);

	// Prone falls back to crouch
	FSimpleGameplayTagArray& ProneFallback = Fallbacks.Add(FSimpleTags::Simple_Stance_Prone);
	ProneFallback.AddTagFast(FSimpleTags::Simple_Stance_Crouch);
}

FSimpleStateToStrafeGaitSet::FSimpleStateToStrafeGaitSet()
{
	Sets.Add(FSimpleTags::Simple_State_Default);
}

FSimpleStateToStartGaitSet::FSimpleStateToStartGaitSet()
{
	Sets.Add(FSimpleTags::Simple_State_Default);
}

FSimpleStateToTurnGaitSet::FSimpleStateToTurnGaitSet()
{
	Sets.Add(FSimpleTags::Simple_State_Default);
}

FSimpleStateToStanceToStrafeGaitSet::FSimpleStateToStanceToStrafeGaitSet()
{
	Sets.Add(FSimpleTags::Simple_State_Default);
}

FSimpleStateToStanceToStartGaitSet::FSimpleStateToStanceToStartGaitSet()
{
	Sets.Add(FSimpleTags::Simple_State_Default);
}

FSimpleStateToStanceToTurnGaitSet::FSimpleStateToStanceToTurnGaitSet()
{
	Sets.Add(FSimpleTags::Simple_State_Default);
}

FSimpleStanceToTransitionSet::FSimpleStanceToTransitionSet()
{
	Sets.Add(FSimpleTags::Simple_Stance_Stand);
	
	// Crouch falls back to standing
	FSimpleGameplayTagArray& CrouchFallback = Fallbacks.Add(FSimpleTags::Simple_Stance_Crouch);
	CrouchFallback.AddTagFast(FSimpleTags::Simple_Stance_Stand);

	// Prone falls back to crouch
	FSimpleGameplayTagArray& ProneFallback = Fallbacks.Add(FSimpleTags::Simple_Stance_Prone);
	ProneFallback.AddTagFast(FSimpleTags::Simple_Stance_Crouch);
}

FSimpleStateToStanceSet::FSimpleStateToStanceSet()
{
	Sets.Add(FSimpleTags::Simple_State_Default);
}

FSimpleStateToStanceToTransitionSet::FSimpleStateToStanceToTransitionSet()
{
	Sets.Add(FSimpleTags::Simple_State_Default);
}
