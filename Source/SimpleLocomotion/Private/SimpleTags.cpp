// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleTags.h"

namespace FSimpleTags
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
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_Strafe_1Way, TEXT("Simple.Mode.Strafe.1Way"), "Forward movement without strafing");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_Strafe_2Way, TEXT("Simple.Mode.Strafe.2Way"), "2-Way Movement, e.g. side-scroller movement");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_Strafe_4Way, TEXT("Simple.Mode.Strafe.4Way"), "4-Way Strafing");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_Strafe_6Way, TEXT("Simple.Mode.Strafe.6Way"), "6-Way Strafing - 4-Way, but includes strafing with back facing away");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_Strafe_8Way, TEXT("Simple.Mode.Strafe.8Way"), "8-Way Strafing");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_Strafe_10Way, TEXT("Simple.Mode.Strafe.10Way"), "10-Way Strafing - 8-Way, but includes strafing with back facing away");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_Start_1Way, TEXT("Simple.Mode.Start.1Way"), "Forward only");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_Start_2Way, TEXT("Simple.Mode.Start.2Way"), "Forward and backward only");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_Start_4Way, TEXT("Simple.Mode.Start.4Way"), "Forward, left, right, and backward");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_Start_8Way, TEXT("Simple.Mode.Start.8Way"), "Forward, 45 degree increments, and backward");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_Turn_1Way, TEXT("Simple.Mode.Turn.1Way"), "Turn around backwards only");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_Turn_4Way, TEXT("Simple.Mode.Turn.4Way"), "4-Way Turning");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Mode_Turn_8Way, TEXT("Simple.Mode.Turn.8Way"), "8-Way Turning");
	
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
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Cardinal_BackwardTurnLeft, TEXT("Simple.Cardinal.BackwardTurnLeft"), "Used for start or forward movement pivot anims that can turn 180 degrees to either the left or right");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Simple_Cardinal_BackwardTurnRight, TEXT("Simple.Cardinal.BackwardTurnRight"), "Used for start or forward movement pivot anims that can turn 180 degrees to either the left or right");
}