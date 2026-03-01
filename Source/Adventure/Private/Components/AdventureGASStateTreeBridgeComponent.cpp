#include "Components/AdventureGASStateTreeBridgeComponent.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTagContainer.h"

UAdventureGASStateTreeBridgeComponent::UAdventureGASStateTreeBridgeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    Tag_TraversalActive = FGameplayTag::RequestGameplayTag(FName("State.TraversalActive"));
    Tag_TypeClimb = FGameplayTag::RequestGameplayTag(FName("Traversal.Type.Climb"));
    Tag_TypeMantle = FGameplayTag::RequestGameplayTag(FName("Traversal.Type.Mantle"));
    Tag_TypeSwimExit = FGameplayTag::RequestGameplayTag(FName("Traversal.Type.SwimExit"));
}

void UAdventureGASStateTreeBridgeComponent::BeginPlay()
{
    Super::BeginPlay();

    if (UAbilitySystemComponent *ASC = ResolveASC())
    {
        BindASCDelegates(ASC);

        // Initialize from current tag state
        bTraversalActive = ASC->HasMatchingGameplayTag(Tag_TraversalActive);
        if (ASC->HasMatchingGameplayTag(Tag_TypeClimb))
            ActiveTraversalType = Tag_TypeClimb;
        if (ASC->HasMatchingGameplayTag(Tag_TypeMantle))
            ActiveTraversalType = Tag_TypeMantle;
        if (ASC->HasMatchingGameplayTag(Tag_TypeSwimExit))
            ActiveTraversalType = Tag_TypeSwimExit;
    }
}

UAbilitySystemComponent *UAdventureGASStateTreeBridgeComponent::ResolveASC() const
{
    const AActor *Owner = GetOwner();
    if (!Owner)
        return nullptr;

    // If ASC is on PlayerState, try to obtain via pawn/controller first.
    const APlayerState *PS = nullptr;
    if (const APawn *Pawn = Cast<APawn>(Owner))
    {
        PS = Pawn->GetPlayerState();
    }
    else if (const AController *C = Cast<AController>(Owner))
    {
        PS = C->PlayerState;
    }

    if (PS)
    {
        return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(const_cast<APlayerState *>(PS));
    }

    // Fallback: try owner itself
    return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(const_cast<AActor *>(Owner));
}

void UAdventureGASStateTreeBridgeComponent::BindASCDelegates(UAbilitySystemComponent *ASC)
{
    if (!ASC)
        return;

    ASC->RegisterGameplayTagEvent(Tag_TraversalActive, EGameplayTagEventType::NewOrRemoved)
        .AddUObject(this, &UAdventureGASStateTreeBridgeComponent::OnTraversalActiveTagChanged);

    ASC->RegisterGameplayTagEvent(Tag_TypeClimb, EGameplayTagEventType::NewOrRemoved)
        .AddUObject(this, &UAdventureGASStateTreeBridgeComponent::OnClimbTypeTagChanged);

    ASC->RegisterGameplayTagEvent(Tag_TypeMantle, EGameplayTagEventType::NewOrRemoved)
        .AddUObject(this, &UAdventureGASStateTreeBridgeComponent::OnMantleTypeTagChanged);

    ASC->RegisterGameplayTagEvent(Tag_TypeSwimExit, EGameplayTagEventType::NewOrRemoved)
        .AddUObject(this, &UAdventureGASStateTreeBridgeComponent::OnSwimExitTypeTagChanged);
}

void UAdventureGASStateTreeBridgeComponent::OnTraversalActiveTagChanged(const FGameplayTag Tag, int32 NewCount)
{
    bTraversalActive = (NewCount > 0);

    // If traversal ended, clear type
    if (!bTraversalActive)
    {
        ActiveTraversalType = FGameplayTag();
    }
}

void UAdventureGASStateTreeBridgeComponent::OnClimbTypeTagChanged(const FGameplayTag Tag, int32 NewCount)
{
    if (NewCount > 0)
        ActiveTraversalType = Tag_TypeClimb;
}

void UAdventureGASStateTreeBridgeComponent::OnMantleTypeTagChanged(const FGameplayTag Tag, int32 NewCount)
{
    if (NewCount > 0)
        ActiveTraversalType = Tag_TypeMantle;
}

void UAdventureGASStateTreeBridgeComponent::OnSwimExitTypeTagChanged(const FGameplayTag Tag, int32 NewCount)
{
    if (NewCount > 0)
        ActiveTraversalType = Tag_TypeSwimExit;
}
