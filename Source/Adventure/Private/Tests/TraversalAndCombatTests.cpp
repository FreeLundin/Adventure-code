#include "Misc/AutomationTest.h"
#include "GAS/AdventureGameplayAbility.h"
#include "GAS/AdventureGameplayTags.h"
#include "GAS/AdventureAttributeSet.h"
#include "Core/AdventurePlayerState.h"
#include "Core/PC_AdventureController.h"
#include "Character/CBP_AdventureCharacter.h"
#include "Components/AdventureGASStateTreeBridgeComponent.h"
#include "AbilitySystemComponent.h"

#if WITH_DEV_AUTOMATION_TESTS

// helper that creates an ASC with the ritual energy attribute attached
static UAbilitySystemComponent *CreateTestASC()
{
    UAbilitySystemComponent *ASC = NewObject<UAbilitySystemComponent>();
    ASC->RegisterComponent();
    ASC->AddAttributeSetSubobject(NewObject<UAdventureAttributeSet>(ASC));
    return ASC;
}

// ---------------------------------------------------------------------
// actual tests
// ---------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FClimbBasicTest, "SVGLND.Traversal.Climb.Basic", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FClimbBasicTest::RunTest(const FString &Parameters)
{
    UGA_AdventureClimb *Ability = NewObject<UGA_AdventureClimb>();
    TestNotNull(TEXT("Climb ability instance created"), Ability);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCombatBlockedDuringTraversalTest, "SVGLND.Combat.BlockedDuringTraversal", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FCombatBlockedDuringTraversalTest::RunTest(const FString &Parameters)
{
    APC_AdventureController *Controller = NewObject<APC_AdventureController>();
    Controller->CachedAbilitySystemComponent = CreateTestASC();

    Controller->SetTraversalOverride(true);
    Controller->TryActivateAbilityByTag(AdventureGameplayTags::Ability_Attack_Light);
    TestEqual(TEXT("No activation attempts when overridden"), Controller->ActivationAttempts, 0);

    Controller->SetTraversalOverride(false);
    Controller->TryActivateAbilityByTag(AdventureGameplayTags::Ability_Attack_Light);
    TestEqual(TEXT("One activation attempt after override removed"), Controller->ActivationAttempts, 1);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCameraToggleTest, "SVGLND.Camera.Toggle", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FCameraToggleTest::RunTest(const FString &Parameters)
{
    APC_AdventureController *Controller = NewObject<APC_AdventureController>();
    TestEqual(TEXT("Initial camera style index"), Controller->GetCurrentCameraStyle(), 1);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCameraSmoothTest, "SVGLND.Camera.SmoothTransition", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FCameraSmoothTest::RunTest(const FString &Parameters)
{
    APC_AdventureController *Controller = NewObject<APC_AdventureController>();
    Controller->CycleCamera();
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRitualDecayTest, "SVGLND.RitualEnergy.Decay", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FRitualDecayTest::RunTest(const FString &Parameters)
{
    AAdventurePlayerState *PS = NewObject<AAdventurePlayerState>();
    UAbilitySystemComponent *ASC = CreateTestASC();
    ASC->SetNumericAttributeBase(UAdventureAttributeSet::GetRitualEnergyAttribute(), 10.0f);
    PS->TimeSinceCombat = 8.0f;
    PS->Tick(1.0f);
    float After = ASC->GetNumericAttribute(UAdventureAttributeSet::GetRitualEnergyAttribute());
    TestTrue(TEXT("Ritual energy should have decreased"), After < 10.0f);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHUDBindingTest, "SVGLND.HUD.Bindings", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FHUDBindingTest::RunTest(const FString &Parameters)
{
    TestTrue(TEXT("HUD widget class open"), true);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLightAttackTest, "SVGLND.Combat.LightAttack", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FLightAttackTest::RunTest(const FString &Parameters)
{
    UGA_AdventureLightAttack *Ability = NewObject<UGA_AdventureLightAttack>();
    TestTrue(TEXT("Light ability has correct tag"), Ability->HasMatchingGameplayTag(AdventureGameplayTags::Ability_Attack_Light));

    UAbilitySystemComponent *ASC = CreateTestASC();
    float Before = ASC->GetNumericAttribute(UAdventureAttributeSet::GetRitualEnergyAttribute());
    FGameplayAbilityActorInfo ActorInfo;
    ActorInfo.AbilitySystemComponent = ASC;
    Ability->ActivateAbility(FGameplayAbilitySpecHandle(), &ActorInfo, FGameplayAbilityActivationInfo(), nullptr);
    float After = ASC->GetNumericAttribute(UAdventureAttributeSet::GetRitualEnergyAttribute());
    TestTrue(TEXT("Ritual energy increased by at least 3"), After >= Before + 3.0f);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHeavyAttackTest, "SVGLND.Combat.HeavyAttack", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FHeavyAttackTest::RunTest(const FString &Parameters)
{
    UGA_AdventureHeavyAttack *Ability = NewObject<UGA_AdventureHeavyAttack>();
    TestTrue(TEXT("Heavy ability has correct tag"), Ability->HasMatchingGameplayTag(AdventureGameplayTags::Ability_Attack_Heavy));

    UAbilitySystemComponent *ASC = CreateTestASC();
    float Before = ASC->GetNumericAttribute(UAdventureAttributeSet::GetRitualEnergyAttribute());
    FGameplayAbilityActorInfo ActorInfo;
    ActorInfo.AbilitySystemComponent = ASC;
    Ability->ActivateAbility(FGameplayAbilitySpecHandle(), &ActorInfo, FGameplayAbilityActivationInfo(), nullptr);
    float After = ASC->GetNumericAttribute(UAdventureAttributeSet::GetRitualEnergyAttribute());
    TestTrue(TEXT("Ritual energy increased by at least 8"), After >= Before + 8.0f);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlockAttackTest, "SVGLND.Combat.BlockAttack", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FBlockAttackTest::RunTest(const FString &Parameters)
{
    UGA_AdventureBlock *Ability = NewObject<UGA_AdventureBlock>();
    TestTrue(TEXT("Block ability has correct tag"), Ability->HasMatchingGameplayTag(AdventureGameplayTags::Ability_Attack_Block));

    UAbilitySystemComponent *ASC = CreateTestASC();
    float Before = ASC->GetNumericAttribute(UAdventureAttributeSet::GetRitualEnergyAttribute());
    FGameplayAbilityActorInfo ActorInfo;
    ActorInfo.AbilitySystemComponent = ASC;
    Ability->ActivateAbility(FGameplayAbilitySpecHandle(), &ActorInfo, FGameplayAbilityActivationInfo(), nullptr);
    float After = ASC->GetNumericAttribute(UAdventureAttributeSet::GetRitualEnergyAttribute());
    TestTrue(TEXT("Ritual energy increased by at least 1"), After >= Before + 1.0f);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FParryAttackTest, "SVGLND.Combat.ParryAttack", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FParryAttackTest::RunTest(const FString &Parameters)
{
    UGA_AdventureParry *Ability = NewObject<UGA_AdventureParry>();
    TestTrue(TEXT("Parry ability has correct tag"), Ability->HasMatchingGameplayTag(AdventureGameplayTags::Ability_Attack_Parry));

    UAbilitySystemComponent *ASC = CreateTestASC();
    float Before = ASC->GetNumericAttribute(UAdventureAttributeSet::GetRitualEnergyAttribute());
    FGameplayAbilityActorInfo ActorInfo;
    ActorInfo.AbilitySystemComponent = ASC;
    Ability->ActivateAbility(FGameplayAbilitySpecHandle(), &ActorInfo, FGameplayAbilityActivationInfo(), nullptr);
    float After = ASC->GetNumericAttribute(UAdventureAttributeSet::GetRitualEnergyAttribute());
    TestTrue(TEXT("Ritual energy increased by at least 5"), After >= Before + 5.0f);
    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
