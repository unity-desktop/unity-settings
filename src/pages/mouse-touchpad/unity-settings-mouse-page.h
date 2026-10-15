/* unity-settings-mouse-page.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_SETTINGS_TYPE_MOUSE_PAGE (unity_settings_mouse_page_get_type ())

G_DECLARE_FINAL_TYPE (UnitySettingsMousePage, unity_settings_mouse_page, UNITY_SETTINGS, MOUSE_PAGE, AdwBin)

G_END_DECLS
