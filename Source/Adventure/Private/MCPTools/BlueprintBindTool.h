#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

/**
 * Simple MCP tool that binds a traversal input action to a list of blueprints.
 * This is analogous to the Python automation script but exposed as a UE_MCP
 * tool so the UE_MCP_Agent can invoke it directly.
 */
class FBlueprintBindTool
{
public:
    static bool Execute(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult);
    static TSharedPtr<FJsonObject> GetInputSchema();
    static FString GetName() { return TEXT("blueprint.bindTraversal"); }
    static FString GetDescription() { return TEXT("Adds traversal ability input wiring to one or more blueprints"); }
};
