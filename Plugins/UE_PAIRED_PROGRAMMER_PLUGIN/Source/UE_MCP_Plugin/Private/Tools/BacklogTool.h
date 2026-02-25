#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

/**
 * Simple backlog management utility for designers/PMs.  Items are stored in
 * Saved/MCPBacklog.json as an array of strings.  Two tools are provided:
 *   - backlog.add  { "item": "text" }
 *   - backlog.list { }
 */
class UE_MCP_PLUGIN_API FBacklogTool
{
public:
    static bool Execute(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult);
    static TSharedPtr<FJsonObject> GetInputSchema();
    static FString GetName() { return TEXT("backlog.manage"); }
    static FString GetDescription() { return TEXT("Manage a simple product backlog (add/list items)"); }
};
