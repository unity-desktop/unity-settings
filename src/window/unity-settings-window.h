/* unity-settings-window.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_SETTINGS_TYPE_WINDOW (unity_settings_window_get_type ())

G_DECLARE_FINAL_TYPE (UnitySettingsWindow, unity_settings_window, UNITY_SETTINGS, WINDOW, AdwApplicationWindow)

GtkWindow *unity_settings_window_new (GtkApplication *application);

G_END_DECLS
