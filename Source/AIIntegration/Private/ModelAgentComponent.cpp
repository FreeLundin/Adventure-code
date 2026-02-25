#include "ModelAgentComponent.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Engine/Engine.h"

// NNE runtime headers (ensure plugin enabled)
#include "NNERuntime.h"
#include "NNECore.h"

UModelAgentComponent::UModelAgentComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentModelVersion = TEXT("none");
    LocalModelPath = FPaths::ProjectSavedDir() / TEXT("AIModels");
}

void UModelAgentComponent::BeginPlay()
{
    Super::BeginPlay();

    // If an asset has been assigned in the editor, create an NNE model instance now.
    if (PreLoadedModelData)
    {
        RuntimeModel = UNNEModel::CreateFromData(PreLoadedModelData);
        if (RuntimeModel)
        {
            ModelInstance = RuntimeModel->CreateInstance();
            UE_LOG(LogTemp, Log, TEXT("Initialized NNE model instance from PreLoadedModelData"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create runtime model from PreLoadedModelData"));
        }
    }
    // Optionally query MCP for latest stable model here.
}

void UModelAgentComponent::LoadModelFromUrl(const FString& ModelUrl)
{
    // NOTE: This is a skeleton. Replace with robust download, integrity checks, and NNE/ONNX import.
    UE_LOG(LogTemp, Log, TEXT("Model download requested: %s"), *ModelUrl);

    // Example: create directory
    IFileManager::Get().MakeDirectory(*LocalModelPath, true);

    // For production: use FHttpModule to download binary, verify checksum, then call NNE/ONNX plugin API to register/load.
    // Save placeholder path for later inference calls.
    LocalModelPath = FPaths::Combine(LocalModelPath, TEXT("downloaded_model.onnx"));
    CurrentModelVersion = TEXT("v0-placeholder");
}

bool UModelAgentComponent::RunInference(const TArray<float>& Input, TArray<float>& Output)
{
    if (Input.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("RunInference called with empty input"));
        return false;
    }

    if (ModelInstance.IsValid())
    {
        // Example usage - real API may differ depending on NNE version
        // Set first input tensor memory
        ModelInstance->SetInput(0, Input.GetData(), Input.Num());
        if (!ModelInstance->RunInference())
        {
            UE_LOG(LogTemp, Error, TEXT("ModelInstance inference failed"));
            return false;
        }
        // read output (assuming single output of same size)
        int32 OutSize = ModelInstance->GetOutputTensorSize(0);
        Output.SetNum(OutSize);
        ModelInstance->GetOutput(0, Output.GetData(), OutSize);
        return true;
    }

    // Fallback deterministic behavior: echo input if no model available
    Output = Input;
    UE_LOG(LogTemp, Log, TEXT("RunInference fallback executed; input echoed to output."));
    return true;
}

void UModelAgentComponent::ReportTelemetry(const FString& PayloadJson)
{
    const FString MCPUrl = TEXT("https://mcp.example.com/api/models/metrics");
    PostToMCP(MCPUrl, PayloadJson);
}

void UModelAgentComponent::PostToMCP(const FString& Url, const FString& JsonPayload)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(JsonPayload);

    Request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bWasSuccessful)
    {
        if (bWasSuccessful && Resp.IsValid())
        {
            UE_LOG(LogTemp, Log, TEXT("MCP Response: %s"), *Resp->GetContentAsString());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("MCP request failed"));
        }
    });

    Request->ProcessRequest();
}