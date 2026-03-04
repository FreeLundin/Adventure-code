#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossEncounter.generated.h"

UCLASS()
class ADVENTURE_API ABossEncounter : public AActor
{
    GENERATED_BODY()

public:
    ABossEncounter();

    virtual void BeginPlay() override;

    // placeholder for phase management
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss")
    int32 CurrentPhase = 1;

    UFUNCTION(BlueprintCallable, Category="Boss")
    void AdvancePhase();
};