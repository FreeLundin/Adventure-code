#include "AdventureModule.h"
#include "Modules/ModuleManager.h"

// include our new tool and registry
#include "MCPTools/BlueprintBindTool.h"
#include "UE_MCP_Plugin/Public/ToolRegistry.h"

void FAdventureModule::StartupModule()
{
    // register the blueprint binding tool with the MCP registry if available
    // blueprint bind tool is now registered by the UE_MCP_Plugin itself; no action required here
}

void FAdventureModule::ShutdownModule()
{
    if (FToolRegistry* Registry = FToolRegistry::Get())
    {
        Registry->UnregisterTool(FBlueprintBindTool::GetName());
    }
}

IMPLEMENT_PRIMARY_GAME_MODULE( FAdventureModule, Adventure, "Adventure" );
