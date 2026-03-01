# Adventure

This repository contains several planning and governance documents to help with development.  Key files include:

- `ISSUES.md` – actionable issue list derived from the implementation roadmap.
- `TECH_DEBT.md` – current technical debt register.
- `LESSONS_LEARNED.md` – post-mortem notes.
- `runbooks/CULTURAL_REVIEW_CHECKLIST.md` – guidelines for Orisha-based content.

See the `Design Document/Implementation` folder for the full original plan.


For information on the C++/Blueprint hybrid workflow, macros, tooling and
project conventions, see [Documentation/HybridWorkflow.md](Documentation/HybridWorkflow.md).


This repository contains the Adventure prototype built on Unreal Engine 5.7.
The game is an experimental sample demonstrating:

- Gameplay Ability System (GAS) integration
- Motion-matching locomotion and combat
- Enhanced Input and custom camera modes
- Python editor tooling for asset pipelines

## Getting Started
1. Open `Adventure.uproject` using Unreal Engine 5.7.
2. Build the project with `Build.bat` (see tasks in VS Code for common targets).
3. Refer to the documentation under `Design Document/` for design notes, plans, and runbooks.
   - A concise index is available at `Design Document/INDEX.md`.
   - Core planning docs (`Product_Backlog.md`, `PROJECT_STATUS.md`, etc.) are located at the root of the design document folder.

## Documentation
All design documentation, runbooks, and scripts live under the `Design Document/` folder.
See `Design Document/README.md` for an overview and quick start.

## Contributing
1. Run `Design Document/PROJECT_STATUS.md` to find current tasks and priorities.
2. Create issues using the templates in `Design Document/issues/` and link them back to the backlog.
3. Follow the governance guidelines in `Design Document/Governance/`.

## License
[Specify license here if applicable]
