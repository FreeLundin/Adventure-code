#pragma once
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/AdventureInteractableInterface.h"
#include "Core/AdventureMacros.h"
#include "InteractableDummy.generated.h"

ADV_CLASS()
class ADVENTURE_API AInteractableDummy : public AActor, public IAdventureInteractableInterface
{
    GENERATED_BODY()

public:
    AInteractableDummy();

    /** Implementation of the interaction interface */
    virtual void Interact_Implementation(AActor* Instigator) override;

    /** A property editable in Blueprints to show that class-level exposure works */
    ADV_PROP(EditAnywhere, Category="Interactable")
    FString DummyText;
};
