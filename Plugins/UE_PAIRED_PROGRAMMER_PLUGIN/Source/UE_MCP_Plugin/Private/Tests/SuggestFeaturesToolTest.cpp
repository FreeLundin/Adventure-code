#include "Misc/AutomationTest.h"
#include "Tools/SuggestFeaturesTool.h"

#if WITH_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSuggestFeaturesToolTest, "UE_MCP_Plugin.SuggestFeaturesTool.Basic", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FSuggestFeaturesToolTest::RunTest(const FString& Parameters)
{
    TSharedPtr<FJsonObject> Params = MakeShared<FJsonObject>();
    Params->SetStringField(TEXT("description"), TEXT("A simple test game description"));

    TSharedPtr<FJsonObject> Result;
    bool bSuccess = FSuggestFeaturesTool::Execute(Params, Result);

    TestTrue(TEXT("SuggestFeatures tool executed"), bSuccess);
    TestTrue(TEXT("Result object returned"), Result.IsValid());
    if (Result.IsValid())
    {
        TestTrue(TEXT("Result contains suggestions field"), Result->HasField(TEXT("suggestions")));
        FString Out = Result->GetStringField(TEXT("suggestions"));
        TestTrue(TEXT("Suggestions text is non-empty"), !Out.IsEmpty());
    }

    return true;
}

#endif // WITH_AUTOMATION_TESTS
