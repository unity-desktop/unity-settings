/* unity-settings-mouse-touchpad-page.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_SETTINGS_TYPE_MOUSE_TOUCHPAD_PAGE (unity_settings_mouse_touchpad_page_get_type ())

G_DECLARE_FINAL_TYPE (UnitySettingsMouseTouchpadPage, unity_settings_mouse_touchpad_page, UNITY_SETTINGS, MOUSE_TOUCHPAD_PAGE, AdwBin)

G_END_DECLS
