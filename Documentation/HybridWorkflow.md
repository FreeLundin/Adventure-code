# Hybrid C++ / Blueprint Workflow

This document captures the patterns, tools and lessons we developed in the **Adventure** project to make working across C++ and Blueprints easy, consistent and automatable.  The techniques are generic; copy them into any other Unreal Engine project to get the same benefits.

---
## 1. Project‑wide reflection macros

Create a header with convenient wrappers around `UCLASS`, `USTRUCT`, `UPROPERTY`, etc.  The macros reduce boilerplate and encourage designers to expose everything by default.

```cpp
// Source/<YourModule>/Public/Core/AdventureMacros.h
#pragma once

// class/struct decorators
#define ADV_CLASS(...) UCLASS(Blueprintable, BlueprintType, __VA_ARGS__)
#define ADV_STRUCT(...) USTRUCT(BlueprintType, __VA_ARGS__)

// interface helpers
#define ADV_INTERFACE() UINTERFACE(Blueprintable, MinimalAPI)
#define ADV_INTERFACE_FUNC(ReturnType) UFUNCTION(BlueprintNativeEvent, Category="Adventure") ReturnType

// function/property helpers
#define ADV_FUNC(...) UFUNCTION(BlueprintCallable, Category="Adventure", __VA_ARGS__)
#define ADV_FUNC_PURE(...) UFUNCTION(BlueprintPure, Category="Adventure", __VA_ARGS__)
#define ADV_NATIVE_EVENT() UFUNCTION(BlueprintNativeEvent, Category="Adventure")
#define ADV_IMPLEMENT_EVENT(func) void func##_Implementation

// properties
#define ADV_PROP(...) UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Adventure", __VA_ARGS__)
#define ADV_PROP_RO(...) UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Adventure", __VA_ARGS__)

// metadata helpers
#define ADV_EXPOSE_ON_SPAWN() meta=(ExposeOnSpawn)
#define ADV_CLAMP_MIN(val) meta=(ClampMin = val)
#define ADV_CLAMP_MAX(val) meta=(ClampMax = val)
```

Include this header at the top of every C++ class, struct or interface and use the macros instead of writing the full `UPROPERTY` declaration each time.  Add more helpers as new metadata patterns appear (e.g. `ADV_TOOLTIP`, `ADV_DEPRECATED`, etc.).

---
## 2. Example usage

```cpp
// a class:
ADV_CLASS()
class AMyActor : public AActor
{
    GENERATED_BODY()

    ADV_PROP(ADV_CLAMP_MIN("0"))
    float Health;

    ADV_PROP_RO()
    UCameraComponent* Camera;

    ADV_FUNC()
    void DoSomething();

    ADV_NATIVE_EVENT()
    void OnEvent();
};
```

Interfaces use `ADV_INTERFACE()` and `ADV_INTERFACE_FUNC`:

```cpp
ADV_INTERFACE()
class UMyInterface : public UInterface { GENERATED_BODY() };

class IMyInterface
{
    GENERATED_BODY();
    ADV_INTERFACE_FUNC(void)(AActor* Instigator);
};
```

---
## 3. Exposure checker script

Place a Python script in `scripts/check_blueprint_exposure.py`:

```python
#!/usr/bin/env python3
import os, re, sys
PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SEARCH_ROOT = os.path.join(PROJECT_ROOT, "Source", "Adventure")
prop_pattern = re.compile(r"UPROPERTY\(([^)]*)\)")
blueprint_keywords = ["BlueprintRead", "BlueprintWrite", "BlueprintCallable", "BlueprintImplementableEvent", "BlueprintNativeEvent"]

def scan_file(path):
    with open(path, 'r', encoding='utf-8', errors='ignore') as f:
        for line_no, line in enumerate(f, 1):
            match = prop_pattern.search(line)
            if match:
                params = match.group(1)
                if not any(k in params for k in blueprint_keywords):
                    print(f"{path}:{line_no}: UPROPERTY missing Blueprint specifier -> {params}")

def main(root=SEARCH_ROOT):
    for dirpath, dirs, files in os.walk(root):
        for fname in files:
            if fname.endswith('.h') or fname.endswith('.cpp'):
                scan_file(os.path.join(dirpath, fname))

if __name__ == '__main__':
    target = sys.argv[1] if len(sys.argv) > 1 else SEARCH_ROOT
    main(target)
```

**How to use**:

```bash
python scripts/check_blueprint_exposure.py  # scans the project
``` 
Optionally, add a pre‑commit hook (see `.git/hooks/pre-commit` example below) or integrate it into CI.

---
## 4. CI integration

A GitHub Actions workflow runs the script on every push/PR touching source files.

```yaml
# .github/workflows/blueprint-exposure.yml
name: "Blueprint Exposure Check"

on:
  push:
    paths:
      - 'Source/**'
      - 'scripts/check_blueprint_exposure.py'
  pull_request:
    paths:
      - 'Source/**'
      - 'scripts/check_blueprint_exposure.py'

jobs:
  check-exposure:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v3
      - uses: actions/setup-python@v4
        with:
          python-version: '3.11'
      - name: Run blueprint exposure script
        run: |
          python scripts/check_blueprint_exposure.py
```

Adjust paths for your module name if not `Adventure`.

---
## 5. UHT plugin for compile‑time rules

A minimal plugin stub lives under `Plugins/AdventureUHT` and registers a module that can hook into UHT parsing.

- `AdventureUHT.uplugin` describes the plugin.
- `Source/AdventureUHT/AdventureUHTModule.*` contains the module implementation.

In `StartupModule()` you can add callbacks (e.g. `FHeaderParser::OnParseClass.AddStatic`) to run custom validation and issue errors or warnings.  Use this to enforce macro usage or metadata patterns across the codebase.

---
## 6. Editor utilities and snippets

* Create editor utility widgets (Blutilities) to scaffold new Blueprintable classes/interfaces with the proper macros.
* Add VSCode/Visual Studio code snippets that expand to `ADV_CLASS()` skeletons, interface patterns, or `ADV_PROP_RO()` declarations.  Store them in your repo (e.g. `.vscode/snippets/`) and share with the team.

---
## 7. Documentation & style guide

Keep a short markdown style guide (this file is one such document) under `Documentation/`. Include:

* Macro reference with examples
* When to use `BlueprintCallable` vs `BlueprintImplementableEvent` vs `BlueprintNativeEvent`
* Naming conventions (`b` prefix for bools, `e` for enums, `F` for structs, etc.)
* How and when to run the exposure script or add new macros

This guide can be copied to other projects or turned into a wiki.

---
## 8. Lessons learned

1. **Opt‑in reflection is power** – macros make it painless to opt‑in consistently.  Without them, macros are forgotten and things mysteriously disappear from Blueprints.
2. **Early feedback avoids pain** – the script and CI catch missing specs before they reach designers or cause crashes.
3. **Interfaces decouple systems** – using `UINTERFACE`/`IInterface` with macros keeps C++ code minimal and designers free to implement behavior in Blueprints.
4. **Automation reduces toil** – a few helper scripts and a plugin stub save hours of repetitive editing when the project grows.
5. **Documentation prevents tribal knowledge** – having a written guide lets new contributors follow the patterns immediately.

---
## 9. How to port to another project

1. **Copy `AdventureMacros.h`** to your new module and rename macros (e.g. `MYGAME_PROP`).
2. **Add the Python script** under `scripts/` and update `SEARCH_ROOT` to your module path.
3. **Create the GitHub Actions workflow** (or equivalent CI job). Adjust paths accordingly.
4. **Copy the UHT plugin** folder if you want compile‑time enforcement; update module names to match your project.
5. **Add example interfaces/structs** using the macros so others see usage patterns.
6. **Add pre‑commit hook (optional)**:
   ```bash
   #! /bin/sh
   python scripts/check_blueprint_exposure.py || exit 1
   ```
   Place this in `.git/hooks/pre-commit` and make it executable.
7. **Write a `Documentation/HybridWorkflow.md`** similar to this one and update as patterns evolve.

With those in place, the workflow in your new project will be identical: developers use macros, designers can rely on Blueprints being exposed, and tooling keeps the bridge healthy.

---

Feel free to adapt any part of this documentation or tooling to suit your team’s conventions; the core ideas are what matters.  If you want help generating the initial macros or hooks in a fresh repository, just ask and I’ll scaffold it for you. Good luck!  
