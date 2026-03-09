#include "AI/EnemySpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

AEnemySpawner::AEnemySpawner()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::BeginPlay()
{
    Super::BeginPlay();
    TimeSinceLast = SpawnInterval; // spawn immediately if possible
}

void AEnemySpawner::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!EnemyClass)
        return;

    TimeSinceLast += DeltaSeconds;
    if (TimeSinceLast >= SpawnInterval)
    {
        TimeSinceLast = 0.f;
        FVector Origin = GetActorLocation();
        FVector RandPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, FVector(SpawnRadius));
        FActorSpawnParameters Params;
        GetWorld()->SpawnActor<APawn>(EnemyClass, RandPoint, FRotator::ZeroRotator, Params);
    }
}