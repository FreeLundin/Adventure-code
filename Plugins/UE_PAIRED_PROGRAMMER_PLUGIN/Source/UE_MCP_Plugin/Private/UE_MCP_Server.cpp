// Copyright © 2026 Nola Development Incubator. All Rights Reserved.

#include "UE_MCP_Server.h"
#include "JsonObjectConverter.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "HttpServerModule.h"
#include "HttpServerResponse.h"

// JSON-RPC Error Codes
#define JSONRPC_PARSE_ERROR -32700
#define JSONRPC_INVALID_REQUEST -32600
#define JSONRPC_METHOD_NOT_FOUND -32601
#define JSONRPC_INVALID_PARAMS -32602
#define JSONRPC_INTERNAL_ERROR -32603

FUE_MCP_Server::FUE_MCP_Server()
	: bIsRunning(false)
	, ServerPort(3000)
{
}

FUE_MCP_Server::~FUE_MCP_Server()
{
	StopServer();
}

bool FUE_MCP_Server::StartServer(int32 Port)
{
	if (bIsRunning)
	{
		UE_LOG(LogTemp, Warning, TEXT("MCP Server: Server is already running on port %d"), ServerPort);
		return false;
	}

	ServerPort = Port;
	
	// Get HTTP Server module
	FHttpServerModule& HttpServerModule = FHttpServerModule::Get();
	HttpRouter = HttpServerModule.GetHttpRouter(ServerPort);
	
	if (!HttpRouter.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("MCP Server: Failed to create HTTP router on port %d"), ServerPort);
		return false;
	}
	
	// Create the handler
	FHttpRequestHandler RequestHandler;
	RequestHandler.BindRaw(this, &FUE_MCP_Server::HandleHttpRequest);
	
	// Bind the MCP endpoint
	HttpRouter->BindRoute(FHttpPath(TEXT("/mcp")), EHttpServerRequestVerbs::VERB_POST, RequestHandler);
	
	// Start the HTTP server
	HttpServerModule.StartAllListeners();
	
	bIsRunning = true;

	UE_LOG(LogTemp, Log, TEXT("MCP Server: Started HTTP server on http://localhost:%d/mcp"), ServerPort);
	UE_LOG(LogTemp, Log, TEXT("MCP Server: Ready to accept JSON-RPC requests via HTTP POST"));
	UE_LOG(LogTemp, Warning, TEXT("MCP Server: Using HTTP transport (WebSocket not available in UE)"));

	return true;
}

void FUE_MCP_Server::StopServer()
{
	if (!bIsRunning)
	{
		return;
	}
	
	// Unbind routes
	if (HttpRouter.IsValid())
	{
		// Routes are automatically unbound when router is destroyed
		HttpRouter.Reset();
	}
	
	// Stop HTTP server
	FHttpServerModule& HttpServerModule = FHttpServerModule::Get();
	HttpServerModule.StopAllListeners();

	bIsRunning = false;
	RegisteredTools.Empty();

	UE_LOG(LogTemp, Log, TEXT("MCP Server: Stopped"));
}

bool FUE_MCP_Server::HandleHttpRequest(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	UE_LOG(LogTemp, Log, TEXT("MCP Server: Received HTTP request"));
	
	// Get request body
	const TArray<uint8>& Body = Request.Body;
	UE_LOG(LogTemp, Log, TEXT("MCP Server: HTTP Body length: %d"), Body.Num());
	FString RequestJson;
	if (Body.Num() > 0)
	{
		RequestJson = FString(UTF8_TO_TCHAR(reinterpret_cast<const char*>(Body.GetData())));
		// Ensure we don't read past buffer if not null-terminated
		if (RequestJson.Len() > Body.Num())
		{
			RequestJson = RequestJson.Left(Body.Num());
		}
	}
	else
	{
		RequestJson = FString();
	}
	UE_LOG(LogTemp, Log, TEXT("MCP Server: HTTP Body content: %s"), *RequestJson);

	// Process the JSON-RPC request
	FString ResponseJson = HandleJsonRpcRequest(RequestJson);
	
	// Create HTTP response
	TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(ResponseJson, TEXT("application/json"));
	Response->Code = EHttpServerResponseCodes::Ok;
	
	// Add CORS headers
	Response->Headers.Add(TEXT("Access-Control-Allow-Origin"), {TEXT("*")});
	Response->Headers.Add(TEXT("Access-Control-Allow-Methods"), {TEXT("POST, OPTIONS")});
	Response->Headers.Add(TEXT("Access-Control-Allow-Headers"), {TEXT("Content-Type")});
	
	// Send response
	OnComplete(MoveTemp(Response));
	
	return true;
}

void FUE_MCP_Server::RegisterTool(const FString& ToolName, const FString& Description, FOnMCPToolExecute Callback)
{
	if (ToolName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("MCP Server: Cannot register tool with empty name"));
		return;
	}

	if (RegisteredTools.Contains(ToolName))
	{
		UE_LOG(LogTemp, Warning, TEXT("MCP Server: Tool '%s' is already registered, overwriting"), *ToolName);
	}

	FMCPTool NewTool;
	NewTool.Name = ToolName;
	NewTool.Description = Description;
	NewTool.Callback = Callback;

	RegisteredTools.Add(ToolName, NewTool);

	UE_LOG(LogTemp, Log, TEXT("MCP Server: Registered tool '%s' - %s"), *ToolName, *Description);
}

void FUE_MCP_Server::UnregisterTool(const FString& ToolName)
{
	if (RegisteredTools.Remove(ToolName) > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("MCP Server: Unregistered tool '%s'"), *ToolName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MCP Server: Tool '%s' was not registered"), *ToolName);
	}
}

void FUE_MCP_Server::SendNotification(const FString& Method, TSharedPtr<FJsonObject> Params)
{
	if (!bIsRunning)
	{
		UE_LOG(LogTemp, Warning, TEXT("MCP Server: Cannot send notification, server is not running"));
		return;
	}

	// Create JSON-RPC notification
	TSharedPtr<FJsonObject> NotificationObject = MakeShareable(new FJsonObject);
	NotificationObject->SetStringField(TEXT("jsonrpc"), TEXT("2.0"));
	NotificationObject->SetStringField(TEXT("method"), Method);

	if (Params.IsValid())
	{
		NotificationObject->SetObjectField(TEXT("params"), Params);
	}

	// Convert to string
	FString NotificationString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&NotificationString);
	FJsonSerializer::Serialize(NotificationObject.ToSharedRef(), Writer);

	UE_LOG(LogTemp, Log, TEXT("MCP Server: Sent notification - Method: %s"), *Method);
}

FString FUE_MCP_Server::HandleJsonRpcRequest(const FString& RequestJson)
{
	if (!bIsRunning)
	{
		TSharedPtr<FJsonObject> ErrorResponse = CreateErrorResponse(
			JSONRPC_INTERNAL_ERROR,
			TEXT("Server is not running"),
			MakeShareable(new FJsonValueNull())
		);

		FString ResponseString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ResponseString);
		FJsonSerializer::Serialize(ErrorResponse.ToSharedRef(), Writer);
		return ResponseString;
	}

	// Parse the incoming request
	TSharedPtr<FJsonObject> RequestObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(RequestJson);

	if (!FJsonSerializer::Deserialize(Reader, RequestObject) || !RequestObject.IsValid())
	{
		TSharedPtr<FJsonObject> ErrorResponse = CreateErrorResponse(
			JSONRPC_PARSE_ERROR,
			TEXT("Parse error"),
			MakeShareable(new FJsonValueNull())
		);

		FString ResponseString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ResponseString);
		FJsonSerializer::Serialize(ErrorResponse.ToSharedRef(), Writer);
		return ResponseString;
	}

	// Validate JSON-RPC request
	if (!RequestObject->HasField(TEXT("jsonrpc")) || 
		RequestObject->GetStringField(TEXT("jsonrpc")) != TEXT("2.0") ||
		!RequestObject->HasField(TEXT("method")))
	{
		TSharedPtr<FJsonValue> Id = RequestObject->HasField(TEXT("id")) 
			? RequestObject->TryGetField(TEXT("id"))
			: MakeShareable(new FJsonValueNull());

		TSharedPtr<FJsonObject> ErrorResponse = CreateErrorResponse(
			JSONRPC_INVALID_REQUEST,
			TEXT("Invalid Request"),
			Id
		);

		FString ResponseString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ResponseString);
		FJsonSerializer::Serialize(ErrorResponse.ToSharedRef(), Writer);
		return ResponseString;
	}

	// Extract method and parameters
	FString Method = RequestObject->GetStringField(TEXT("method"));
	TSharedPtr<FJsonObject> Params = RequestObject->HasField(TEXT("params")) 
		? RequestObject->GetObjectField(TEXT("params"))
		: MakeShareable(new FJsonObject());

	TSharedPtr<FJsonValue> Id = RequestObject->HasField(TEXT("id"))
		? RequestObject->TryGetField(TEXT("id"))
		: MakeShareable(new FJsonValueNull());

	// If no ID, this is a notification and we don't send a response
	if (Id->IsNull() || Id->Type == EJson::None)
	{
		ProcessMethodCall(Method, Params, Id);
		return FString();
	}

	// Process the method call
	TSharedPtr<FJsonObject> ResponseObject = ProcessMethodCall(Method, Params, Id);

	// Convert response to string
	FString ResponseString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ResponseString);
	FJsonSerializer::Serialize(ResponseObject.ToSharedRef(), Writer);

	return ResponseString;
}

TSharedPtr<FJsonObject> FUE_MCP_Server::ProcessMethodCall(const FString& Method, TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonValue> Id)
{
	UE_LOG(LogTemp, Log, TEXT("MCP Server: Processing method call '%s'"), *Method);

	// Handle built-in methods first
	if (Method == TEXT("initialize"))
	{
		TSharedPtr<FJsonObject> ResultObject = MakeShareable(new FJsonObject());
		ResultObject->SetStringField(TEXT("protocolVersion"), TEXT("2024-11-05"));
		ResultObject->SetStringField(TEXT("serverInfo"), TEXT("UE MCP Server"));

		TSharedPtr<FJsonObject> Capabilities = MakeShareable(new FJsonObject());
		Capabilities->SetBoolField(TEXT("tools"), true);
		ResultObject->SetObjectField(TEXT("capabilities"), Capabilities);

		return CreateSuccessResponse(ResultObject, Id);
	}
	else if (Method == TEXT("tools/list"))
	{
		TSharedPtr<FJsonObject> ResultObject = MakeShareable(new FJsonObject());
		TArray<TSharedPtr<FJsonValue>> ToolsArray;

		for (const auto& ToolPair : RegisteredTools)
		{
			TSharedPtr<FJsonObject> ToolObject = MakeShareable(new FJsonObject());
			ToolObject->SetStringField(TEXT("name"), ToolPair.Value.Name);
			ToolObject->SetStringField(TEXT("description"), ToolPair.Value.Description);

			ToolsArray.Add(MakeShareable(new FJsonValueObject(ToolObject)));
		}

		ResultObject->SetArrayField(TEXT("tools"), ToolsArray);
		return CreateSuccessResponse(ResultObject, Id);
	}

	// Check if this is a tool call
	if (Method.StartsWith(TEXT("tools/")))
	{
		FString ToolName = Method.RightChop(6); // Remove "tools/" prefix

		if (!RegisteredTools.Contains(ToolName))
		{
			return CreateErrorResponse(
				JSONRPC_METHOD_NOT_FOUND,
				FString::Printf(TEXT("Tool not found: %s"), *ToolName),
				Id
			);
		}

		// Execute the tool
		FMCPTool& Tool = RegisteredTools[ToolName];
		TSharedPtr<FJsonObject> ResultObject = MakeShareable(new FJsonObject());

		Tool.Callback.ExecuteIfBound(ToolName, Params, ResultObject);

		return CreateSuccessResponse(ResultObject, Id);
	}

	// Method not found
	return CreateErrorResponse(
		JSONRPC_METHOD_NOT_FOUND,
		FString::Printf(TEXT("Method not found: %s"), *Method),
		Id
	);
}

TSharedPtr<FJsonObject> FUE_MCP_Server::CreateErrorResponse(int32 Code, const FString& Message, TSharedPtr<FJsonValue> Id)
{
	TSharedPtr<FJsonObject> ErrorObject = MakeShareable(new FJsonObject());
	ErrorObject->SetNumberField(TEXT("code"), Code);
	ErrorObject->SetStringField(TEXT("message"), Message);

	TSharedPtr<FJsonObject> ResponseObject = MakeShareable(new FJsonObject());
	ResponseObject->SetStringField(TEXT("jsonrpc"), TEXT("2.0"));
	ResponseObject->SetObjectField(TEXT("error"), ErrorObject);
	ResponseObject->SetField(TEXT("id"), Id);

	UE_LOG(LogTemp, Warning, TEXT("MCP Server: Error response - Code: %d, Message: %s"), Code, *Message);

	return ResponseObject;
}

TSharedPtr<FJsonObject> FUE_MCP_Server::CreateSuccessResponse(TSharedPtr<FJsonObject> Result, TSharedPtr<FJsonValue> Id)
{
	TSharedPtr<FJsonObject> ResponseObject = MakeShareable(new FJsonObject());
	ResponseObject->SetStringField(TEXT("jsonrpc"), TEXT("2.0"));
	ResponseObject->SetObjectField(TEXT("result"), Result);
	ResponseObject->SetField(TEXT("id"), Id);

	return ResponseObject;
}
