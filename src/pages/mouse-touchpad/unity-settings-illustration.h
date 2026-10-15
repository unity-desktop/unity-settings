/* unity-settings-illustration.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_SETTINGS_TYPE_ILLUSTRATION (unity_settings_illustration_get_type ())

G_DECLARE_FINAL_TYPE (UnitySettingsIllustration, unity_settings_illustration, UNITY_SETTINGS, ILLUSTRATION, GtkWidget)

void unity_settings_illustration_set_resource     (UnitySettingsIllustration *self,
                                                   const gchar               *resource);
void unity_settings_illustration_set_check_button (UnitySettingsIllustration *self,
                                                   GtkCheckButton            *check_button);

G_END_DECLS
