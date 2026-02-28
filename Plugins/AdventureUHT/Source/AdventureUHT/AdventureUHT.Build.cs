using UnrealBuildTool;
using System.Collections.Generic;

public class AdventureUHT : ModuleRules
{
    public AdventureUHT(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});

        // This module only contains editor/UHT helpers; add editor dependencies
        // when building for the editor so the module can reference UnrealEd.
        if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] {"UnrealEd"});
        }
    }
}
