// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Adventure : ModuleRules
{
	public Adventure(ReadOnlyTargetRules Target) : base(Target)
	{
		// Optimized for Live Coding: Use explicit PCHs
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Latest;
		
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"EnhancedInput",
			"Mover",
			"MotionWarping",
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"ControlRig"
		});

		// Public include paths for Live Coding recompilation
		PublicIncludePaths.AddRange(new string[] {
			"Adventure/Public",
			"Adventure/Public/Core",
			"Adventure/Public/Character",
			"Adventure/Public/GAS",
			"Adventure/Public/UI"
		});

		// Private include paths for internal compilation
		PrivateIncludePaths.AddRange(new string[] {
			"Adventure/Private",
			"Adventure/Private/Core",
			"Adventure/Private/Character",
			"Adventure/Private/GAS"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
		
		// Editor-only modules for Blueprint manipulation utilities
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { 
				"UnrealEd",          // Core editor functionality
				"Kismet",            // Blueprint editing utilities
				"BlueprintGraph",    // Blueprint graph nodes
				"AssetRegistry"      // Asset discovery and loading
			});
		}
	}
}
