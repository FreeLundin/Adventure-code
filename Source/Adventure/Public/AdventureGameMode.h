#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AdventureGameMode.generated.h"

UCLASS()
class ADVENTURE_API AAdventureGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    AAdventureGameMode();
};

UCLASS()
class ADVENTURE_API ASVGLNDGameMode : public AAdventureGameMode
{
    GENERATED_BODY()
};
