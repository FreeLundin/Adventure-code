#include "AgentDashboard.h"
#include "ToolRegistry.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"
#include "Tools/SuggestFeaturesTool.h"

void SAgentDashboard::Construct(const FArguments& InArgs)
{
    // populate initial tool list
    if (FToolRegistry* Reg = FToolRegistry::Get())
    {
        TArray<FString> Names = Reg->GetToolNames();
        for (auto& Name : Names)
        {
            ToolNames.Add(MakeShared<FString>(Name));
        }
    }
    // also populate backlog view
    OnRefreshBacklogClicked();

    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(2)
        [
            SNew(STextBlock)
            .AutoWrapText(true)
            .Text(FText::FromString("Welcome to the MCP Agent Dashboard!\n" \
                                   "No coding or Blueprints required – use the buttons below to " \
                                   "prototype features, send telemetry, or ask for design ideas. \n" \
                                   "Think of the agent as your paired programmer."))
        ]

        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(2)
        [
            SNew(STextBlock)
            .Text_Lambda([this]() {
                bool bRunning = false;
                if (FUE_MCP_PluginModule* Mod = FUE_MCP_PluginModule::Get())
                {
                    bRunning = (Mod->GetMCPServer() != nullptr);
                }
                return FText::FromString(bRunning ? TEXT("MCP Server: running") : TEXT("MCP Server: stopped"));
            })
        ]

        + SVerticalBox::Slot()
        .FillHeight(1.0f)
        [
            SNew(SBorder)
            .Padding(8)
            .BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
            [
                SNew(SVerticalBox)

            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(2)
            [
                SNew(SButton)
                .Text(FText::FromString("Refresh Tools"))
                .OnClicked(this, &SAgentDashboard::OnRefreshClicked)
            ]

            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(SButton)
                    .Text(FText::FromString("Send Test Telemetry"))
                    .OnClicked(this, &SAgentDashboard::OnSendTelemetryClicked)
                ]
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(4,0)
                [
                    SNew(SButton)
                    .Text(FText::FromString("Create Example Blueprint"))
                    .OnClicked(this, &SAgentDashboard::OnCreateExampleClicked)
                ]
            ]

            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .FillWidth(1.0f)
                [
                    SNew(SEditableTextBox)
                    .HintText(FText::FromString("Describe your game (100 words or less)..."))
                    .OnTextCommitted(this, &SAgentDashboard::OnFeatureDescCommitted)
                ]
                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(SButton)
                    .Text(FText::FromString("Suggest Features"))
                    .OnClicked(this, &SAgentDashboard::OnSuggestClicked)
                ]
            ]

            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .FillWidth(1.0f)
                [
                    SNew(SEditableTextBox)
                    .HintText(FText::FromString("Add backlog item..."))
                    .OnTextCommitted(this, &SAgentDashboard::OnBacklogCommitted)
                ]
                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(SButton)
                    .Text(FText::FromString("Add to Backlog"))
                    .OnClicked(this, &SAgentDashboard::OnAddBacklogClicked)
                ]
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(4,0)
                [
                    SNew(SButton)
                    .Text(FText::FromString("Refresh Backlog"))
                    .OnClicked(this, &SAgentDashboard::OnRefreshBacklogClicked)
                ]
            ]

            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(2)
            [
                SNew(STextBlock)
                .Text(FText::FromString("Backlog items (refresh after adding):"))
            ]

            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(2)
            [
                SNew(SListView<TSharedPtr<FString>>)
                .ListItemsSource(&BacklogItems)
                .OnGenerateRow_Lambda([](TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& Owner)
                {
                    return SNew(STableRow<TSharedPtr<FString>>, Owner)
                        [ SNew(STextBlock).Text(FText::FromString(*Item)) ];
                })
            ]

            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(2)
            [
                SNew(STextBlock)
                .AutoWrapText(true)
                .Text_Lambda([this]() { return FText::FromString(FeatureSuggestions); })
            ]

            + SVerticalBox::Slot()
            .FillHeight(1.0f)
            .Padding(2)
            [
                SNew(SListView<TSharedPtr<FString>>)
                .ListItemsSource(&ToolNames)
                .OnGenerateRow_Lambda([](TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& Owner)
                {
                    return SNew(STableRow<TSharedPtr<FString>>, Owner)
                        [ SNew(STextBlock).Text(FText::FromString(*Item)) ];
                })
            ]
        ]
    ];
}

FReply SAgentDashboard::OnRefreshClicked()
{
    ToolNames.Empty();
    if (FToolRegistry* Reg = FToolRegistry::Get())
    {
        TArray<FString> Names = Reg->GetToolNames();
        for (auto& Name : Names)
        {
            ToolNames.Add(MakeShared<FString>(Name));
        }
    }
    return FReply::Handled();
}

FReply SAgentDashboard::OnSendTelemetryClicked()
{
    // fire an example telemetry event
    UMcpTelemetryLibrary::ReportTelemetry(TEXT("{\"example\":true}"));
    return FReply::Handled();
}

void SAgentDashboard::OnFeatureDescCommitted(const FText& Text, ETextCommit::Type CommitType)
{
    FeatureDescription = Text.ToString();
}

void SAgentDashboard::OnBacklogCommitted(const FText& Text, ETextCommit::Type CommitType)
{
    BacklogEntry = Text.ToString();
}

FReply SAgentDashboard::OnAddBacklogClicked()
{
    if (BacklogEntry.IsEmpty())
    {
        return FReply::Handled();
    }
    TSharedPtr<FJsonObject> Params = MakeShared<FJsonObject>();
    Params->SetStringField(TEXT("action"), TEXT("add"));
    Params->SetStringField(TEXT("item"), BacklogEntry);
    TSharedPtr<FJsonObject> Result;
    if (FToolRegistry* Reg = FToolRegistry::Get())
    {
        Reg->ExecuteTool(FBacklogTool::GetName(), Params, Result);
    }
    BacklogEntry.Empty();
    OnRefreshBacklogClicked();
    return FReply::Handled();
}

FReply SAgentDashboard::OnRefreshBacklogClicked()
{
    BacklogItems.Empty();
    TSharedPtr<FJsonObject> Params = MakeShared<FJsonObject>();
    Params->SetStringField(TEXT("action"), TEXT("list"));
    TSharedPtr<FJsonObject> Result;
    if (FToolRegistry* Reg = FToolRegistry::Get())
    {
        if (Reg->ExecuteTool(FBacklogTool::GetName(), Params, Result) && Result.IsValid())
        {
            TArray<TSharedPtr<FJsonValue>> Arr = Result->GetArrayField(TEXT("items"));
            for (auto& Val : Arr)
            {
                BacklogItems.Add(MakeShared<FString>(Val->AsString()));
            }
        }
    }
    return FReply::Handled();
}
FReply SAgentDashboard::OnSuggestClicked()
{
    if (FeatureDescription.IsEmpty())
    {
        FeatureSuggestions = TEXT("Please enter a brief description first.");
        return FReply::Handled();
    }

    // create params and execute tool
    TSharedPtr<FJsonObject> Params = MakeShared<FJsonObject>();
    Params->SetStringField(TEXT("description"), FeatureDescription);
    TSharedPtr<FJsonObject> Result;
    if (FToolRegistry* Reg = FToolRegistry::Get())
    {
        Reg->ExecuteTool(FSuggestFeaturesTool::GetName(), Params, Result);
        if (Result.IsValid())
        {
            FeatureSuggestions = Result->GetStringField(TEXT("suggestions"));
        }
    }
    return FReply::Handled();
}

FReply SAgentDashboard::OnCreateExampleClicked()
{
    // the onboard tool will produce a blueprint asset that demonstrates telemetry
    if (FToolRegistry* Reg = FToolRegistry::Get())
    {
        Reg->ExecuteTool(TEXT("onboard.createExample"), MakeShared<FJsonObject>());
    }
    return FReply::Handled();
}
