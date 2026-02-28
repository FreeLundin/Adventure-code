#include "AdventureModule.h"
#include "Modules/ModuleManager.h"

// include our new tool and registry
#include "MCPTools/BlueprintBindTool.h"
// The MCP plugin has been removed/disabled for this workspace. Do not include its headers here.

void FAdventureModule::StartupModule()
{
    // register the blueprint binding tool with the MCP registry if available
    // blueprint bind tool is now registered by the UE_MCP_Plugin itself; no action required here
}

void FAdventureModule::ShutdownModule()
{
    // no-op; registration is handled by the MCP plugin itself and the plugin
    // is currently disabled to avoid build errors.
}

IMPLEMENT_PRIMARY_GAME_MODULE( FAdventureModule, Adventure, "Adventure" );
