#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ArtifactData.generated.h"

class UGameplayAbility;

/**
 * Data asset describing a collectible artifact with passive and signature effects.
 * Designed for Phase 4: Crescent City Artifact ("Bayou Chalice").
 */
UCLASS(BlueprintType)
class ADVENTURE_API UArtifactData : public UDataAsset
{
    GENERATED_BODY()

public:
    // Friendly name for the artifact
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Artifact")
    FText ArtifactName;

    // Small heal amount applied when player hits an enemy while this artifact is equipped
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Artifact")
    float PassiveHealOnHit = 5.0f;

    // Ritual energy cost for the signature ability
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Artifact")
    float SignatureCost = 100.0f;

    // Gameplay ability to grant when signature is activated; optional
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Artifact")
    TSubclassOf<UGameplayAbility> SignatureAbility;
};