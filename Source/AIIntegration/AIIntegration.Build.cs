// AIIntegration.Build.cs
// Defines the build rules for the AIIntegration module, which provides
// the UModelAgentComponent for loading and running NNE/ONNX models and
// reporting telemetry.

using UnrealBuildTool;
using System.Collections.Generic;

public class AIIntegration : ModuleRules
{
    public AIIntegration(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Latest;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Http",
            // NNE runtime provides model loading/inference APIs
            "NNERuntime"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            // add any private dependencies here
        });

        PublicIncludePaths.AddRange(new string[] {
            "AIIntegration/Public"
        });

        PrivateIncludePaths.AddRange(new string[] {
            "AIIntegration/Private"
        });
    }
}