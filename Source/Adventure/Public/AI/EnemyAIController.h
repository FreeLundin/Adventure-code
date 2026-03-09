#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * Very simple AI controller that chases the player character when possessed pawn
 * has line of sight.  Intended as placeholder behaviour until StateTree is
 * integrated.
 */
UCLASS()
class ADVENTURE_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController();

    virtual void Tick(float DeltaSeconds) override;
};