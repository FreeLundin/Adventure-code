#include "AI/EnemyCultist.h"

AEnemyCultist::AEnemyCultist()
{
    Health = 75.f;
    // default movement speed
    GetCharacterMovement()->MaxWalkSpeed = 400.f;
}
