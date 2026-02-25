#include "MCPTools/BlueprintBindTool.h"
#include "K2Node_AddInputActionEvent.h"
#include "K2Node_CallFunction.h"
#include "K2EditorUtilities.h"
#include "EdGraphSchema_K2.h"
#include "Engine/Blueprint.h"
#include "UObject/SoftObjectPath.h"

bool FBlueprintBindTool::Execute(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult)
{
    const FString InputActionPath = Params->GetStringField(TEXT("inputAction"));
    FString AbilityTag = Params->GetStringField(TEXT("abilityTag"));

    TArray<TSharedPtr<FJsonValue>>* Blueprints;
    if (!Params->TryGetArrayField(TEXT("blueprints"), Blueprints))
    {
        return false;
    }

    UInputAction* InputAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, *InputActionPath));
    if (!InputAction)
    {
        return false;
    }

    for (auto& Val : *Blueprints)
    {
        FString Path = Val->AsString();
        UBlueprint* BP = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), nullptr, *Path));
        if (!BP)
        {
            continue;
        }

        UEdGraph* Graph = FK2EditorUtilities::FindEventGraph(BP);
        if (!Graph)
        {
            continue;
        }

        UEdGraphSchema_K2* Schema = Cast<UEdGraphSchema_K2>(Graph->GetSchema());
        if (!Schema)
        {
            continue;
        }

        // create input event
        UK2Node_AddInputActionEvent* InputNode = NewObject<UK2Node_AddInputActionEvent>(Graph);
        InputNode->InputAction = InputAction;
        FK2EditorUtilities::AddNode(Graph, InputNode, /*bSelectNewNode=*/false);

        // create call function node
        UK2Node_CallFunction* CallNode = NewObject<UK2Node_CallFunction>(Graph);
        // set the call to the character/controller function TryActivateAbilityByTag
        CallNode->FunctionReference.SetExternalMember(GET_FUNCTION_NAME_CHECKED(APC_AdventureController, TryActivateAbilityByTag), APC_AdventureController::StaticClass());
        FK2EditorUtilities::AddNode(Graph, CallNode, /*bSelectNewNode=*/false);

        // set tag default value
        if (UEdGraphPin* TagPin = CallNode->FindPin(TEXT("Tag")))
        {
            TagPin->DefaultValue = AbilityTag;
        }

        // connect pins
        Schema->TryCreateConnection(InputNode->FindPin(TEXT("Pressed")), CallNode->GetExecPin());

        BP->Modify();
        BP->MarkPackageDirty();
    }

    return true;
}

TSharedPtr<FJsonObject> FBlueprintBindTool::GetInputSchema()
{
    TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
    // simple schema describing required fields
    Schema->SetStringField(TEXT("type"), TEXT("object"));
    TSharedPtr<FJsonObject> props = MakeShared<FJsonObject>();
    props->SetObjectField(TEXT("blueprints"), MakeShared<FJsonObject>());
    props->SetStringField(TEXT("inputAction"), TEXT("string"));
    props->SetStringField(TEXT("abilityTag"), TEXT("string"));
    Schema->SetObjectField(TEXT("properties"), props);
    return Schema;
}
