#include "UI/AdventureHUDComponent.h"
#include "GAS/AdventureAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemInterface.h"

UAdventureHUDComponent::UAdventureHUDComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAdventureHUDComponent::BeginPlay()
{
    Super::BeginPlay();

    if (HUDClass)
    {
        APlayerController *PC = Cast<APlayerController>(GetOwner());
        if (PC)
        {
            HUDWidget = CreateWidget<UAdventureHUDWidget>(PC, HUDClass);
            if (HUDWidget)
            {
                HUDWidget->AddToViewport();
            }
        }
    }

    if (UAbilitySystemComponent *ASC = GetASC())
    {
        ASC->GetGameplayAttributeValueChangeDelegate(
               UAdventureAttributeSet::GetHealthAttribute())
            .AddUObject(this, &UAdventureHUDComponent::OnHealthChanged);

        ASC->GetGameplayAttributeValueChangeDelegate(
               UAdventureAttributeSet::GetStaminaAttribute())
            .AddUObject(this, &UAdventureHUDComponent::OnStaminaChanged);

        ASC->GetGameplayAttributeValueChangeDelegate(
               UAdventureAttributeSet::GetRitualEnergyAttribute())
            .AddUObject(this, &UAdventureHUDComponent::OnMagicChanged);

        ASC->GetGameplayAttributeValueChangeDelegate(
               UAdventureAttributeSet::GetRageAttribute())
            .AddUObject(this, &UAdventureHUDComponent::OnRageChanged);

        ASC->GetGameplayAttributeValueChangeDelegate(
               UAdventureAttributeSet::GetShieldAttribute())
            .AddUObject(this, &UAdventureHUDComponent::OnShieldChanged);
    }
}

const UAdventureAttributeSet *UAdventureHUDComponent::GetAttributes() const
{
    if (UAbilitySystemComponent *ASC = GetASC())
    {
        return ASC->GetSet<UAdventureAttributeSet>();
    }
    return nullptr;
}

UAbilitySystemComponent *UAdventureHUDComponent::GetASC() const
{
    if (AActor *Owner = GetOwner())
    {
        IAbilitySystemInterface *ASCInterface = Cast<IAbilitySystemInterface>(Owner);
        if (ASCInterface)
        {
            return ASCInterface->GetAbilitySystemComponent();
        }
    }
    return nullptr;
}

void UAdventureHUDComponent::OnHealthChanged(const FOnAttributeChangeData &Data)
{
    if (HUDWidget)
    {
        if (UAdventureAttributeSet *Attr = GetAttributes())
        {
            HUDWidget->SetHealth(Data.NewValue, Attr->GetMaxHealth());
        }
    }
}

void UAdventureHUDComponent::OnStaminaChanged(const FOnAttributeChangeData &Data)
{
    if (HUDWidget)
    {
        if (UAdventureAttributeSet *Attr = GetAttributes())
        {
            HUDWidget->SetStamina(Data.NewValue, Attr->GetMaxStamina());
        }
    }
}

void UAdventureHUDComponent::OnMagicChanged(const FOnAttributeChangeData &Data)
{
    if (HUDWidget)
    {
        HUDWidget->SetRitualEnergy(Data.NewValue);
    }
}

void UAdventureHUDComponent::OnRageChanged(const FOnAttributeChangeData &Data)
{
    if (HUDWidget)
    {
        if (UAdventureAttributeSet *Attr = GetAttributes())
        {
            HUDWidget->SetRage(Data.NewValue, Attr->GetMaxRage());
        }
    }
}

void UAdventureHUDComponent::OnShieldChanged(const FOnAttributeChangeData &Data)
{
    if (HUDWidget)
    {
        if (UAdventureAttributeSet *Attr = GetAttributes())
        {
            HUDWidget->SetShield(Data.NewValue, Attr->GetMaxShield());
        }
    }
}