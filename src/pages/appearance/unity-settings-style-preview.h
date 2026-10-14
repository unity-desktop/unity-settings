/* unity-settings-style-preview.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>
#include <gdesktop-enums.h>

#include "cc-background-item.h"

G_BEGIN_DECLS

#define UNITY_SETTINGS_TYPE_STYLE_PREVIEW (unity_settings_style_preview_get_type ())

G_DECLARE_FINAL_TYPE (UnitySettingsStylePreview, unity_settings_style_preview, UNITY_SETTINGS, STYLE_PREVIEW, AdwBin)

void unity_settings_style_preview_set_item         (UnitySettingsStylePreview *self,
                                                    CcBackgroundItem          *item);

void unity_settings_style_preview_set_color_scheme (UnitySettingsStylePreview *self,
                                                    GDesktopColorScheme        color_scheme);

G_END_DECLS
