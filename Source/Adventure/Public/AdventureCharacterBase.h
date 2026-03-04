#pragma once

#include "CoreMinimal.h"
#include "AdventureMoverPawnBase.h"
#include "AdventureGASStateTreeBridgeComponent.h"
#include "AdventureCharacterBase.generated.h"

UCLASS()
class ADVENTURE_API AAdventureCharacterBase : public AAdventureMoverPawnBase
{
    GENERATED_BODY()

public:
    AAdventureCharacterBase();

    virtual void BeginPlay() override;

protected:
    // GAS-StateTree bridge
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Adventure|Components")
    UAdventureGASStateTreeBridgeComponent *GASStateTreeBridge;
};

UCLASS()
class ADVENTURE_API ASVGLNDCharacterBase : public AAdventureCharacterBase
{
    GENERATED_BODY()
};
