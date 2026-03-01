#include "AdventureCharacterBase.h"

AAdventureCharacterBase::AAdventureCharacterBase()
{
    // create the GAS-StateTree bridge so designers don't have to add it manually
    GASStateTreeBridge = CreateDefaultSubobject<UAdventureGASStateTreeBridgeComponent>(TEXT("GASStateTreeBridge"));
}

void AAdventureCharacterBase::BeginPlay()
{
    Super::BeginPlay();

    // additional initialization can go here (binding to Mover events, etc.)
}
