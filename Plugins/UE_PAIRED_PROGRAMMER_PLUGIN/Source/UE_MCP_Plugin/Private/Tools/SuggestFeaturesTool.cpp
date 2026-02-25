#include "Tools/SuggestFeaturesTool.h"

bool FSuggestFeaturesTool::Execute(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult)
{
    FString Description;
    if (!Params->TryGetStringField(TEXT("description"), Description))
    {
        return false;
    }

    // dummy suggestion algorithm: echo plus generic ideas
    FString Suggestions = FString::Printf(
        TEXT("Based on your description '%s', consider:\n- Responsive combat mechanics\n- Crafting system\n- Dynamic weather\n- Skill tree progression"),
        *Description);

    OutResult = MakeShared<FJsonObject>();
    OutResult->SetStringField(TEXT("suggestions"), Suggestions);
    return true;
}

TSharedPtr<FJsonObject> FSuggestFeaturesTool::GetInputSchema()
{
    TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
    Schema->SetStringField(TEXT("type"), TEXT("object"));
    TSharedPtr<FJsonObject> Props = MakeShared<FJsonObject>();
    Props->SetStringField(TEXT("description"), TEXT("string"));
    Schema->SetObjectField(TEXT("properties"), Props);
    return Schema;
}
