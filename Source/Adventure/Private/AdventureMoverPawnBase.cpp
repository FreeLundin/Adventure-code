#include "AdventureMoverPawnBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

// Mover includes (paths may vary by engine setup; rely on IntelliSense)
#include "MoverComponent.h" // provided by Mover plugin

// StateTree
#include "StateTreeComponent.h"

AAdventureMoverPawnBase::AAdventureMoverPawnBase()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);

    Mover = CreateDefaultSubobject<UMoverComponent>(TEXT("Mover"));
    StateTree = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));

    AutoPossessPlayer = EAutoReceiveInput::Disabled; // GameMode should possess
}

void AAdventureMoverPawnBase::BeginPlay()
{
    Super::BeginPlay();

    // StateTree will run if configured with a StateTree asset in BP
}

void AAdventureMoverPawnBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // You can forward MoveInput/LookInput into Mover via BP or custom code.
    // Keep heavy logic out of Tick if possible (event-driven preferred).
}

void AAdventureMoverPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Minimal legacy axis bindings (you can replace with Enhanced Input)
    PlayerInputComponent->BindAxis("MoveForward", this, &AAdventureMoverPawnBase::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AAdventureMoverPawnBase::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &AAdventureMoverPawnBase::LookYaw);
    PlayerInputComponent->BindAxis("LookUp", this, &AAdventureMoverPawnBase::LookPitch);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAdventureMoverPawnBase::JumpPressed);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AAdventureMoverPawnBase::JumpReleased);
}

void AAdventureMoverPawnBase::MoveForward(float Value) { MoveInput.Y = Value; }
void AAdventureMoverPawnBase::MoveRight(float Value)   { MoveInput.X = Value; }
void AAdventureMoverPawnBase::LookYaw(float Value)     { LookInput.X = Value; }
void AAdventureMoverPawnBase::LookPitch(float Value)   { LookInput.Y = Value; }

void AAdventureMoverPawnBase::JumpPressed()
{
    // Hook to Mover jump if enabled; otherwise use a layered move or mode.
}

void AAdventureMoverPawnBase::JumpReleased()
{
}

UAbilitySystemComponent* AAdventureMoverPawnBase::GetASCFromPlayerState() const
{
    const APlayerState* PS = GetPlayerState();
    if (!PS) return nullptr;
    return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(const_cast<APlayerState*>(PS));
}
