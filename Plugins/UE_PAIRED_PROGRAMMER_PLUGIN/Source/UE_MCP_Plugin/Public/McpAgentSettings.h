#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "McpAgentSettings.generated.h"

/**
 * Project settings for the MCP Agent plugin.  Editable under
 * Edit > Project Settings > MCP Agent.
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="MCP Agent"))
class UE_MCP_PLUGIN_API UMcpAgentSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UMcpAgentSettings();

    /** base URL for MPC server (no trailing slash) */
    UPROPERTY(config, EditAnywhere, Category="Connection")
    FString BaseUrl;

    /** API key/header value sent with each request */
    UPROPERTY(config, EditAnywhere, Category="Connection")
    FString ApiKey;

    /** if true start the server/connector automatically on plugin load */
    UPROPERTY(config, EditAnywhere, Category="Connection")
    bool bAutoStart;
};
