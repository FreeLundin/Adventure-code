#include "AdventureGameMode.h"
#include "AdventurePlayerController.h"
#include "AdventureCharacterBase.h"

AAdventureGameMode::AAdventureGameMode()
{
    PlayerControllerClass = AAdventurePlayerController::StaticClass();

    // use the C++ base character by default; blueprint child can override if needed
    DefaultPawnClass = AAdventureCharacterBase::StaticClass();

    // designers may still create a BP subclass (BP_AdventureCharacter) and set it here or in a child GameMode.
}
