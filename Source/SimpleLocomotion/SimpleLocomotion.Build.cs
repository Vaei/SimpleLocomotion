﻿// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SimpleLocomotion : ModuleRules
{
	public SimpleLocomotion(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", 
				"GameplayTags",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
			}
			);
		
		if (Target.bBuildEditor)
		{
			// FNotificationInfo & FSlateNotificationManager
			PrivateDependencyModuleNames.Add("Slate");
		}
	}
}
