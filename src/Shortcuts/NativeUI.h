#pragma once
#include "utility/ConfigEvent.h"
#include <autowiring/ContextMember.h>
struct NativeUI:
  public ContextMember,
  public ConfigEvent
{
  NativeUI(void);

  /// <summary>
  /// Registers the system tray icon with the system to allow user interaction
  /// </summary>
  void ShowUI(void);

  /// <summary>
  /// Causes any previously displayed notification area UI to be destroyed
  /// </summary>
  /// <remarks>
  /// This operation is permanent and is called before application termination.  This method is idempotent.
  /// </remarks>
  void DestroyUI(void);

  /// <summary>
  /// Invoked by the UI when the user has indicated that we should quit
  /// </summary>
  void OnQuit(void);

  /// <summary>
  /// Invoked when the configuration dialog is presented
  /// </summary>
  void OnConfigUiVisible(void);

  /// <summary>
  /// Invoked when the configuration dialog is hidden
  /// </summary>
  /// <param name="bCancelled">True if the configuration dialog was dismissed via a "cancel" behavior</param>
  void OnConfigUiHidden(bool bCancelled);

  /// <summary>
  /// Invoked when a user changes a config setting from the UI.
  /// </summary>
  void OnSettingChanged(const std::string& var, bool state);
  void OnSettingChanged(const std::string& var, double value);

  /// <summary>
  /// Invoked when the user has requested that HTML help be displayed
  /// </summary>
  /// <param name="helpWith">A context clue used to indicate what the user specifically wants help with, may be nullptr</param>
  void OnShowHtmlHelp(const char* helpWith);

  /// <summary>
  /// Invoked when we want to display a toolbar notification message
  /// </summary>
  void ShowToolbarMessage(const char* title, const char* message);

  /// <summary>
  /// Causes the config events to be re-fired if a config object exists in the current context.
  /// Used only on windows since we can't autowire things.
  /// </summary>
  void RequestConfigs();

  /// <summary>
  /// Used on windows to set the user config file.
  /// </summary>
  void SetUserConfigFile(const std::string& path);

  /// <summary>
  /// Invoked when a config setting is altered from the native code.
  /// </summary>
  void ConfigChanged(const std::string& config, const json11::Json& value) override;

  /// <summary>
  /// Invoked when the launch on startup setting is changed by the user
  /// </summary>
  void OnStartupChanged(bool value);

  /// <summary>
  /// Used to query what the launch on startup setting is
  /// </summary>
  bool GetLaunchOnStartup();
};
