// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SoulLike : ModuleRules
{
	public SoulLike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities", "PoseSearch"});
        PublicDefinitions.Add("PLATFORM_ARM64=1");
		PrivateDependencyModuleNames.AddRange(new string[] { 			
			"GameplayTasks", 
			"GameplayTags",
			"NetCore", 
			"MotionWarping",
			"AIModule",
			"MotionTrajectory"
			
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
