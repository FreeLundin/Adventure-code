# Lessons Learned

## 2026-03-01: Editor launch failure fixed

### Symptoms
- Attempting to open `Adventure.uproject` in Unreal Editor resulted in an "Incompatible or missing module: Adventure" error.
- Review of `Saved/Logs/Adventure.log` showed that the build system was trying to compile the `Adventure` module and failing during the build process.

### Root Causes
1. **Module header not on include path**: `Adventure.h` lived at the root of `Source/Adventure` instead of `Source/Adventure/Public`. Other headers included it with `#include "Adventure.h"`, causing `fatal error C1083` when compiling.
2. **Stubbed generated header checked in**: A minimal stub of `AdventureBaseAbility.generated.h` existed under `Source/Adventure/Public/Gameplay`. It defined UHT macros as empty, so the compiler processed the class declaration incorrectly, leading to a cascade of syntax errors during UnrealBuildTool execution.

### Fixes Applied
- Moved `Adventure.h` into `Source/Adventure/Public` and updated include paths if necessary.
- Deleted the bogus `*.generated.h` file; the real generated file is produced in `Intermediate` by UHT.
- Cleaned and rebuilt the project; build succeeded, and the editor launched.

### Recommendations
- Never commit `*.generated.h` files. Add a `.gitignore` rule if needed to prevent accidental check-in.
- Keep public module headers inside `Public/` so they are automatically found by the build system.
- When encountering "incompatible module" errors, inspect the log for build failures and look for missing includes or stub headers.
- Regularly clean `Intermediate`/`Binaries` before major upgrades or after moving files to remove stale artifacts.

### Next Steps
- Add a Git pre‑commit hook or CI check to block `*.generated.h` from being committed.
- Review other modules for misplaced headers.
- Document above in this file for team reference.

## 2026-03-09: Tooling & workflow enhancements
### Integration tools evaluated
- **Microsoft 365** (Teams, Planner, OneDrive, Power Automate) was recommended for cross‑discipline communication, task tracking and asset storage. Set up a Planner board aliased to our GitHub issues and a Teams channel for CI/PR notifications to centralize collaboration.
- **Nsight Extensions** for Visual Studio/VS Code provide GPU frame capture, shader debugging and system‑wide profiling. Install on all artist/engineer workstations to diagnose rendering bottlenecks and compute kernels.
- **NVIDIA Omniverse** offers USD‑based live‑sync between DCC tools and Unreal; consider proof‑of‑concept sessions to allow artists and designers to work on the same scene without constant exports.

### Lessons learned implementing documentation cleanup
- Be careful with `git mv` paths; earlier attempts inadvertently staged deletions of many docs. Always verify `git status` before committing moves.
- Centralize project‑level documents under a `Documentation/` folder and archive outdated design notes in `Documentation/Archive` to keep the repo tidy and reduce noise for code‑review tools.

### Recommendations
- Create templates in OneDrive for design documents and link them from README so authors use consistent structure.
- Add a CI check that warns if Nsight or Omniverse plugins are missing from engine builds, to keep toolchain aligned.
- Periodically review and prune the `Documentation/Archive` directory to avoid accumulating stale content.
