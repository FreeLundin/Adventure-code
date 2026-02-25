#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * Simple dashboard showing registered MCP tools and a manual telemetry button.
 */
class SAgentDashboard : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SAgentDashboard) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    /** handle when user clicks "Refresh" */
    FReply OnRefreshClicked();
    /** handle when user clicks "Send Test Telemetry" */
    FReply OnSendTelemetryClicked();
    /** handle when user clicks "Create Example Blueprint" */
    FReply OnCreateExampleClicked();

    /** cached list of tool names */
    TArray<TSharedPtr<FString>> ToolNames;

    /** description entered by user for feature suggestion */
    FString FeatureDescription;

    /** suggestion result text */
    FString FeatureSuggestions;

    /** callback from editable box */
    void OnFeatureDescCommitted(const FText& Text, ETextCommit::Type CommitType);

    /** handle when suggest button clicked */
    FReply OnSuggestClicked();

    /** backlog text entered by user */
    FString BacklogEntry;

    /** current backlog items (cached) */
    TArray<TSharedPtr<FString>> BacklogItems;

    void OnBacklogCommitted(const FText& Text, ETextCommit::Type CommitType);
    FReply OnAddBacklogClicked();
    FReply OnRefreshBacklogClicked();
};
