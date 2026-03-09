#include "AI/EnemyMutantRusher.h"

AEnemyMutantRusher::AEnemyMutantRusher()
{
    // adjust defaults
    Health = 50.f;
    GetCharacterMovement()->MaxWalkSpeed = 600.f;
}
