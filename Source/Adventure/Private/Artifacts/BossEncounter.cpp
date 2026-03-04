#include "Artifacts/BossEncounter.h"

ABossEncounter::ABossEncounter()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ABossEncounter::BeginPlay()
{
    Super::BeginPlay();
    // initial phase setup could go here
}

void ABossEncounter::AdvancePhase()
{
    CurrentPhase++;
    // logic for transitioning between phases would be implemented in Blueprint
}
