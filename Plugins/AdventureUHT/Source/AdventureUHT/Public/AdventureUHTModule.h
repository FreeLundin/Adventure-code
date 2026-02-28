#pragma once

#include "Modules/ModuleManager.h"

/**
 * AdventureUHT module stub.  You can register custom UHT delegates here
 * (e.g. FHeaderParser::OnParseClass.AddRaw(...)) to enforce project‑specific rules.
 */
class FAdventureUHTModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
