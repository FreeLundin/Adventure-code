#include "Misc/AutomationTest.h"
#include "Tools/BacklogTool.h"

#if WITH_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBacklogToolTest, "UE_MCP_Plugin.BacklogTool.Basic", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FBacklogToolTest::RunTest(const FString& Parameters)
{
    TSharedPtr<FJsonObject> Params = MakeShared<FJsonObject>();
    TSharedPtr<FJsonObject> Result;

    // ensure list works even if file doesn't exist yet
    Params->SetStringField(TEXT("action"), TEXT("list"));
    bool bSuccess = FBacklogTool::Execute(Params, Result);
    TestTrue(TEXT("Backlog list executes"), bSuccess);
    TestTrue(TEXT("Result object returned"), Result.IsValid());

    // add an item
    Params = MakeShared<FJsonObject>();
    Params->SetStringField(TEXT("action"), TEXT("add"));
    Params->SetStringField(TEXT("item"), TEXT("test entry"));
    bSuccess = FBacklogTool::Execute(Params, Result);
    TestTrue(TEXT("Backlog add executes"), bSuccess);

    // list again and verify previous item appears
    Params = MakeShared<FJsonObject>();
    Params->SetStringField(TEXT("action"), TEXT("list"));
    bSuccess = FBacklogTool::Execute(Params, Result);
    TestTrue(TEXT("Backlog list after add executes"), bSuccess);
    if (Result.IsValid())
    {
        TArray<TSharedPtr<FJsonValue>> Items = Result->GetArrayField(TEXT("items"));
        TestTrue(TEXT("Backlog contains at least one item"), Items.Num() > 0);
    }

    return true;
}

#endif // WITH_AUTOMATION_TESTS
