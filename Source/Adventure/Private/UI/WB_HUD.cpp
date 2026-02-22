// Copyright Epic Games, Inc. All Rights Reserved.
// HUD Widget for Adventure — Implementation

#include "UI/WB_HUD.h"
#include "Character/CBP_AdventureCharacter.h"
#include "Core/PC_AdventureController.h"
#include "Core/AdventurePlayerState.h"
#include "GAS/AdventureAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UWB_HUD::NativeConstruct()
{
	Super::NativeConstruct();

	// Widget is now added to viewport; safe to access player controller
	if (APC_AdventureController* OwnerController = Cast<APC_AdventureController>(GetOwningPlayer()))
	{
		InitializeHUD(OwnerController);
	}
}

void UWB_HUD::NativeDestruct()
{
	ShutdownHUD();
	Super::NativeDestruct();
}

void UWB_HUD::InitializeHUD(APC_AdventureController* OwnerController)
{
	if (!OwnerController)
	{
		return;
	}

	// Get player character
	CachedCharacter = Cast<ACBP_AdventureCharacter>(OwnerController->GetPawn());
	if (!CachedCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWB_HUD::InitializeHUD - No character found on player controller"));
		return;
	}

	// Get AttributeSet
	CachedAttributeSet = GetCharacterAttributeSet();
	if (!CachedAttributeSet)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWB_HUD::InitializeHUD - No AttributeSet found"));
		return;
	}

	// Bind to attribute callbacks
	BindToAttributeSet();

	// Initial refresh to populate bars with current values
	RefreshAllBars();

	UE_LOG(LogTemp, Log, TEXT("UWB_HUD::InitializeHUD - HUD initialized successfully"));
}

void UWB_HUD::ShutdownHUD()
{
	UnbindFromAttributeSet();
	CachedAttributeSet = nullptr;
	CachedCharacter = nullptr;
}

UAdventureAttributeSet* UWB_HUD::GetCharacterAttributeSet() const
{
	if (!CachedCharacter)
	{
		return nullptr;
	}

	// Get PlayerState ASC
	AAdventurePlayerState* PlayerState = Cast<AAdventurePlayerState>(CachedCharacter->GetPlayerState());
	if (!PlayerState || !PlayerState->GetAbilitySystemComponent())
	{
		return nullptr;
	}

	// Get AttributeSet from ASC
	return Cast<UAdventureAttributeSet>(PlayerState->GetAbilitySystemComponent()->GetAttributeSet(UAdventureAttributeSet::StaticClass()));
}

void UWB_HUD::BindToAttributeSet()
{
	if (!CachedAttributeSet)
	{
		return;
	}

	// Get ASC for delegate binding
	AAdventurePlayerState* PlayerState = Cast<AAdventurePlayerState>(CachedCharacter->GetPlayerState());
	if (!PlayerState || !PlayerState->GetAbilitySystemComponent())
	{
		return;
	}

	UAbilitySystemComponent* ASC = PlayerState->GetAbilitySystemComponent();

	// Bind to attribute change delegates
	// These are triggered whenever the attribute is modified
	ASC->GetGameplayAttributeValueChangeDelegate(CachedAttributeSet->GetHealthAttribute()).AddDynamic(this, &UWB_HUD::OnHealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(CachedAttributeSet->GetStaminaAttribute()).AddDynamic(this, &UWB_HUD::OnStaminaChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(CachedAttributeSet->GetRitualEnergyAttribute()).AddDynamic(this, &UWB_HUD::OnRitualEnergyChanged);

	UE_LOG(LogTemp, Log, TEXT("UWB_HUD::BindToAttributeSet - Bound to all attribute delegates"));
}

void UWB_HUD::UnbindFromAttributeSet()
{
	if (!CachedAttributeSet || !CachedCharacter)
	{
		return;
	}

	AAdventurePlayerState* PlayerState = Cast<AAdventurePlayerState>(CachedCharacter->GetPlayerState());
	if (!PlayerState || !PlayerState->GetAbilitySystemComponent())
	{
		return;
	}

	UAbilitySystemComponent* ASC = PlayerState->GetAbilitySystemComponent();

	// Unbind from all delegates
	ASC->GetGameplayAttributeValueChangeDelegate(CachedAttributeSet->GetHealthAttribute()).RemoveDynamic(this, &UWB_HUD::OnHealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(CachedAttributeSet->GetStaminaAttribute()).RemoveDynamic(this, &UWB_HUD::OnStaminaChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(CachedAttributeSet->GetRitualEnergyAttribute()).RemoveDynamic(this, &UWB_HUD::OnRitualEnergyChanged);

	UE_LOG(LogTemp, Log, TEXT("UWB_HUD::UnbindFromAttributeSet - Unbound from all attribute delegates"));
}

void UWB_HUD::OnHealthChanged(float NewHealth, float MaxHealth)
{
	if (bUpdatingUI)
	{
		return; // Prevent recursive updates
	}

	UpdateHealthBar(NewHealth, MaxHealth);
}

void UWB_HUD::OnStaminaChanged(float NewStamina, float MaxStamina)
{
	if (bUpdatingUI)
	{
		return;
	}

	UpdateStaminaBar(NewStamina, MaxStamina);
}

void UWB_HUD::OnRitualEnergyChanged(float NewRitualEnergy, float MaxRitualEnergy)
{
	if (bUpdatingUI)
	{
		return;
	}

	UpdateRitualEnergyBar(NewRitualEnergy, MaxRitualEnergy);
}

void UWB_HUD::RefreshAllBars()
{
	if (!CachedAttributeSet)
	{
		return;
	}

	bUpdatingUI = true;

	// Update all bars with current attribute values
	UpdateHealthBar(CachedAttributeSet->GetHealth(), CachedAttributeSet->GetMaxHealth());
	UpdateStaminaBar(CachedAttributeSet->GetStamina(), CachedAttributeSet->GetMaxStamina());
	UpdateRitualEnergyBar(CachedAttributeSet->GetRitualEnergy(), CachedAttributeSet->GetMaxRitualEnergy());

	bUpdatingUI = false;
}

void UWB_HUD::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (!HealthBar || !HealthText)
	{
		return;
	}

	// Calculate percentage fill (0.0 to 1.0)
	float FillPercent = GetBarFillPercentage(CurrentHealth, MaxHealth);
	HealthBar->SetPercent(FillPercent);

	// Set color based on health level
	FLinearColor BarColor = HealthBarColor;
	if (FillPercent < (LowHealthThreshold / 100.0f))
	{
		BarColor = LowHealthWarningColor; // Use warning color for low health
	}
	HealthBar->SetFillColorAndOpacity(BarColor);

	// Update text display
	HealthText->SetText(FText::FromString(FormatAttributeText(CurrentHealth, MaxHealth)));
}

void UWB_HUD::UpdateStaminaBar(float CurrentStamina, float MaxStamina)
{
	if (!StaminaBar || !StaminaText)
	{
		return;
	}

	// Calculate percentage fill
	float FillPercent = GetBarFillPercentage(CurrentStamina, MaxStamina);
	StaminaBar->SetPercent(FillPercent);

	// Set bar color
	StaminaBar->SetFillColorAndOpacity(StaminaBarColor);

	// Update text display
	StaminaText->SetText(FText::FromString(FormatAttributeText(CurrentStamina, MaxStamina)));
}

void UWB_HUD::UpdateRitualEnergyBar(float CurrentRitual, float MaxRitual)
{
	if (!RitualEnergyBar || !RitualEnergyText)
	{
		return;
	}

	// Calculate percentage fill
	float FillPercent = GetBarFillPercentage(CurrentRitual, MaxRitual);
	RitualEnergyBar->SetPercent(FillPercent);

	// Set bar color (can be dynamic based on energy level for visual feedback)
	FLinearColor EnergyColor = RitualEnergyBarColor;
	if (CurrentRitual > 80.0f)
	{
		// Fully charged - bright purple
		EnergyColor = FLinearColor(1.0f, 0.0f, 1.0f, 1.0f);
	}
	RitualEnergyBar->SetFillColorAndOpacity(EnergyColor);

	// Update text display
	RitualEnergyText->SetText(FText::FromString(FormatAttributeText(CurrentRitual, MaxRitual)));
}

float UWB_HUD::GetBarFillPercentage(float Current, float Max)
{
	if (Max <= 0.0f)
	{
		return 0.0f;
	}
	
	float Percentage = Current / Max;
	return FMath::Clamp(Percentage, 0.0f, 1.0f);
}

FString UWB_HUD::FormatAttributeText(float Current, float Max)
{
	return FString::Printf(TEXT("%.1f / %.1f"), Current, Max);
}
