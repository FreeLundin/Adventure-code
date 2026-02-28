#include "AdventureUHTModule.h"
#include "Misc/MessageDialog.h"
#include "UObject/ObjectMacros.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"

#define LOCTEXT_NAMESPACE "FAdventureUHTModule"

void FAdventureUHTModule::StartupModule()
{
    UE_LOG(LogTemp, Log, TEXT("AdventureUHT module started - running basic header validation."));

    // Simple fallback: scan source headers for UPROPERTY usages missing our macros
    // This runs when the plugin loads (e.g. during editor startup or UHT run).
    
    FString SourceDir = FPaths::ProjectDir() / TEXT("Source/Adventure");
    TArray<FString> Files;
    IFileManager::Get().FindFilesRecursive(Files, *SourceDir, TEXT("*.h"), true, false);

    for (const FString& File : Files)
    {
        FString Contents;
        if (FFileHelper::LoadFileToString(Contents, *File))
        {
            TArray<FString> Lines;
            Contents.ParseIntoArrayLines(Lines);
            for (int32 i = 0; i < Lines.Num(); ++i)
            {
                const FString& Line = Lines[i];
                if (Line.Contains(TEXT("UPROPERTY("))
                    && !Line.Contains(TEXT("ADV_PROP"))
                    && !Line.Contains(TEXT("ADV_PROP_RO")))
                {
                    UE_LOG(LogTemp, Warning, TEXT("[AdventureUHT] %s:%d - UPROPERTY missing ADV_PROP macros"), *File, i+1);
                }
            }
        }
    }
}

void FAdventureUHTModule::ShutdownModule()
{
    UE_LOG(LogTemp, Log, TEXT("AdventureUHT module shutting down."));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAdventureUHTModule, AdventureUHT)
