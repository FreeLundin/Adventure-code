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
