#pragma once

// Project-wide convenience macros for automatically exposing
// C++ classes, structs, functions and properties to Blueprints.
//
// Using these helps ensure we don't forget the usual
// BlueprintReadWrite/EditAnywhere specifiers and keeps
// declarations consistent across the codebase.

// class/struct decorators
#define ADV_CLASS(...) UCLASS(Blueprintable, BlueprintType, __VA_ARGS__)
#define ADV_STRUCT(...) USTRUCT(BlueprintType, __VA_ARGS__)

// interface decorator
#define ADV_INTERFACE() UINTERFACE(Blueprintable, MinimalAPI)

// return type for native interface functions
#define ADV_INTERFACE_FUNC(ReturnType) UFUNCTION(BlueprintNativeEvent, Category="Adventure") ReturnType

// function/property decorators with sensible defaults
#define ADV_FUNC(...) UFUNCTION(BlueprintCallable, Category="Adventure", __VA_ARGS__)
#define ADV_FUNC_PURE(...) UFUNCTION(BlueprintPure, Category="Adventure", __VA_ARGS__)

// handy shortcut for Blueprint events that can be overridden in C++
#define ADV_NATIVE_EVENT() UFUNCTION(BlueprintNativeEvent, Category="Adventure")
#define ADV_IMPLEMENT_EVENT(func) void func##_Implementation

// event macros
#define ADV_EVENT() UFUNCTION(BlueprintImplementableEvent, Category="Adventure")
#define ADV_EVENT_NATIVE() UFUNCTION(BlueprintNativeEvent, Category="Adventure")

// property macro - read/write by default, editable anywhere
#define ADV_PROP(...) UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Adventure", __VA_ARGS__)

// read-only property helper (useful for components exposed for lookup)
#define ADV_PROP_RO(...) UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Adventure", __VA_ARGS__)

// common metadata patterns
#define ADV_EXPOSE_ON_SPAWN() meta=(ExposeOnSpawn)
#define ADV_CLAMP_MIN(val) meta=(ClampMin = val)
#define ADV_CLAMP_MAX(val) meta=(ClampMax = val)

// Example usage (commented out - keep for reference):
// ADV_CLASS()
// class AMyActor : public AActor { ... };
// ADV_PROP(ADV_CLAMP_MIN("0.0")) float Health;
// ADV_FUNC() void DoThing();
