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
			"UMG",
			// "StateTree" removed because the module/plugin isn't enabled in the project.
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"ControlRig"
		});

		// Editor-only dependencies required for tools that manipulate blueprints
		if (Target.bBuildEditor == true)
		{
			PrivateDependencyModuleNames.AddRange(new string[] {
				"UnrealEd",
				"Kismet",
				"BlueprintGraph",
				"KismetCompiler"
			});
		}

		// Public include paths for Live Coding recompilation
		PublicIncludePaths.AddRange(new string[] {
			"Adventure/Public",
			"Adventure/Public/Core",
			"Adventure/Public/Character",
			"Adventure/Public/GAS",
			// UI folder removed; it doesn't exist in the repository.
			"Adventure/Public/Components"   // headers like AdventureGASStateTreeBridgeComponent
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
	}
}
