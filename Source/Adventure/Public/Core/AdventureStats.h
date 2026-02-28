#pragma once

#include "CoreMinimal.h"
#include "Core/AdventureMacros.h"
#include "AdventureStats.generated.h"

/**
 * Example data-only struct that is BlueprintType because of ADV_STRUCT.
 * The GENERATED_BODY macro is still required for reflection.
 */
ADV_STRUCT()
struct FAdventureStats
{
    GENERATED_BODY()

    ADV_PROP(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float Health;

    ADV_PROP(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float Stamina;
};
