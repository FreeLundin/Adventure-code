# Complete Guide: C++ and Blueprint Interoperability

This guide covers all three major use cases for C++ and Blueprint interaction in Unreal Engine, with practical examples from the Adventure project.

---

## Table of Contents

1. [Use Case 1: Exposing C++ to Blueprint](#use-case-1-exposing-c-to-blueprint)
2. [Use Case 2: Runtime Blueprint Instance Manipulation](#use-case-2-runtime-blueprint-instance-manipulation)
3. [Use Case 3: Editor Blueprint Asset Creation/Modification](#use-case-3-editor-blueprint-asset-creationmodification)
4. [Best Practices](#best-practices)
5. [Quick Reference](#quick-reference)

---

## Use Case 1: Exposing C++ to Blueprint

**When to use:** Creating C++ classes/functions that game designers can use in Blueprint graphs.

### Example Class: `ABlueprintInteropExample`

Located in: `Source/Adventure/Public/BlueprintInteropExample.h`

### Key Concepts

#### 1. Making Classes Blueprint-Accessible

```cpp
UCLASS(Blueprintable, BlueprintType)
class ADVENTURE_API AMyActor : public AActor
{
    GENERATED_BODY()
};
```

- **`Blueprintable`** - You can create Blueprint classes that inherit from this C++ class
- **`BlueprintType`** - Can be used as a variable type in Blueprints

#### 2. Property Exposure

| Specifier | Editor Visibility | Blueprint Graph Access | Use Case |
|-----------|------------------|----------------------|----------|
| `EditAnywhere` | Editable everywhere | - | Properties you want to configure in both BP defaults and level instances |
| `EditDefaultsOnly` | Only in BP defaults | - | Class-wide settings (not per-instance) |
| `EditInstanceOnly` | Only in level instances | - | Instance-specific values (like unique IDs) |
| `VisibleAnywhere` | Visible (read-only) | - | Display-only values |
| `BlueprintReadWrite` | - | Read & Write | Properties BP can get and set |
| `BlueprintReadOnly` | - | Read only | Properties BP can read but not modify |

**Common Combinations:**

```cpp
// Designers can edit in BP defaults and modify in BP graphs
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
float MaxHealth = 100.f;

// Designers can set in BP defaults, BP can only read
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
float HealthRegenRate = 5.f;

// Runtime-only value, visible for debugging
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Runtime")
float CurrentHealth;
```

#### 3. Function Exposure

**BlueprintCallable** - Regular function callable from BP:
```cpp
UFUNCTION(BlueprintCallable, Category = "Combat")
void TakeDamage(float DamageAmount);
```

**BlueprintPure** - Function with no side effects (displays as pure node with no exec pins):
```cpp
UFUNCTION(BlueprintPure, Category = "Stats")
float GetHealthPercentage() const;
```

**BlueprintImplementableEvent** - Function implemented in Blueprint (no C++ body):
```cpp
UFUNCTION(BlueprintImplementableEvent, Category = "Events")
void OnHealthChanged(float NewHealthPercentage);
// No C++ implementation needed - Blueprint provides the body
```

**BlueprintNativeEvent** - Can be implemented in both C++ and Blueprint:
```cpp
UFUNCTION(BlueprintNativeEvent, Category = "Events")
void OnStateChanged(ECharacterState NewState);

// C++ implementation MUST have _Implementation suffix:
virtual void OnStateChanged_Implementation(ECharacterState NewState);
```

#### 4. Delegates (Events)

Blueprints can bind to these events:

```cpp
// Declare delegate type
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);

// Add delegate property
UPROPERTY(BlueprintAssignable, Category = "Events")
FOnHealthChanged OnHealthChangedEvent;

// Fire the event in C++:
OnHealthChangedEvent.Broadcast(CurrentHealth);
```

In Blueprint, you can now create "Event Dispatchers" that listen to this.

#### 5. Enums and Structs

**Blueprint-accessible enum:**
```cpp
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
    Idle    UMETA(DisplayName = "Idle"),
    Walking UMETA(DisplayName = "Walking"),
    Running UMETA(DisplayName = "Running")
};
```

**Blueprint-accessible struct:**
```cpp
USTRUCT(BlueprintType)
struct FCharacterStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Health = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Stamina = 100.f;
};
```

#### 6. Advanced Metadata

```cpp
// Clamp values in editor
UPROPERTY(EditAnywhere, BlueprintReadWrite, 
    meta = (ClampMin = "0.0", ClampMax = "100.0"))
float Percentage;

// Conditional editing
UPROPERTY(EditAnywhere, BlueprintReadWrite)
bool bUseCustomValue;

UPROPERTY(EditAnywhere, BlueprintReadWrite, 
    meta = (EditCondition = "bUseCustomValue"))
float CustomValue;

// Function metadata
UFUNCTION(BlueprintCallable, 
    meta = (DisplayName = "Deal Damage", 
            Keywords = "hurt damage attack",
            ToolTip = "Applies damage to the target"))
void ApplyDamage(float Amount);
```

---

## Use Case 2: Runtime Blueprint Instance Manipulation

**When to use:** C++ code needs to interact with Blueprint-created actors at runtime (gameplay code).

### Example Class: `ABlueprintRuntimeManager`

Located in: `Source/Adventure/Public/BlueprintRuntimeManager.h`

### Key Operations

#### 1. Spawning Blueprint Actors from C++

**Method A: Using TSubclassOf reference (drag Blueprint into property)**

```cpp
// In your C++ class:
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
TSubclassOf<AActor> BlueprintActorClass;

// Spawn it:
AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
    BlueprintActorClass, 
    Location, 
    Rotation
);
```

**Method B: Loading by path**

```cpp
// Load Blueprint class at runtime
FString BlueprintPath = "/Game/Blueprints/BP_MyActor.BP_MyActor_C";
UClass* LoadedClass = StaticLoadClass(AActor::StaticClass(), nullptr, *BlueprintPath);

if (LoadedClass)
{
    AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(LoadedClass, Location, Rotation);
}
```

**Important:** Blueprint asset paths need the `_C` suffix to reference the generated class.

#### 2. Accessing Blueprint Properties via Reflection

```cpp
// Get a property value by name
bool GetBlueprintPropertyFloat(AActor* TargetActor, FName PropertyName, float& OutValue)
{
    if (!TargetActor) return false;
    
    FProperty* Property = TargetActor->GetClass()->FindPropertyByName(PropertyName);
    if (!Property) return false;
    
    FFloatProperty* FloatProp = CastField<FFloatProperty>(Property);
    if (!FloatProp) return false;
    
    OutValue = FloatProp->GetPropertyValue_InContainer(TargetActor);
    return true;
}

// Set a property value by name
bool SetBlueprintPropertyFloat(AActor* TargetActor, FName PropertyName, float Value)
{
    FProperty* Property = TargetActor->GetClass()->FindPropertyByName(PropertyName);
    if (!Property) return false;
    
    FFloatProperty* FloatProp = CastField<FFloatProperty>(Property);
    if (!FloatProp) return false;
    
    FloatProp->SetPropertyValue_InContainer(TargetActor, Value);
    return true;
}
```

**Usage:**
```cpp
float Health;
if (GetBlueprintPropertyFloat(MyBPActor, "Health", Health))
{
    UE_LOG(LogTemp, Log, TEXT("Current health: %f"), Health);
}

SetBlueprintPropertyFloat(MyBPActor, "Health", 50.f);
```

#### 3. Calling Blueprint Functions from C++

```cpp
bool CallBlueprintFunction(UObject* TargetObject, FName FunctionName)
{
    if (!TargetObject) return false;
    
    UFunction* Function = TargetObject->FindFunction(FunctionName);
    if (!Function) return false;
    
    // Call with no parameters
    TargetObject->ProcessEvent(Function, nullptr);
    return true;
}

// Call with parameters
bool CallBlueprintFunctionWithFloat(UObject* TargetObject, FName FunctionName, float Parameter)
{
    UFunction* Function = TargetObject->FindFunction(FunctionName);
    if (!Function) return false;
    
    // Create parameter struct
    struct { float Value; } Params;
    Params.Value = Parameter;
    
    TargetObject->ProcessEvent(Function, &Params);
    return true;
}
```

**Usage:**
```cpp
// Call Blueprint function "OnPlayerDied" with no parameters
CallBlueprintFunction(MyBPActor, "OnPlayerDied");

// Call Blueprint function "TakeDamage" with float parameter
CallBlueprintFunctionWithFloat(MyBPActor, "TakeDamage", 25.f);
```

#### 4. Accessing Blueprint Components

```cpp
// Get component by class
TArray<UActorComponent*> Components;
TargetActor->GetComponents(UStaticMeshComponent::StaticClass(), Components);

for (UActorComponent* Comp : Components)
{
    UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Comp);
    if (MeshComp)
    {
        // Do something with the mesh component
    }
}

// Add component at runtime
UStaticMeshComponent* NewMesh = NewObject<UStaticMeshComponent>(TargetActor);
NewMesh->RegisterComponent();
TargetActor->AddInstanceComponent(NewMesh);
```

#### 5. Finding Blueprint Instances

```cpp
// Find all instances of a Blueprint class
TArray<AActor*> FoundActors;
UGameplayStatics::GetAllActorsOfClass(GetWorld(), BlueprintClass, FoundActors);

// Find instances with a specific tag
for (AActor* Actor : FoundActors)
{
    if (Actor->ActorHasTag("Enemy"))
    {
        // Process enemy actor
    }
}
```

#### 6. Working with Blueprint Interfaces

```cpp
// Check if actor implements an interface
if (TargetActor->GetClass()->ImplementsInterface(UMyInterface::StaticClass()))
{
    // Execute interface function (type-safe)
    IMyInterface::Execute_MyInterfaceFunction(TargetActor);
}
```

---

## Use Case 3: Editor Blueprint Asset Creation/Modification

**When to use:** Automating Blueprint creation, batch editing Blueprints, or building editor tools.

### Example Class: `UBlueprintEditorUtilities`

Located in: `Source/Adventure/Public/BlueprintEditorUtilities.h`

### ⚠️ Important Notes

1. **Editor-only code** - Must be wrapped in `#if WITH_EDITOR` preprocessor directives
2. **Only runs in editor** - Not available in packaged games
3. **Requires editor modules** - Needs `UnrealEd`, `Kismet`, `BlueprintGraph` modules in Build.cs

### Key Operations

#### 1. Creating New Blueprint Assets

```cpp
#if WITH_EDITOR

UBlueprint* CreateNewBlueprint(const FString& PackagePath, const FString& BlueprintName, UClass* ParentClass)
{
    // Create package
    FString PackageName = PackagePath + BlueprintName;
    UPackage* Package = CreatePackage(*PackageName);
    
    // Create Blueprint
    UBlueprint* NewBlueprint = FKismetEditorUtilities::CreateBlueprint(
        ParentClass,
        Package,
        FName(*BlueprintName),
        BPTYPE_Normal,
        UBlueprint::StaticClass(),
        UBlueprintGeneratedClass::StaticClass()
    );
    
    if (NewBlueprint)
    {
        // Compile and register
        FKismetEditorUtilities::CompileBlueprint(NewBlueprint);
        FAssetRegistryModule::AssetCreated(NewBlueprint);
    }
    
    return NewBlueprint;
}

#endif
```

**Usage:**
```cpp
// Create a new Blueprint that inherits from AActor
UBlueprint* NewBP = UBlueprintEditorUtilities::CreateNewBlueprint(
    "/Game/Blueprints/",
    "BP_MyNewActor",
    AActor::StaticClass()
);
```

#### 2. Adding Components to Blueprints

```cpp
bool AddComponentToBlueprint(UBlueprint* Blueprint, TSubclassOf<UActorComponent> ComponentClass, const FString& ComponentName)
{
    USimpleConstructionScript* SCS = Blueprint->SimpleConstructionScript;
    if (!SCS) return false;
    
    // Create new SCS node
    USCS_Node* NewNode = SCS->CreateNode(ComponentClass, FName(*ComponentName));
    SCS->AddNode(NewNode);
    
    // Mark modified and recompile
    FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
    
    return true;
}
```

**Usage:**
```cpp
// Add a Static Mesh Component to a Blueprint
UBlueprintEditorUtilities::AddComponentToBlueprint(
    MyBlueprint,
    UStaticMeshComponent::StaticClass(),
    "MyMeshComponent"
);
```

#### 3. Adding Variables to Blueprints

```cpp
bool AddFloatVariable(UBlueprint* Blueprint, const FString& VariableName, float DefaultValue)
{
    // Define pin type (float)
    FEdGraphPinType PinType;
    PinType.PinCategory = UEdGraphSchema_K2::PC_Real;
    PinType.PinSubCategory = UEdGraphSchema_K2::PC_Float;
    
    // Add variable
    FName VarName = FBlueprintEditorUtils::FindUniqueKismetName(Blueprint, VariableName);
    FBlueprintEditorUtils::AddMemberVariable(Blueprint, VarName, PinType);
    
    // Mark modified
    FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
    
    return true;
}
```

**Common Pin Types:**
- `PC_Real` + `PC_Float` - Float
- `PC_Int` - Integer
- `PC_Boolean` - Boolean
- `PC_String` - String
- `PC_Object` - Object reference
- `PC_Struct` - Struct
- Set `ContainerType = EPinContainerType::Array` for arrays

#### 4. Modifying Blueprint Properties

```cpp
bool SetBlueprintDefaultFloatProperty(UBlueprint* Blueprint, const FString& PropertyName, float Value)
{
    // Get the Class Default Object (CDO)
    UObject* CDO = Blueprint->GeneratedClass->GetDefaultObject();
    
    // Find property
    FProperty* Property = CDO->GetClass()->FindPropertyByName(FName(*PropertyName));
    FFloatProperty* FloatProp = CastField<FFloatProperty>(Property);
    
    if (FloatProp)
    {
        FloatProp->SetPropertyValue_InContainer(CDO, Value);
        FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
        return true;
    }
    
    return false;
}
```

#### 5. Finding and Loading Blueprint Assets

```cpp
// Find all Blueprints in a path
TArray<UBlueprint*> FindAllBlueprintsInPath(const FString& Path)
{
    TArray<UBlueprint*> Blueprints;
    FAssetRegistryModule& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    
    TArray<FAssetData> AssetList;
    AssetRegistry.Get().GetAssetsByPath(FName(*Path), AssetList, true);
    
    for (const FAssetData& Asset : AssetList)
    {
        if (Asset.AssetClassPath.ToString() == TEXT("/Script/Engine.Blueprint"))
        {
            UBlueprint* BP = Cast<UBlueprint>(Asset.GetAsset());
            if (BP) Blueprints.Add(BP);
        }
    }
    
    return Blueprints;
}
```

#### 6. Batch Operations

```cpp
// Example: Add a variable to all Blueprints in a folder
void BatchAddHealthVariable()
{
    TArray<UBlueprint*> Blueprints = FindAllBlueprintsInPath("/Game/Blueprints/Characters/");
    
    for (UBlueprint* BP : Blueprints)
    {
        AddFloatVariable(BP, "Health", 100.f);
        SetVariableMetadata(BP, "Health", true, false, "Stats");
        CompileBlueprint(BP);
    }
}
```

#### 7. Compiling and Saving

```cpp
// Compile a Blueprint
FKismetEditorUtilities::CompileBlueprint(Blueprint);

// Save a Blueprint asset
UPackage* Package = Blueprint->GetOutermost();
FString PackageFileName = FPackageName::LongPackageNameToFilename(
    Package->GetName(), 
    FPackageName::GetAssetPackageExtension()
);

FSavePackageArgs SaveArgs;
SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
UPackage::SavePackage(Package, nullptr, *PackageFileName, SaveArgs);
```

---

## Best Practices

### When to Use Each Approach

| Scenario | Use Case | Example |
|----------|----------|---------|
| Creating gameplay classes | Use Case 1 | Character classes, weapon classes, pickups |
| Runtime dynamic behavior | Use Case 2 | Spawning enemies, modifying AI behavior, procedural generation |
| Editor automation | Use Case 3 | Batch processing, automated testing, custom editor tools |

### Performance Considerations

1. **Reflection is slower** - Use Case 2's property/function access via reflection has overhead. For performance-critical code, use direct C++ or defined interfaces.

2. **Editor-only code** - Use Case 3 operations are expensive. Never try to run them at runtime.

3. **Casting is cheap** - If you know the type, cast and access directly rather than using reflection:
   ```cpp
   // Fast (if you know it's AMyCharacter)
   AMyCharacter* Character = Cast<AMyCharacter>(Actor);
   Character->TakeDamage(10.f);
   
   // Slower (using reflection)
   SetBlueprintPropertyFloat(Actor, "Health", 50.f);
   ```

### Design Patterns

#### Pattern 1: C++ Base Classes with Blueprint Children

```cpp
// C++ base class (Use Case 1)
UCLASS(Blueprintable)
class ABaseEnemy : public AActor
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float BaseHealth = 100.f;
    
    UFUNCTION(BlueprintNativeEvent)
    void OnDeath();
    
    virtual void OnDeath_Implementation() 
    {
        // Default C++ behavior
    }
};

// Blueprint inherits and customizes:
// - Overrides OnDeath to play custom death animation
// - Sets BaseHealth to different values
// - Adds Blueprint-specific visual components
```

**When to use:** Most common pattern. C++ defines structure and core logic, Blueprints customize per enemy type.

#### Pattern 2: Runtime Factory Pattern

```cpp
// Factory that spawns different Blueprint types (Use Case 2)
UCLASS()
class AEnemySpawner : public AActor
{
    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<ABaseEnemy>> EnemyTypes;
    
    void SpawnRandomEnemy()
    {
        int32 Index = FMath::RandRange(0, EnemyTypes.Num() - 1);
        GetWorld()->SpawnActor<ABaseEnemy>(EnemyTypes[Index], GetActorLocation(), FRotator::ZeroRotator);
    }
};
```

#### Pattern 3: Editor Automation

```cpp
#if WITH_EDITOR
// Commandlet or Editor Utility Widget (Use Case 3)
void AutomateEnemySetup()
{
    // Find all enemy Blueprints
    TArray<UBlueprint*> Enemies = FindBlueprintsByParentClass(ABaseEnemy::StaticClass());
    
    // Add health component to each
    for (UBlueprint* Enemy : Enemies)
    {
        AddComponentToBlueprint(Enemy, UHealthComponent::StaticClass(), "HealthComp");
        CompileBlueprint(Enemy);
        SaveBlueprint(Enemy);
    }
}
#endif
```

---

## Quick Reference

### Use Case 1: Exposing C++ to Blueprint

| What | How |
|------|-----|
| Make class Blueprint-able | `UCLASS(Blueprintable)` |
| Make property visible | `UPROPERTY(EditAnywhere, BlueprintReadWrite)` |
| Make function callable | `UFUNCTION(BlueprintCallable)` |
| Make pure function | `UFUNCTION(BlueprintPure)` and mark `const` |
| Allow Blueprint to implement | `UFUNCTION(BlueprintImplementableEvent)` |
| Allow Blueprint to override | `UFUNCTION(BlueprintNativeEvent)` + `_Implementation` |
| Create event Blueprint can bind to | `UPROPERTY(BlueprintAssignable)` delegate |
| Make enum visible | `UENUM(BlueprintType)` |
| Make struct visible | `USTRUCT(BlueprintType)` |

### Use Case 2: Runtime Manipulation

| Task | Function/Pattern |
|------|------------------|
| Spawn Blueprint actor | `GetWorld()->SpawnActor<AActor>(BlueprintClass, ...)` |
| Load Blueprint class | `StaticLoadClass(AActor::StaticClass(), nullptr, *Path)` |
| Get property by name | `FindPropertyByName()` + `CastField<>()` + `GetPropertyValue_InContainer()` |
| Set property by name | `FindPropertyByName()` + `CastField<>()` + `SetPropertyValue_InContainer()` |
| Call function by name | `FindFunction()` + `ProcessEvent()` |
| Get component | `GetComponents(ComponentClass, OutArray)` |
| Find all instances | `UGameplayStatics::GetAllActorsOfClass()` |

### Use Case 3: Editor Operations

| Task | Function |
|------|----------|
| Create new Blueprint | `FKismetEditorUtilities::CreateBlueprint()` |
| Add component | `SCS->CreateNode()` + `SCS->AddNode()` |
| Add variable | `FBlueprintEditorUtils::AddMemberVariable()` |
| Add function | `FBlueprintEditorUtils::CreateNewGraph()` + `AddFunctionGraph()` |
| Compile Blueprint | `FKismetEditorUtilities::CompileBlueprint()` |
| Save Blueprint | `UPackage::SavePackage()` |
| Find Blueprints | `FAssetRegistryModule::Get().GetAssetsByPath()` |
| Mark modified | `FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified()` |

---

## Example Workflows

### Workflow 1: Creating a Customizable Character System

1. **Use Case 1**: Create `ABaseCharacter` C++ class with Blueprint-exposed properties:
   ```cpp
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   float MaxHealth;
   
   UFUNCTION(BlueprintNativeEvent)
   void OnDeath();
   ```

2. **Editor**: Create multiple Blueprint children (BP_Warrior, BP_Mage, etc.) that inherit from `ABaseCharacter`

3. **Use Case 2**: At runtime, spawn the correct character Blueprint based on player choice:
   ```cpp
   TSubclassOf<ABaseCharacter> ChosenClass = PlayerSelection == "Warrior" ? WarriorClass : MageClass;
   ABaseCharacter* Player = GetWorld()->SpawnActor<ABaseCharacter>(ChosenClass, ...);
   ```

### Workflow 2: Procedural Enemy Generation

1. **Use Case 1**: Create `AProceduralEnemy` with Blueprint-customizable stats

2. **Use Case 2**: At runtime, spawn base enemy and modify properties programmatically:
   ```cpp
   AProceduralEnemy* Enemy = GetWorld()->SpawnActor<AProceduralEnemy>(EnemyClass, ...);
   SetBlueprintPropertyFloat(Enemy, "Health", FMath::RandRange(50.f, 150.f));
   SetBlueprintPropertyFloat(Enemy, "Speed", FMath::RandRange(300.f, 600.f));
   ```

### Workflow 3: Batch Blueprint Setup

1. **Use Case 3**: Create editor script to add standard components to all character Blueprints:
   ```cpp
   TArray<UBlueprint*> Characters = FindAllBlueprintsInPath("/Game/Characters/");
   for (UBlueprint* BP : Characters)
   {
       AddComponentToBlueprint(BP, UHealthComponent::StaticClass(), "Health");
       AddFloatVariable(BP, "WalkSpeed", 600.f);
       CompileBlueprint(BP);
       SaveBlueprint(BP);
   }
   ```

---

## Troubleshooting

### Common Issues

**1. "Property not found" when using Use Case 2**
- Ensure property is properly exposed with `UPROPERTY(BlueprintReadWrite)` or similar
- Check spelling and case sensitivity
- Compile both C++ and Blueprint

**2. "Function not found" when calling Blueprint functions**
- Blueprint functions need matching signatures
- Check function name spelling
- Ensure function is marked as `BlueprintCallable` if defined in C++

**3. Editor utilities not compiling**
- Verify `WITH_EDITOR` preprocessor check
- Ensure your `.Build.cs` includes editor modules:
  ```csharp
  if (Target.bBuildEditor)
  {
      PrivateDependencyModuleNames.AddRange(new string[] { 
          "UnrealEd", 
          "Kismet", 
          "BlueprintGraph" 
      });
  }
  ```

**4. Blueprint path not loading**
- Blueprint class paths must end with `_C`
- Example: `/Game/Blueprints/BP_MyActor.BP_MyActor_C`
- Use Asset Registry to find correct paths

---

## Additional Resources

- **Unreal Documentation**: [Blueprint and C++ Integration](https://docs.unrealengine.com/5.0/en-US/blueprints-and-cpp/)
- **Example Files**:
  - `BlueprintInteropExample.h/cpp` - Use Case 1 examples
  - `BlueprintRuntimeManager.h/cpp` - Use Case 2 examples
  - `BlueprintEditorUtilities.h/cpp` - Use Case 3 examples
