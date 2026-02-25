// Copyright © 2026 Nola Development Incubator. All Rights Reserved.

#include "MCPServerPlugin.h"

#define LOCTEXT_NAMESPACE "FMCPServerPluginModule"

void FMCPServerPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogTemp, Log, TEXT("MCP Server Plugin: Module Started"));
}

void FMCPServerPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(LogTemp, Log, TEXT("MCP Server Plugin: Module Shutdown"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMCPServerPluginModule, MCPServerPlugin)
