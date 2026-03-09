#include "AI/EnemyBase.h"
#include "AI/EnemyAIController.h"

AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // use simple AI controller by default
    AIControllerClass = AEnemyAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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
