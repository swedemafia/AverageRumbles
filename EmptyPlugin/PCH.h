#ifndef PCH_H
#define PCH_H
#pragma once

#define WIN32_LEAN_AND_MEAN

// Plugin-specific requirements:
#include <stdio.h>
#include <Windows.h>
// Non-specific requirements:
#include <Richedit.h>

// Plugin-specific requirements:
#include "EmulatorPlugin.h"
#include "EmulatorAPIManager.h"
#include "HookManager.h"
#include "Plugin.h"
#include "PluginSettings.h"

// Non-specific requirements:
#include "resource.h"
#include "DialogManager.h"
#include "DisplayDialog.h"

#endif
