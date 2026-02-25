#include "McpTelemetryLibrary.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/ConfigCacheIni.h"
#include "Engine/World.h"
#include "TimerManager.h"

FMcpTelemetryLibrary::FOnTelemetrySent UMcpTelemetryLibrary::OnTelemetrySent;

void UMcpTelemetryLibrary::ReportTelemetry(const FString& PayloadJson)
{
    FString BaseUrl;
    FString ApiKey;

    // read from project settings class for nicer UX
    if (const UMcpAgentSettings* Settings = GetDefault<UMcpAgentSettings>())
    {
        BaseUrl = Settings->BaseUrl;
        ApiKey = Settings->ApiKey;
    }
    if (BaseUrl.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("McpTelemetryLibrary: BaseUrl not configured in project settings"));
    }

    if (BaseUrl.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("McpTelemetryLibrary: telemetry BaseUrl is empty; payload queued"));
        // queue for later retry
        static TArray<FString> Pending;
        Pending.Add(PayloadJson);
        OnTelemetrySent.Broadcast(false);
        return;
    }

    // construct full telemetry endpoint
    FString Url = BaseUrl;
    if (!Url.EndsWith("/"))
    {
        Url += TEXT("/");
    }
    Url += TEXT("telemetry");

    auto SendRequest = [&](const FString& Body, int32 Attempt) -> void
    {
        TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
        Request->SetURL(Url);
        Request->SetVerb(TEXT("POST"));
        Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
        if (!ApiKey.IsEmpty())
        {
            Request->SetHeader(TEXT("x-api-key"), ApiKey);
        }
        Request->SetContentAsString(Body);

        Request->OnProcessRequestComplete().BindLambda([Body, Attempt](FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bSuccess)
        {
            bool bWasOk = bSuccess && Resp.IsValid() && EHttpResponseCodes::IsOk(Resp->GetResponseCode());
            if (!bWasOk && Attempt < 3)
            {
                // simple backoff: delay = 2^Attempt seconds
                FTimerHandle Timer;
                GWorld->GetTimerManager().SetTimer(Timer, [Body, Attempt]() { UMcpTelemetryLibrary::ReportTelemetry(Body); }, FMath::Pow(2.0f, Attempt), false);
            }
            OnTelemetrySent.Broadcast(bWasOk);
        });

        Request->ProcessRequest();
    };

    // immediately send
    SendRequest(PayloadJson, 0);
}
