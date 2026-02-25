#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ModelAgentComponent.generated.h"

/**
 * UModelAgentComponent
 * - Lightweight AAA-studio-ready skeleton for loading models, running inference via NNE/ONNX, and reporting telemetry to an MCP.
 * - Implement actual model loading and NNE/ONNX calls in your studio's runtime plugin integration.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AIINTEGRATION_API UModelAgentComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UModelAgentComponent();

protected:
    virtual void BeginPlay() override;

public:
    /** Optional model asset that can be assigned in the editor. */
    UPROPERTY(EditAnywhere, Category="AI|Model")
    TObjectPtr<class UNNEModelData> PreLoadedModelData;

    /** Load model artifact from a remote URL or local path. Saves model to Saved/AIModels/<name> and prepares runtime. */
    UFUNCTION(BlueprintCallable, Category="AI|Model")
    void LoadModelFromUrl(const FString& ModelUrl);

    /** Run inference synchronously. Returns true on success; Output is filled with floats. */
    UFUNCTION(BlueprintCallable, Category="AI|Model")
    bool RunInference(const TArray<float>& Input, TArray<float>& Output);

    /** Report telemetry JSON to MCP endpoint (non-blocking). */
    UFUNCTION(BlueprintCallable, Category="AI|Telemetry")
    void ReportTelemetry(const FString& PayloadJson);

    /** Current loaded model version tag (ModelCard.version). */
    UPROPERTY(BlueprintReadOnly, Category="AI|Model")
    FString CurrentModelVersion;

private:
    /** Internal helper to POST JSON to MCP. */
    void PostToMCP(const FString& Url, const FString& JsonPayload);

    /** Internal: path to downloaded model artifact. */
    FString LocalModelPath;

    // runtime handles created from NNE
    class UNNEModel* RuntimeModel = nullptr;
    TUniquePtr<class UNNEModelInstance> ModelInstance;
};