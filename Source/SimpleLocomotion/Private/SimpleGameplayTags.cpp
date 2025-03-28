// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleGameplayTags.h"

namespace FSimpleGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_State_Default, TEXT("Simple.State.Default"), "Represents the overarching anim state your character is in, this will be project specific, e.g. combat state.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Gait_Idle, TEXT("Simple.Gait.Idle"), "Idle - Standing still. NOT used, exists for extension only. This could be used for a stance transition system that has both idle and moving gait-based transitions, e.g. sprint to prone");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Gait_Stroll, TEXT("Simple.Gait.Stroll"), "Strolling - Not doing anything important but also not standing still. Possibly patrolling.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Gait_Walk, TEXT("Simple.Gait.Walk"), "Walking - Moving slower than our run");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Gait_Run, TEXT("Simple.Gait.Run"), "Running - Generic gait when no inputs or modifiers are applied");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Gait_Sprint, TEXT("Simple.Gait.Sprint"), "Sprinting - Moving faster than our run");
	
	UE_DEFINE_GAMEPLAY_TAG(Simple_Stance_Stand, TEXT("Simple.Stance.Stand"));
	UE_DEFINE_GAMEPLAY_TAG(Simple_Stance_Crouch, TEXT("Simple.Stance.Crouch"));
	UE_DEFINE_GAMEPLAY_TAG(Simple_Stance_Prone, TEXT("Simple.Stance.Prone"));
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_1Way, TEXT("Simple.Mode.1Way"), "Forward movement without strafing");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_2Way, TEXT("Simple.Mode.2Way"), "2-Way Movement, e.g. side-scroller movement");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_4Way, TEXT("Simple.Mode.4Way"), "4-Way Strafing");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_6Way, TEXT("Simple.Mode.6Way"), "6-Way Strafing - 4-Way, but includes strafing with back facing away");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_8Way, TEXT("Simple.Mode.8Way"), "8-Way Strafing");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_10Way, TEXT("Simple.Mode.10Way"), "10-Way Strafing - 8-Way, but includes strafing with back facing away");

	UE_DEFINE_GAMEPLAY_TAG(Simple_Cardinal_Forward, TEXT("Simple.Cardinal.Forward"));
	UE_DEFINE_GAMEPLAY_TAG(Simple_Cardinal_Forward_Left, TEXT("Simple.Cardinal.Forward.Left"));
	UE_DEFINE_GAMEPLAY_TAG(Simple_Cardinal_Forward_Right, TEXT("Simple.Cardinal.Forward.Right"));
	UE_DEFINE_GAMEPLAY_TAG(Simple_Cardinal_Left, TEXT("Simple.Cardinal.Left"));
	UE_DEFINE_GAMEPLAY_TAG(Simple_Cardinal_Right, TEXT("Simple.Cardinal.Right"));
	UE_DEFINE_GAMEPLAY_TAG(Simple_Cardinal_Left_Away, TEXT("Simple.Cardinal.Left.Away"));
	UE_DEFINE_GAMEPLAY_TAG(Simple_Cardinal_Right_Away, TEXT("Simple.Cardinal.Right.Away"));
	UE_DEFINE_GAMEPLAY_TAG(Simple_Cardinal_Backward, TEXT("Simple.Cardinal.Backward"));
	UE_DEFINE_GAMEPLAY_TAG(Simple_Cardinal_Backward_Left, TEXT("Simple.Cardinal.Backward.Left"));
	UE_DEFINE_GAMEPLAY_TAG(Simple_Cardinal_Backward_Right, TEXT("Simple.Cardinal.Backward.Right"));
}