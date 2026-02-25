#include "Misc/AutomationTest.h"
#include "Tools/OnboardTool.h"
#include "Engine/Blueprint.h"

#if WITH_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOnboardToolTest, "UE_MCP_Plugin.OnboardTool.CreateExample", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FOnboardToolTest::RunTest(const FString& Parameters)
{
    TSharedPtr<FJsonObject> Params = MakeShared<FJsonObject>();
    TSharedPtr<FJsonObject> Result;
    bool bSuccess = FOnboardTool::Execute(Params, Result);

    TestTrue(TEXT("Onboard tool executed successfully"), bSuccess);
    TestTrue(TEXT("Result object returned"), Result.IsValid());
    if (Result.IsValid())
    {
        TestTrue(TEXT("Result contains path field"), Result->HasField(TEXT("path")));
        FString Path = Result->GetStringField(TEXT("path"));
        UBlueprint* BP = LoadObject<UBlueprint>(nullptr, *Path);
        TestTrue(TEXT("Generated blueprint asset can be loaded"), BP != nullptr);
    }

    return true;
}

#endif // WITH_AUTOMATION_TESTS
