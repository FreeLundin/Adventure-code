#include "AI/EnemyBase.h"

AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBase::ReceiveDamage(float Amount)
{
    Health -= Amount;
    if (Health <= 0.f)
    {
        // simple death behaviour
        Destroy();
    }
}
