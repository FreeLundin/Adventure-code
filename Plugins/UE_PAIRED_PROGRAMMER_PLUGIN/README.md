# UE MCP Agent Plugin Quick Start

This plugin implements the Model Context Protocol (MCP) for Unreal Engine.
It exposes a small HTTP server and a set of programmable tools that can be
invoked by remote agents or via the in-editor console.

## Getting started

1. **Enable the plugin** in your project (`Edit > Plugins > MCP Agent`).
2. **Open the dashboard**: `Window > Developer Tools > MCP Agent Dashboard`.
   The dashboard lists registered tools and has buttons for manual actions.
3. **Configure settings**:
   * Navigate to `Edit > Project Settings > MCP Agent`.
   * Set `BaseUrl` and `ApiKey` (these are written to `DefaultEngine.ini`).
   * Optionally clear `bAutoStart` (in the same settings page) if you prefer
     to delay the server/connectivity initialization until you actually invoke
     a tool.  This speeds up editor launch when you don't need the agent right
     away.
4. **Try the built-in tools**:
   * Press *Refresh Tools* in the dashboard to see available tools.
   * Use the HTTP interface or the console command:
     ```
     tool.perform name=telemetry.report params={"payload":"{\"hello\":true}"}
     ```
5. **Create an example Blueprint** (optional):
   * Run the `onboard.createExample` tool from the dashboard or via JSON‑RPC.
   * A new actor blueprint `BP_MCPExample` will be created in `/Game/MCPExamples`.
   * Open it and inspect the Event Graph to see a placeholder node where you
     can add `Report Telemetry` calls.

## Additional resources

* See `Documentation/AIIntegrationGuide.md` for detailed usage patterns.
* Use the Python script in `Tools/BlueprintAutomation` for batch operations.
* Consult the `ToolRegistry` C++ class if you need to add your own tools.

The plugin aims to be self‑contained and usable by any developer in the studio;
no game‑specific code is required beyond enabling the plugin and optionally
calling its API from Blueprints or C++.