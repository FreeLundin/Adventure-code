#include "AI/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;
    bAttachToPawn = true;
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    APawn* Controlled = GetPawn();
    if (!Controlled)
        return;

    ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (PlayerChar)
    {
        const float Dist = FVector::Dist(Controlled->GetActorLocation(), PlayerChar->GetActorLocation());
        // if very close, dash faster
        float Acceptance = 100.0f;
        float SpeedFactor = 1.0f;
        if (Dist < 500.0f)
        {
            Acceptance = 50.0f;
            SpeedFactor = 2.0f;
        }
        MoveToActor(PlayerChar, Acceptance, true, true, true, 0, true);
        if (ACharacter* Ch = Cast<ACharacter>(Controlled))
        {
            Ch->GetCharacterMovement()->MaxWalkSpeed = 400.f * SpeedFactor;
        }
    }
}