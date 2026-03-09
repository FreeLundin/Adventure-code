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
        // if within charge range, increase speed and attempt to damage
        float Acceptance = 100.0f;
        float SpeedFactor = 1.0f;
        if (Dist < 300.0f)
        {
            Acceptance = 20.0f;
            SpeedFactor = 3.0f;
            // apply damage if we overlap the player
            if (Dist < 100.0f)
            {
                UGameplayStatics::ApplyDamage(PlayerChar, 10.0f, this, Controlled, nullptr);
            }
        }
        MoveToActor(PlayerChar, Acceptance, true, true, true, 0, true);
        if (ACharacter* Ch = Cast<ACharacter>(Controlled))
        {
            Ch->GetCharacterMovement()->MaxWalkSpeed = 400.f * SpeedFactor;
        }
    }
}