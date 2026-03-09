# Live Coding Troubleshooting & Configuration
**Adventure | UE 5.7 | Feb 21, 2026**

---

## ⚠️ Live Coding Issues & Fixes

### Issue 1: Live Coding Button Disabled/Grayed Out

**Symptom:** "Compile" button in editor appears inactive or grayed out.

**Causes:**
- Live Coding not enabled in editor preferences
- Project not compiled with development configuration
- Missing Visual Studio or compiler

**Fix:**

1. **Enable Live Coding in Editor:**
   - `Tools` → `Editor Preferences` → Search "Live Coding"
   - ✅ Enable "Live Coding"
   - ✅ Enable "Live Coding with Engine Changes"
   - Restart editor

2. **Verify Compiler Availability:**
   ```powershell
   # Check if Visual Studio compiler exists (Live Coding requires MSVC)
   if (Test-Path "C:\Program Files (x86)\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC") {
       Write-Host "✅ Visual Studio compiler found"
   } else {
       Write-Host "❌ Visual Studio not found - install Community or Professional edition"
   }
   ```

3. **Verify Development Build:**
   ```powershell
   # Check project was built as Development (required for Live Coding)
   ls -Path "c:\Unreal_Projects\Adventure\Binaries\Win64" -Filter "*Development*"
   ```
   You should see:
   - `UnrealEditor.exe` ✅
   - `UE4Editor-Adventure-Win64-Development.dll` ✅

---

### Issue 2: Compilation Fails After Code Changes

**Symptom:** Edit code, click "Compile" → Error in Output Log

**Common Errors:**
- `LNK2005: Already defined` → Module conflict
- `Cannot find unresolved external symbol` → Missing dependency
- `No suitable member function` → Header mismatch

**Fix:**

1. **Verify Build.cs Module Dependencies:**
   
   Current: [`Source/Adventure/Adventure.Build.cs`](../Source/Adventure/Adventure.Build.cs)
   
   Check that all public dependencies are declared:
   ```csharp
   PublicDependencyModuleNames.AddRange(new string[] { 
       "Core",
       "CoreUObject", 
       "Engine",
       "GameplayAbilities",  // ✅ Required for GAS
       "GameplayTags",       // ✅ Required for ability tags
       "EnhancedInput",      // ✅ Required for input system
       "UMG"                 // ✅ Required for HUD widget
   });
   ```

2. **Check for Forward Declaration Issues:**
   
   **In Header (.h) files:**
   ```cpp
   // ✅ Good - forward declare if only using pointers
   class UAbilitySystemComponent;
   class APC_AdventureController;
   
   // ❌ Bad - would cause link errors
   // Don't use members that need full definition
   ```

3. **Verify Include Paths Match Build.cs:**
   
   Add to `Adventure.Build.cs` if headers are in subdirectories:
   ```csharp
   PublicIncludePaths.AddRange(new string[] {
       "Adventure/Public",
       "Adventure/Public/Core",
       "Adventure/Public/Character",
       "Adventure/Public/GAS",
       "Adventure/Public/UI"
   });
   
   PrivateIncludePaths.AddRange(new string[] {
       "Adventure/Private",
       "Adventure/Private/Core",
       "Adventure/Private/Character",
       "Adventure/Private/GAS"
   });
   ```

---

### Issue 3: Recompile Hangs or Takes Excessive Time

**Symptom:** Click "Compile" → freezes for 30+ seconds

**Causes:**
- Too many includes causing dependency chain
- Shader recompilation triggering
- Distributed compilation misconfigured

**Fix:**

1. **Use Precompiled Headers (PCH):**
   
   Already configured, but verify in `.Build.cs`:
   ```csharp
   PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;  // ✅ Current setting
   ```

2. **Disable Shader Recompilation During Live Coding:**
   
   Add to `Config/DefaultEngine.ini`:
   ```ini
   [/Script/Engine.RendererSettings]
   r.ShaderCompileOnLoad=0
   r.ShaderCompilerSkipCompile=1
   ```

3. **Limit Parallel Compilation:**
   
   Add to `Config/DefaultEngine.ini`:
   ```ini
   [Core.System]
   MaxParallelIncludeDependencyCompilations=2
   ```

---

### Issue 4: "Module X Built For Incompatible Engine" Error

**Symptom:** After Live Coding compile, editor crashes or shows binary mismatch error

**Cause:** Engine version mismatch or corrupt intermediate files

**Fix:**

1. **Clean Intermediate Files:**
   ```powershell
   $projectPath = "c:\Unreal_Projects\Adventure"
   $foldersToClean = "Binaries", "Intermediate", "Saved", "DerivedDataCache"
   
   foreach ($folder in $foldersToClean) {
       $path = "$projectPath\$folder"
       if (Test-Path $path) {
           Remove-Item -Path $path -Recurse -Force
           Write-Host "✅ Cleaned $folder"
       }
   }
   ```

2. **Regenerate Project Files:**
   ```powershell
   cd c:\Unreal_Projects\Adventure
   
   # Close editor first!
   
   # Regenerate VS project files
   # Method: Right-click Adventure.uproject → Generate Visual Studio project files
   # OR run:
   # "C:\Program Files\Epic Games\UE_5.7\Engine\Build\Windows\Build.bat" Adventure
   ```

3. **Full Rebuild (if needed):**
   ```powershell
   cd "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles"
   cmd /c "Build.bat -Target=AdventureEditor Win64 Development -Project=c:/Unreal_Projects/Adventure/Adventure.uproject"
   ```

---

## ✅ Live Coding Setup Checklist

- [ ] **Editor Settings**
  - [ ] Live Coding enabled in `Editor Preferences`
  - [ ] Support for Engine Changes enabled

- [ ] **Visual Studio Compiler**
  - [ ] Visual Studio 2022 Community or Professional installed
  - [ ] C++ workload selected in VS Installer
  - [ ] MSVC compiler path recognized

- [ ] **Project Build Configuration**
  - [ ] Project compiled with Development configuration
  - [ ] `Adventure.Build.cs` has all required module dependencies
  - [ ] Include paths configured correctly

- [ ] **Source Code Structure**
  - [ ] `Source/Adventure/Public/Core/` exists ✅
  - [ ] `Source/Adventure/Public/Character/` exists ✅
  - [ ] `Source/Adventure/Public/GAS/` exists ✅
  - [ ] `Source/Adventure/Public/UI/` exists ✅
  - [ ] All `.h` files have proper guards (`#pragma once`)
  - [ ] All `.cpp` files include matching `.h` file first

- [ ] **Header Dependencies**
  - [ ] No circular includes (A.h → B.h → A.h)
  - [ ] Forward declarations used for pointers when possible
  - [ ] Full includes only in `.cpp` files

- [ ] **Plugins**
  - [ ] GameplayAbilities enabled ✅
  - [ ] EnhancedInput enabled ✅
  - [ ] All enabled plugins compatible with Development build

---

## Live Coding Workflow

### ✅ Correct Process

1. **Open Editor** with project loaded
2. **Make code change** in `.cpp` or `.h`
3. **Save file** (`Ctrl+S`)
4. **Click `Compile`** button (top-right, or `Ctrl+Alt+F8`)
5. **Wait for** "Compile Complete" message
6. **Hot-reload** dlls auto-applied to running editor
7. ✅ **Changes visible immediately** (no restart needed)

### ❌ Common Mistakes

- ❌ Modifying `.uproject` or `.Build.cs` → requires full restart
- ❌ Changing header exports/visibility → may need restart
- ❌ Adding new source files → need to rescan in editor
- ❌ Modifying project settings → requires restart
- ❌ Changing plugins → requires restart

---

## Module Visibility & Export Macros

**Ensure your header files export symbols correctly:**

```cpp
// In Source/Adventure/Public/Core/GM_AdventureMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_AdventureMode.generated.h"

/**
 * Exported from Adventure module for use in other modules
 */
UCLASS()
class ADVENTURE_API AGM_AdventureMode : public AGameModeBase
{
    GENERATED_BODY()
};
```

**Key Point:** `ADVENTURE_API` macro must be used on all public classes that Live Coding needs to reload.

---

## Performance Tips for Live Coding

| Action | Impact | Time |
|--------|--------|------|
| Recompile `.cpp` only | Fastest | ~5-10s |
| Add/modify `.h` file | Medium | ~10-20s |
| Add new class | Slower | ~20-30s |
| Modify module dependencies | Restart needed | ~2-5min full rebuild |
| Change plugins | Restart needed | ~2-5min full rebuild |

**Best Practice:** Make small changes to `.cpp` files → compile frequently → stay in fast iteration loop.

---

## Verification Test

**Quick test to verify Live Coding works:**

1. Open editor with Adventure loaded
2. Open `Source/Adventure/Private/Character/CBP_AdventureCharacter.cpp`
3. Find function `CalculateMaxSpeed()`
4. Change return value:
   ```cpp
   float MaxSpeed = 600.0f; // Change from prior value
   ```
5. Save (`Ctrl+S`)
6. Click Compile button
7. Should see "Compile Complete" in ~10-15 seconds
8. ✅ Test: Launch level, move character, verify speed changed

If this works, Live Coding is operational!

---

## Troubleshooting Commands

```powershell
# Test Visual Studio compiler availability
& "C:\Program Files (x86)\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

# Check for conflicting .pdb files (lock them in Live Coding)
Get-Process | Where-Object {$_.Modules.FileName -like "*UE4Editor*"}

# Force refresh of module cache
Remove-Item -Path "c:\Unreal_Projects\Adventure\Intermediate\Build" -Recurse -Force

# Verify project file generation
Get-ChildItem "c:\Unreal_Projects\Adventure" -Filter "*.sln"
```

---

## When to Do Full Rebuild vs Live Coding

| Scenario | Action |
|----------|--------|
| Changed `.cpp` logic | Use Live Coding ✅ (10-30s) |
| Added new `.h` file | Use Live Coding ✅ (20-30s) |
| Changed class layout | Use Live Coding ✅ (20-30s) |
| Modified `Build.cs` dependencies | Full rebuild ⚠️ (5-10min) |
| Enabled/disabled plugin | Full rebuild ⚠️ (5-10min) |
| Changed preprocessor defines | Full rebuild ⚠️ (5-10min) |
| Visual Studio version changed | Full rebuild ⚠️ (from scratch) |

