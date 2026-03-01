#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/AdventureMacros.h"
#include "AdventureInteractableInterface.generated.h"

UINTERFACE(Blueprintable)
class ADVENTURE_API UAdventureInteractableInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface that marks an object as interactable by the player or AI.
 */
class ADVENTURE_API IAdventureInteractableInterface
{
    GENERATED_BODY()

public:
    /**
     * Called when an actor (usually the player) interacts with this object.
     * Default behavior is no-op; override in blueprint or C++.
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    void Interact(AActor* InteractingActor);
};
