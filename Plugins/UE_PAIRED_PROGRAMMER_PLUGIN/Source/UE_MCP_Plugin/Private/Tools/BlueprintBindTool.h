#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

/**
 * MCP tool that binds traversal ability input to blueprints.
 * Located inside the plugin so it is available to any project using the plugin.
 */
class UE_MCP_PLUGIN_API FBlueprintBindTool
{
public:
    static bool Execute(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult);
    static TSharedPtr<FJsonObject> GetInputSchema();
    static FString GetName() { return TEXT("blueprint.bindTraversal"); }
    static FString GetDescription() { return TEXT("Adds traversal ability input wiring to one or more blueprints"); }
};
