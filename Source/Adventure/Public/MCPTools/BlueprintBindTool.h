// Public-facing header for BlueprintBindTool (keeps tool usable without editor-only private header issues)
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

class FBlueprintBindTool
{
public:
    static bool Execute(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult);
    static TSharedPtr<FJsonObject> GetInputSchema();
    static FString GetName() { return TEXT("blueprint.bindTraversal"); }
    static FString GetDescription() { return TEXT("Adds traversal ability input wiring to one or more blueprints"); }
};
