#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyBase.h"
#include "EnemyCultist.generated.h"

UENUM(BlueprintType)
enum class ECultistVariant : uint8
{
    Hexer,
    Marksman
};

/**
 * Generic cultist enemy that can be either a hexer (magic) or marksman (ranged).
 */
UCLASS()
class ADVENTURE_API AEnemyCultist : public AEnemyBase
{
    GENERATED_BODY()

public:
    AEnemyCultist();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy")
    ECultistVariant Variant = ECultistVariant::Hexer;
};