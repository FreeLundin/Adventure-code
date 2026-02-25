#include "McpAgentSettings.h"

UMcpAgentSettings::UMcpAgentSettings()
{
    BaseUrl = TEXT("https://mcp.example.com/api");
    ApiKey = TEXT("");
    bAutoStart = true;
}
