#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

/**
 * Spawns a configured enemy class periodically within a radius.
 */
UCLASS()
class ADVENTURE_API AEnemySpawner : public AActor
{
    GENERATED_BODY()

public:
    AEnemySpawner();

    virtual void Tick(float DeltaSeconds) override;

    /** Enemy class to spawn */
    UPROPERTY(EditAnywhere, Category="Spawner")
    TSubclassOf<APawn> EnemyClass;

    /** Seconds between spawns */
    UPROPERTY(EditAnywhere, Category="Spawner")
    float SpawnInterval = 5.f;

    /** Radius around this actor where enemies may appear */
    UPROPERTY(EditAnywhere, Category="Spawner")
    float SpawnRadius = 1000.f;

protected:
    virtual void BeginPlay() override;

private:
    float TimeSinceLast = 0.f;
};