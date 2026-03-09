#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyBase.h"
#include "EnemyMutantRusher.generated.h"

/**
 * Fast melee enemy that charges the player when in range.
 * Placeholder implementation; behaviour will be implemented in StateTree or C++.
 */
UCLASS()
class ADVENTURE_API AEnemyMutantRusher : public AEnemyBase
{
    GENERATED_BODY()

public:
    AEnemyMutantRusher();
};