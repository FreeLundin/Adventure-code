#include "Gameplay/AdventureBaseAbility.h"

UAdventureBaseAbility::UAdventureBaseAbility()
{
    RitualCost = 0.0f;
}

void UAdventureBaseAbility::OnActivate_Implementation()
{
    // default behaviour: nothing.  Derived classes can call Super...
}
