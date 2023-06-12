#ifndef PLUGINSETTINGS_H
#define PLUGINSETTINGS_H
#pragma once

// Global object declarations
extern class Plugin PluginInstance;
extern class EmulatorAPIManager API;

// Plugin-specific declarations:
extern class DisplayDialogManager* DisplayDialog;
extern HANDLE MessageThread;
extern HINSTANCE LibraryInstance;

#endif