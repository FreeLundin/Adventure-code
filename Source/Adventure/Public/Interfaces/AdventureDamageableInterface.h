#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/AdventureMacros.h"
#include "AdventureDamageableInterface.generated.h"

ADV_INTERFACE()
class ADVENTURE_API UAdventureDamageableInterface : public UInterface
{
    GENERATED_BODY()
};

class ADVENTURE_API IAdventureDamageableInterface
{
    GENERATED_BODY()

public:
    /**
     * Applies damage to the implementing object.
     * @param Amount amount of damage to apply
     * @param Instigator actor causing the damage
     */
    ADV_INTERFACE_FUNC(void)(float Amount, AActor* Instigator);
};
