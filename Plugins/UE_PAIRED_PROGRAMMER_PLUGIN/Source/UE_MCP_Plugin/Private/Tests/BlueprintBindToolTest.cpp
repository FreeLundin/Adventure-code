#include "Misc/AutomationTest.h"
#include "Tools/BlueprintBindTool.h"

#if WITH_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintBindToolTest, "UE_MCP_Plugin.BlueprintBindTool.Basic", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FBlueprintBindToolTest::RunTest(const FString& Parameters)
{
    // missing/invalid parameters should cause failure
    TSharedPtr<FJsonObject> Params = MakeShared<FJsonObject>();
    Params->SetStringField(TEXT("inputAction"), TEXT("/Game/Nonexistent.Action"));
    Params->SetStringField(TEXT("abilityTag"), TEXT("Ability.Traversal"));
    TArray<TSharedPtr<FJsonValue>> Array;
    Params->SetArrayField(TEXT("blueprints"), Array);

    TSharedPtr<FJsonObject> Result;
    bool bSuccess = FBlueprintBindTool::Execute(Params, Result);
    TestFalse(TEXT("BlueprintBindTool should fail with invalid input"), bSuccess);

    return true;
}

#endif // WITH_AUTOMATION_TESTS
