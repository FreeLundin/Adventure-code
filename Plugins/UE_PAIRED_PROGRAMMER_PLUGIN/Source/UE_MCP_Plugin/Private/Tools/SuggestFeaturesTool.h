#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

/**
 * Simple tool that returns a list of potential gameplay features based on a
 * short description provided by the user.
 * This is a stub that could later be backed by an LLM or design database.
 */
class UE_MCP_PLUGIN_API FSuggestFeaturesTool
{
public:
    static bool Execute(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult);
    static TSharedPtr<FJsonObject> GetInputSchema();
    static FString GetName() { return TEXT("gameplay.suggestFeatures"); }
    static FString GetDescription() { return TEXT("Suggests gameplay feature ideas based on a brief game description"); }
};
