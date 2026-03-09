#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

/**
 * A very simple base class for enemies.  Specific archetypes should derive
 * from this and override behaviour; use the AIController/StateTree system
 * to implement movement and attack logic.
 */
UCLASS(Blueprintable)
class ADVENTURE_API AEnemyBase : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemyBase();

    // basic health property
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy")
    float Health = 100.f;

    // Called when the enemy takes damage.
    UFUNCTION(BlueprintCallable, Category="Enemy")
    virtual void ReceiveDamage(float Amount);
};