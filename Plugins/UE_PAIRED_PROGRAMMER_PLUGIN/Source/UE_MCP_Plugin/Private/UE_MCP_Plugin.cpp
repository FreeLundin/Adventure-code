// Copyright © 2026 Nola Development Incubator. All Rights Reserved.

#include "UE_MCP_Plugin.h"
#include "UE_MCP_Server.h"
#include "ConnectorClient.h"
#include "TokenManager.h"
#include "ToolRegistry.h"
#include "Tools/ActorSpawnTool.h"
#include "Tools/BacklogTool.h"

#define LOCTEXT_NAMESPACE "FUE_MCP_PluginModule"

void FUE_MCP_PluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Module Starting"));
	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Developed by Nola Development Incubator"));
	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Enterprise-grade MCP Server + Connector for Unreal Engine 5.7.1"));

	// optionally defer heavy initialization until needed
	bool bAutoStart = true;
    if (const UMcpAgentSettings* Settings = GetDefault<UMcpAgentSettings>())
    {
        bAutoStart = Settings->bAutoStart;
    }

    if (bAutoStart)
    {
        InitializeComponents();
        RegisterBuiltInTools();
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: auto-start disabled in settings, components will init lazily"));

	// register dashboard tab (visible under Window > Developer Tools)
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		TEXT("MCPAgentDashboard"),
		FOnSpawnTab::CreateRaw(this, &FUE_MCP_PluginModule::SpawnDashboardTab))
		.SetDisplayName(NSLOCTEXT("MCP","Dashboard","MCP Agent Dashboard"))
		.SetMenuType(ETabSpawnerMenuType::Enabled)
		.SetGroup(WorkspaceMenu::GetMenuStructure().GetDeveloperToolsMiscCategory());

	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Module Started Successfully"));
}


TSharedRef<SDockTab> FUE_MCP_PluginModule::SpawnDashboardTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
	.TabRole(ETabRole::NomadTab)
	[
		SNew(SAgentDashboard)
	];
}

void FUE_MCP_PluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Module Shutting Down"));

	ShutdownComponents();

	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Module Shutdown Complete"));
}

void FUE_MCP_PluginModule::InitializeComponents()
{
	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Initializing Components"));

	// Create MCP Server
	MCPServer = MakeUnique<FUE_MCP_Server>();
	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: MCP Server Created"));

	// Create Connector Client
	ConnectorClient = MakeUnique<FConnectorClient>();
	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Connector Client Created"));

	// Create Token Manager
	TokenManager = MakeUnique<FTokenManager>();
	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Token Manager Created"));

	// Create Tool Registry
	ToolRegistry = MakeUnique<FToolRegistry>();
	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Tool Registry Created"));

	// TODO: Load configuration from settings
	// For now, we'll use default settings
	
	// Start MCP Server on port 4020
	if (MCPServer.IsValid())
	{
		MCPServer->StartServer(4020);
		UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: MCP Server started on port 4020"));
	}
	
	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Components Initialized"));
}

void FUE_MCP_PluginModule::RegisterBuiltInTools()
{
	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Registering Built-In Tools"));

	// if initialization was deferred, do it now before any tools are added
	if (!MCPServer.IsValid())
	{
		InitializeComponents();
	}

	if (!ToolRegistry.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("UE_MCP_Plugin: Tool Registry is not valid, cannot register tools"));
		return;
	}

	// Register actors.spawn tool
	{
		FOnToolExecute SpawnDelegate;
		SpawnDelegate.BindLambda([](TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult) -> bool
		{
			return FActorSpawnTool::Execute(Params, OutResult);
		});

		// the backlog manager can be invoked remotely or from the dashboard; the
		// plugin is sometimes referred to as the "Paired Programmer" plugin in
		// documentation.
		ToolRegistry->RegisterTool(
			FActorSpawnTool::GetName(),
			FActorSpawnTool::GetDescription(),
			FActorSpawnTool::GetInputSchema(),
			SpawnDelegate
		);

		UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Registered tool '%s'"), *FActorSpawnTool::GetName());

		// Also register the tool with the MCP Server so HTTP JSON-RPC calls (e.g., tools/actors.spawn) are forwarded
		if (MCPServer.IsValid())
		{
			FOnMCPToolExecute MCPDelegate;
			MCPDelegate.BindLambda([this](const FString& ToolName, TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult)
			{
				// Forward execution to the internal ToolRegistry
				if (ToolRegistry.IsValid())
				{
					bool bExec = ToolRegistry->ExecuteTool(ToolName, Params, OutResult);
					if (!bExec && OutResult.IsValid() && !OutResult->HasField(TEXT("error")))
					{
						OutResult->SetBoolField(TEXT("success"), false);
						OutResult->SetStringField(TEXT("error"), TEXT("Tool execution failed or returned false"));
					}
				}
				else
				{
					if (OutResult.IsValid())
					{
						OutResult->SetBoolField(TEXT("success"), false);
						OutResult->SetStringField(TEXT("error"), TEXT("Tool registry not available"));
					}
				}
			});

			MCPServer->RegisterTool(FActorSpawnTool::GetName(), FActorSpawnTool::GetDescription(), MCPDelegate);			UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Registered tool '%s' with MCP Server"), *FActorSpawnTool::GetName());
		}
	}

// register backlog manager tool
    {
        FOnToolExecute BacklogDelegate;
        BacklogDelegate.BindLambda([](TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult) -> bool
        {
            return FBacklogTool::Execute(Params, OutResult);
        });
        ToolRegistry->RegisterTool(
            FBacklogTool::GetName(),
            FBacklogTool::GetDescription(),
            FBacklogTool::GetInputSchema(),
            BacklogDelegate
        );
        UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Registered tool '%s'"), *FBacklogTool::GetName());

        if (MCPServer.IsValid())
        {
            FOnMCPToolExecute MCPDelegate;
            MCPDelegate.BindLambda([this](const FString& ToolName, TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult)
            {
                if (ToolRegistry.IsValid())
                {
                    ToolRegistry->ExecuteTool(ToolName, Params, OutResult);
                }
            });
            MCPServer->RegisterTool(FBacklogTool::GetName(), FBacklogTool::GetDescription(), MCPDelegate);
            UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Registered tool '%s' with MCP Server"), *FBacklogTool::GetName());
        }
    }

// register blueprint binding tool
    {
        FOnToolExecute BindDelegate;
        BindDelegate.BindLambda([](TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult) -> bool
        {
            return FBlueprintBindTool::Execute(Params, OutResult);
        });
        ToolRegistry->RegisterTool(
            FBlueprintBindTool::GetName(),
            FBlueprintBindTool::GetDescription(),
            FBlueprintBindTool::GetInputSchema(),
            BindDelegate
        );
        UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Registered tool '%s'"), *FBlueprintBindTool::GetName());

        if (MCPServer.IsValid())
        {
            FOnMCPToolExecute MCPDelegate;
            MCPDelegate.BindLambda([this](const FString& ToolName, TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult)
            {
                if (ToolRegistry.IsValid())
                {
                    ToolRegistry->ExecuteTool(ToolName, Params, OutResult);
                }
            });
            MCPServer->RegisterTool(FBlueprintBindTool::GetName(), FBlueprintBindTool::GetDescription(), MCPDelegate);
            UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Registered tool '%s' with MCP Server"), *FBlueprintBindTool::GetName());
        }
    }

    // telemetry reporting tool
    {
        FOnToolExecute TelemetryDelegate;
        TelemetryDelegate.BindLambda([](TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult) -> bool
        {
            FString Payload;
            if (!Params->TryGetStringField(TEXT("payload"), Payload))
            {
                return false;
            }
            UMcpTelemetryLibrary::ReportTelemetry(Payload);
            OutResult = MakeShared<FJsonObject>();
            OutResult->SetBoolField(TEXT("success"), true);
            return true;
        });

        ToolRegistry->RegisterTool(
            TEXT("telemetry.report"),
            TEXT("Send a JSON telemetry payload to the MCP telemetry endpoint"),
            MakeShared<FJsonObject>(), // no schema for simplicity
            TelemetryDelegate
        );
        UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Registered tool 'telemetry.report'"));

        if (MCPServer.IsValid())
        {
            FOnMCPToolExecute MCPDelegate;
            MCPDelegate.BindLambda([this](const FString& ToolName, TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult)
            {
                if (ToolRegistry.IsValid())
                {
                    ToolRegistry->ExecuteTool(ToolName, Params, OutResult);
                }
            });
            MCPServer->RegisterTool(TEXT("telemetry.report"), TEXT("Send telemetry payload"), MCPDelegate);
            UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Registered tool 'telemetry.report' with MCP Server"));
        }
    }

    // onboarding example creation tool
    {
        FOnToolExecute OnboardDelegate;
        OnboardDelegate.BindLambda([](TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult) -> bool
        {
            return FOnboardTool::Execute(Params, OutResult);
        });
        ToolRegistry->RegisterTool(
            FOnboardTool::GetName(),
            FOnboardTool::GetDescription(),
            FOnboardTool::GetInputSchema(),
            OnboardDelegate
        );
        UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Registered tool '%s'"), *FOnboardTool::GetName());

        if (MCPServer.IsValid())
        {
            FOnMCPToolExecute MCPDelegate;
            MCPDelegate.BindLambda([this](const FString& ToolName, TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult)
            {
                if (ToolRegistry.IsValid())
                {
                    ToolRegistry->ExecuteTool(ToolName, Params, OutResult);
                }
            });
            MCPServer->RegisterTool(FOnboardTool::GetName(), FOnboardTool::GetDescription(), MCPDelegate);
            UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Registered tool '%s' with MCP Server"), *FOnboardTool::GetName());
        }
    }

	// - blueprint.modify
	// - asset.import
	// - geometry.modify
	// - pie.start
	// - pie.stop

	int32 ToolCount = ToolRegistry->GetToolNames().Num();
	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Registered %d built-in tools"), ToolCount);

	// Diagnostic: log that we're about to forward tools
	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Attempting to forward %d tools to MCP Server"), ToolCount);

	// Forward all registered ToolRegistry tools to MCPServer so HTTP calls work
	if (MCPServer.IsValid() && ToolRegistry.IsValid())
	{
		for (const FString& Name : ToolRegistry->GetToolNames())
		{
			FOnMCPToolExecute Delegate;
			Delegate.BindLambda([this](const FString& ToolName, TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult)
			{
				if (ToolRegistry.IsValid())
				{
					ToolRegistry->ExecuteTool(ToolName, Params, OutResult);
				}
			});

			MCPServer->RegisterTool(Name, TEXT("Forwarded tool"), Delegate);
			UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Forwarded tool '%s' to MCP Server"), *Name);
		}
	}
}

void FUE_MCP_PluginModule::ShutdownComponents()
{
	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Shutting Down Components"));

	// Shutdown in reverse order
	if (ToolRegistry.IsValid())
	{
		ToolRegistry->ClearAll();
		ToolRegistry.Reset();
		UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Tool Registry Shutdown"));
	}

	if (TokenManager.IsValid())
	{
		TokenManager.Reset();
		UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Token Manager Shutdown"));
	}

	if (ConnectorClient.IsValid())
	{
		ConnectorClient->Disconnect();
		ConnectorClient.Reset();
		UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: Connector Client Shutdown"));
	}

	if (MCPServer.IsValid())
	{
		MCPServer->StopServer();
		MCPServer.Reset();
		UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: MCP Server Shutdown"));
	}

	UE_LOG(LogTemp, Log, TEXT("UE_MCP_Plugin: All Components Shutdown"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUE_MCP_PluginModule, UE_MCP_Plugin)
