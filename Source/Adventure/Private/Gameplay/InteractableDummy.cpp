#include "Gameplay/InteractableDummy.h"
#include "Engine/Engine.h"

AInteractableDummy::AInteractableDummy()
{
    DummyText = TEXT("Hello world");
}

void AInteractableDummy::Interact_Implementation(AActor* InteractingActor)
{
    if (GEngine && InteractingActor)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
            FString::Printf(TEXT("%s interacted with me!"), *InteractingActor->GetName()));
    }
}
