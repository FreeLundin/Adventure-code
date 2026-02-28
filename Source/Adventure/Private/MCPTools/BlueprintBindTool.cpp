#include "MCPTools/BlueprintBindTool.h"
// This tool manipulates Blueprint graphs and relies on editor-only headers.
// To avoid hard editor dependencies in the runtime build of the project
// we provide a minimal stub implementation here so the module can link.

bool FBlueprintBindTool::Execute(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult)
{
    // Editor-only functionality is intentionally disabled in this build.
    // If you need to use this tool, build the Editor target with the MCP plugin
    // and appropriate editor modules available.
    return false;
}

TSharedPtr<FJsonObject> FBlueprintBindTool::GetInputSchema()
{
    return MakeShared<FJsonObject>();
}
