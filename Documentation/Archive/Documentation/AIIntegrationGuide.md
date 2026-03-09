# AI Integration Guide

This document summarizes the AI/ML integration patterns used in the *Adventure* project.
It collects example code, loading scenarios, and workflow notes so that other
developers can replicate the setup.

## Overview

We use Unreal Engine 5.7's Neural Network Execution (NNE) framework to load
and run ONNX models at runtime. The project also integrates the UE MCP Agent
plugin for telemetry and canary control (see `Governance.md`).

The key component is `UModelAgentComponent` (in `Source/AIIntegration`), which
provides Blueprint-callable helpers for loading models from URLs, running
inference, and reporting telemetry. A standalone actor example (*AMyActor*)
implements a complete workflow as described below.

## Model Loading Approaches

1. **Manual** – `LoadObject<UNNEModelData>(...)` with a hard-coded path.
   Useful for quick experiments but not recommended for production.
2. **Preloaded** – `UPROPERTY(EditAnywhere) TObjectPtr<UNNEModelData> PreLoadedModelData;`
   Assign the asset via the editor; the engine keeps it loaded as long as the
   owning actor/module exists.
3. **Lazy (soft) load** – `TSoftObjectPtr<UNNEModelData> LazyLoadedModelData;`
   Use `UAssetManager::GetStreamableManager().RequestAsyncLoad(...)` to
   retrieve the asset on demand, then reset the pointer after use.

## Runtime & Inference

- Query the desired runtime at startup: `UE::NNE::GetRuntime<INNERuntimeCPU>(TEXT("NNERuntimeORTCpu"))`.
- Create a model from the data blob and then a model instance.
- Perform shape validation and call `SetInputTensorShapes`/`SetOutputTensorShapes`.
- Allocate `TArray<float>` buffers based on `FTensorShape::Volume()` and
  populate `FTensorBindingCPU` structures pointing to them.
- Optionally run inference asynchronously using `AsyncTask` to avoid blocking
the game thread. The example sets `bIsRunning` flags and posts completion
notifications back to the game thread.

Supported models in the example are single-input/single-output, fixed-size
tensors; you can extend the code to handle multiple or dynamic shapes.

## MCP Telemetry & Canary

The UE Paired Programmer (formerly "UE MCP Agent") plugin is enabled (see `Adventure.uproject`). Configure the
base URL and API key in `DefaultEngine.ini`:

```ini
[/Script/UE_MCP.AgentSettings]
BaseUrl=https://mcp.example.com/api
ApiKey=YOUR_KEY_HERE
```

#### Sending telemetry

> **Tip:** set `bAutoStart=false` in `/Script/UE_MCP.AgentSettings` if you
> want the plugin to defer network/server initialization until a tool is
> executed. This can speed up editor launch on machines that only occasionally
> need the agent.


You can report arbitrary JSON telemetry in two ways:

* **C++** – call the helper:
  ```cpp
  UMcpTelemetryLibrary::ReportTelemetry("{\"event\":\"jump\",\"latency\":123}");
  ```
* **Blueprint** – drop the `Report Telemetry` node (category `MCP\Telemetry`).
  A multicast `OnTelemetrySent` delegate fires when the HTTP request completes.

The plugin also exposes an MCP tool that remote agents can call:

```json
{
  "jsonrpc":"2.0","id":1,"method":"tool.perform",
  "params":{
     "name":"telemetry.report",
     "params":{ "payload":"{\"outcome\":\"ok\"}" }
  }
}
```

This is useful during automated runs where the game client is controlled by an
MCP agent rather than a human.

The `ai/canary/canary_control.sh` script, plus CI workflow
`.github/workflows/ai_canary.yml`, automate canary rollout and rollback
decisions based on telemetry.

## Developer & Designer Workflow

The following steps assume you are *not* a programmer – the UE MCP Agent acts
as your senior engineer/paired programmer. Use the editor dashboard and
built‑in tools to prototype AI features, wire inputs, and emit telemetry
without writing C++ or Blueprints.

1. Launch the editor and open **Window › Developer Tools › MCP Agent Dashboard**.
2. Click **Create Example Blueprint** to generate a starter actor with a
   `Report Telemetry` node already set up. This provides a working example
   you can duplicate or inspect.
3. To hook up an ability or input action, choose the corresponding tool
   (e.g. `blueprint.bindTraversal`) from the dashboard and follow the prompt
   – the agent will modify the specified blueprint(s) for you.
4. Enter a short description of your game and press **Suggest Features** to
   receive gameplay ideas; the agent returns a human‑readable list.
5. When ready, hit **Send Test Telemetry** to dispatch a sample payload using
   the configured endpoint. No networking code is required on your part.
6. Proceed with training or adjusting your ONNX model: cook it, assign it to
   an actor (drag‑and‑drop in the editor), and the runtime component does the
   rest.
7. Use the CI Canary workflow as usual; telemetry is collected automatically
   by the example blueprint and any actors you create.

These steps let designers and non‑technical collaborators iterate quickly on
vertical slices. The agent handles all low‑level details; you simply focus on
what you want the game to do.

### Backlog support
For product or sprint planning, use the backlog panel in the MCP Dashboard. Add
items to the list and refresh to see current entries; the plugin saves the
backlog to `Saved/MCPBacklog.json`. This lets you track feature requests or
tasks without leaving the editor.

The plugin is often referred to in documentation as the **"Paired Programmer"**
plugin, emphasising that it automates the engineer role during prototyping.

## Recommended Practices

- Add `NNERuntime` (or specific backends) to `AIIntegration.Build.cs`.
- Keep all ML code confined to `Source/AIIntegration` or similar module.
- Add proper error handling and avoid `checkf` in shipping builds.

### Production telemetry notes

The shipped `UMcpTelemetryLibrary` now implements basic retry/backoff and
queues payloads when configuration is missing.  For enterprise rollout you
should further enhance this with:

  * Offline buffering to disk when the network is unavailable.
  * Rate limiting to avoid throttling your MCP backend under spike loads.
  * Monitoring of the `OnTelemetrySent` delegate for failures.

An empty or mis‑configured `BaseUrl`/`ApiKey` no longer crashes; the system
logs a warning and queues the payload for later.

### Security & approval

MCP supports capability‑based tokens; however the plugin exposes tools by
default (e.g. `actors.spawn`, `blueprint.bindTraversal`, `telemetry.report`).
Before deploying in production audit the list via the dashboard and use your
connector/aggregator policy engine to whitelist only the tools you intend to
expose.  Sensitive operations such as asset mutation should require elevated
scopes and manual human review.

### Testing & validation

Add unit/integration tests covering each MCP tool.  The tools are plain C++
functions, so write simple `AutomationSpec` tests that call
`FToolRegistry::ExecuteTool` with a fake blueprint asset or payload and
assert expected results.  This ensures future refactors don’t silently break
automation.

### Documentation & training

Circulate this guide and the runbooks among your teams.  A short demo (5–10
minutes) showing the MCP Dashboard, invoking `blueprint.bindTraversal`, and
sending telemetry will dramatically lower onboarding friction.  Record the
demo video and link it from `AIIntegrationGuide.md`.

- Monitor `ai/logs/` for raw telemetry and use the Flask ingestion prototype
  (`ai/mcp/telemetry_ingest.py`) only for local testing.
- Document model provenance using the `AI/templates/ModelCard.md` template.


## MCP Plugin Tools

Two utility tools are installed by the MCP plugin. They can be invoked from
within the editor via the MCP console or remotely through the agent's HTTP
API.

* `blueprint.bindTraversal` – automatically adds an input-action event and a
  call to `TryActivateAbilityByTag(Ability.Traversal)` to one or more
  blueprints. Parameters:
  ```json
  {"blueprints":["/Game/Blueprints/CBP_AdventureCharacter"],
   "inputAction":"/Game/Input/IA_HeavyAttack",
   "abilityTag":"Ability.Traversal"}
  ```
* `telemetry.report` – sends a JSON payload to the configured telemetry
  endpoint. Parameter:
  ```json
  {"payload":"{\"key\":\"value\"}"}
  ```
* `onboard.createExample` – creates a new actor blueprint (`/Game/MCPExamples/BP_MCPExample`)
  that you can open to see how to call `Report Telemetry`. Useful for onboarding
  new team members or regenerating the sample asset.
* `gameplay.suggestFeatures` – ask the agent for gameplay feature ideas. Supply a
  100‑word (or less) description of the type of game you’re building and the
  tool will return a short list of suggested mechanics.


### Asking for feature ideas

The MCP Dashboard includes a "Suggest Features" box where you can type a brief
summary of your game (e.g. "A 3rd‑person stealth thriller set in a neon city").
Press the button and the plugin will call `gameplay.suggestFeatures` and show
returned ideas. This is useful when brainstorming with designers or when new
developers need inspiration.

These tools eliminate repetitive wiring and let build/release automation
modify assets programmatically.

---

This guide should be referenced whenever new models or runtimes are integrated
into the project; it lives under the `Documentation/` folder and is included
in the repository.

This guide should be referenced whenever new models or runtimes are integrated
into the project; it lives under the `Documentation/` folder and is included
in the repository.
