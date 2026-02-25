// Copyright © 2026 Nola Development Incubator. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpServerModule.h"
#include "IHttpRouter.h"

/**
 * Delegate for MCP tool execution
 * @param ToolName - The name of the tool to execute
 * @param Parameters - JSON object containing tool parameters
 * @param OutResult - JSON object to populate with the result
 * @return true if the tool executed successfully, false otherwise
 */
DECLARE_DELEGATE_ThreeParams(FOnMCPToolExecute, const FString& /*ToolName*/, TSharedPtr<FJsonObject> /*Parameters*/, TSharedPtr<FJsonObject>& /*OutResult*/);

/**
 * MCP Server class that handles JSON-RPC requests and manages tool routing
 */
class UE_MCP_PLUGIN_API FUE_MCP_Server
{
public:
	FUE_MCP_Server();
	~FUE_MCP_Server();

	/**
	 * Start the MCP server on the specified port
	 * @param Port - Port number to listen on (default: 3000)
	 * @return true if server started successfully, false otherwise
	 */
	bool StartServer(int32 Port = 3000);

	/**
	 * Stop the MCP server
	 */
	void StopServer();

	/**
	 * Check if the server is currently running
	 * @return true if server is running, false otherwise
	 */
	bool IsRunning() const { return bIsRunning; }

	/**
	 * Register a tool with the MCP server
	 * @param ToolName - Name of the tool
	 * @param Description - Description of what the tool does
	 * @param Callback - Delegate to execute when tool is called
	 */
	void RegisterTool(const FString& ToolName, const FString& Description, FOnMCPToolExecute Callback);

	/**
	 * Unregister a tool from the MCP server
	 * @param ToolName - Name of the tool to unregister
	 */
	void UnregisterTool(const FString& ToolName);

	/**
	 * Send a JSON-RPC notification to all connected clients
	 * @param Method - The notification method name
	 * @param Params - Optional parameters as a JSON object
	 */
	void SendNotification(const FString& Method, TSharedPtr<FJsonObject> Params = nullptr);

	/**
	 * Handle incoming JSON-RPC request
	 * @param RequestJson - The JSON-RPC request as a string
	 * @return The JSON-RPC response as a string
	 */
	FString HandleJsonRpcRequest(const FString& RequestJson);

private:
	/**
	 * Process a JSON-RPC method call
	 * @param Method - The method name
	 * @param Params - The parameters JSON object
	 * @param Id - The request ID
	 * @return Response JSON object
	 */
	TSharedPtr<FJsonObject> ProcessMethodCall(const FString& Method, TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonValue> Id);

	/**
	 * Create a JSON-RPC error response
	 * @param Code - Error code
	 * @param Message - Error message
	 * @param Id - Request ID
	 * @return Error response JSON object
	 */
	TSharedPtr<FJsonObject> CreateErrorResponse(int32 Code, const FString& Message, TSharedPtr<FJsonValue> Id);

	/**
	 * Create a JSON-RPC success response
	 * @param Result - Result JSON object
	 * @param Id - Request ID
	 * @return Success response JSON object
	 */
	TSharedPtr<FJsonObject> CreateSuccessResponse(TSharedPtr<FJsonObject> Result, TSharedPtr<FJsonValue> Id);
	
	/**
	 * HTTP request handler for MCP endpoint
	 */
	bool HandleHttpRequest(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete);

	// Structure to hold tool information
	struct FMCPTool
	{
		FString Name;
		FString Description;
		FOnMCPToolExecute Callback;
	};

	// Map of registered tools
	TMap<FString, FMCPTool> RegisteredTools;

	// Server state
	bool bIsRunning;
	int32 ServerPort;

	// HTTP Server
	TSharedPtr<IHttpRouter> HttpRouter;
};
