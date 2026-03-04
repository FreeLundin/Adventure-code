#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AdventureMoverPawnBase.generated.h"

class UMoverComponent;         // Mover plugin (Experimental)
class UStateTreeComponent;     // StateTree
class UAbilitySystemComponent; // GAS (optional hook)
class USkeletalMeshComponent;

UCLASS()
class ADVENTURE_API AAdventureMoverPawnBase : public APawn
{
    GENERATED_BODY()

public:
    AAdventureMoverPawnBase();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;

    // --- Components ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Adventure|Components")
    TObjectPtr<USkeletalMeshComponent> Mesh;

    // Mover movement brain (Experimental)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Adventure|Components")
    TObjectPtr<UMoverComponent> Mover;

    // StateTree for high-level locomotion / traversal state orchestration
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Adventure|Components")
    TObjectPtr<UStateTreeComponent> StateTree;

    // Optional: GAS hook (if you keep ASC on PlayerState, pawn can still query it)
    UFUNCTION(BlueprintCallable, Category = "Adventure|GAS")
    UAbilitySystemComponent *GetASCFromPlayerState() const;

protected:
    // Input handlers (wire to Enhanced Input in BP or C++)
    void MoveForward(float Value);
    void MoveRight(float Value);
    void LookYaw(float Value);
    void LookPitch(float Value);
    void JumpPressed();
    void JumpReleased();

private:
    FVector2D MoveInput = FVector2D::ZeroVector;
    FVector2D LookInput = FVector2D::ZeroVector;
};

UCLASS()
class ADVENTURE_API ASVGLNDMoverPawnBase : public AAdventureMoverPawnBase
{
    GENERATED_BODY()
};
