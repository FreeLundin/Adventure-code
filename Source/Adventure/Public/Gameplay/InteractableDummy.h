#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/AdventureInteractableInterface.h"
#include "Adventure.h"
#include "InteractableDummy.generated.h"

UCLASS()
class ADVENTURE_API AInteractableDummy : public AActor, public IAdventureInteractableInterface
{
    GENERATED_BODY()

public:
    AInteractableDummy();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
    FString DummyText;

    // IAdventureInteractableInterface
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void Interact(AActor* InteractingActor);
    virtual void Interact_Implementation(AActor* InteractingActor) override;
};
