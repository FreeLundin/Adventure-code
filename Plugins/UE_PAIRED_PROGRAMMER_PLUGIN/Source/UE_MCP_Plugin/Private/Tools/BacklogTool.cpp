#include "Tools/BacklogTool.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

static FString GetBacklogPath()
{
    return FPaths::ProjectSavedDir() / TEXT("MCPBacklog.json");
}

bool FBacklogTool::Execute(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject>& OutResult)
{
    // determine action
    FString Action;
    if (!Params->TryGetStringField(TEXT("action"), Action))
    {
        return false;
    }
    TArray<FString> Items;
    const FString Path = GetBacklogPath();

    // load existing items if file exists
    FString FileContents;
    if (FPaths::FileExists(Path) && FFileHelper::LoadFileToString(FileContents, *Path))
    {
        TSharedPtr<FJsonValue> JsonValue;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContents);
        if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid())
        {
            if (JsonValue->Type == EJson::Array)
            {
                for (auto& Val : JsonValue->AsArray())
                {
                    if (Val.IsValid() && Val->Type == EJson::String)
                    {
                        Items.Add(Val->AsString());
                    }
                }
            }
        }
    }

    if (Action == TEXT("add"))
    {
        FString Item;
        if (!Params->TryGetStringField(TEXT("item"), Item))
        {
            return false;
        }
        Items.Add(Item);
        // write back
        TArray<TSharedPtr<FJsonValue>> Arr;
        for (auto& It : Items)
        {
            Arr.Add(MakeShared<FJsonValueString>(It));
        }
        FileContents.Empty();
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&FileContents);
        FJsonSerializer::Serialize(Arr, Writer);
        FFileHelper::SaveStringToFile(FileContents, *Path);
        OutResult = MakeShared<FJsonObject>();
        OutResult->SetStringField(TEXT("status"), TEXT("ok"));
        return true;
    }
    else if (Action == TEXT("list"))
    {
        OutResult = MakeShared<FJsonObject>();
        TArray<TSharedPtr<FJsonValue>> JsonArr;
        for (auto& It : Items)
        {
            JsonArr.Add(MakeShared<FJsonValueString>(It));
        }
        OutResult->SetArrayField(TEXT("items"), JsonArr);
        return true;
    }

    return false;
}

TSharedPtr<FJsonObject> FBacklogTool::GetInputSchema()
{
    TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
    Schema->SetStringField(TEXT("type"), TEXT("object"));
    TSharedPtr<FJsonObject> Props = MakeShared<FJsonObject>();
    // action must be add or list
    TSharedPtr<FJsonObject> ActionSchema = MakeShared<FJsonObject>();
    ActionSchema->SetStringField(TEXT("type"), TEXT("string"));
    ActionSchema->SetStringField(TEXT("enum"), TEXT("[\"add\",\"list\"]"));
    Props->SetObjectField(TEXT("action"), ActionSchema);
    Props->SetStringField(TEXT("item"), TEXT("string (required for add)"));
    Schema->SetObjectField(TEXT("properties"), Props);
    return Schema;
}