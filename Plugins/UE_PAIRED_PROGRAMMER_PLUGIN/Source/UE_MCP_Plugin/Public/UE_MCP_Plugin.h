// Copyright © 2026 Nola Development Incubator. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUE_MCP_Server;
class FConnectorClient;
class FTokenManager;
class FToolRegistry;

/**
 * UE_MCP_Plugin Module
 * Enterprise-grade Unreal Engine plugin implementing the Model Context Protocol (MCP)
 * Developed by Nola Development Incubator
 * 
 * Provides secure AI-assisted automation inside the Unreal Editor by connecting
 * to an MCP Aggregator over WebSocket using JSON-RPC, validating RS256 JWT tokens
 * via JWKS, and enforcing capability-based policies for safe, moderate, and
 * destructive actions.
 */
class FUE_MCP_PluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/**
	 * Get the MCP Server instance
	 * @return Pointer to MCP Server
	 */
	FUE_MCP_Server* GetMCPServer() const { return MCPServer.Get(); }

	/**
	 * Get the Connector Client instance
	 * @return Pointer to Connector Client
	 */
	FConnectorClient* GetConnectorClient() const { return ConnectorClient.Get(); }

	/**
	 * Get the Token Manager instance
	 * @return Pointer to Token Manager
	 */
	FTokenManager* GetTokenManager() const { return TokenManager.Get(); }

	/**
	 * Get the Tool Registry instance
	 * @return Pointer to Tool Registry
	 */
	FToolRegistry* GetToolRegistry() const { return ToolRegistry.Get(); }

	/**
	 * Spawn the MCP Agent dashboard tab (Slate)
	 */
	TSharedRef<class SDockTab> SpawnDashboardTab(const struct FSpawnTabArgs& Args);

private:
	/**
	 * Initialize all MCP components
	 */
	void InitializeComponents();

	/**
	 * Register built-in tools
	 */
	void RegisterBuiltInTools();

	/**
	 * Shutdown all MCP components
	 */
	void ShutdownComponents();

	// MCP Server instance
	TUniquePtr<FUE_MCP_Server> MCPServer;

	// Connector Client instance
	TUniquePtr<FConnectorClient> ConnectorClient;

	// Token Manager instance
	TUniquePtr<FTokenManager> TokenManager;

	// Tool Registry instance
	TUniquePtr<FToolRegistry> ToolRegistry;
};
