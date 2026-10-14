/* unity-settings-appearance-page.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_SETTINGS_TYPE_APPEARANCE_PAGE (unity_settings_appearance_page_get_type ())

G_DECLARE_FINAL_TYPE (UnitySettingsAppearancePage, unity_settings_appearance_page, UNITY_SETTINGS, APPEARANCE_PAGE, AdwBin)

G_END_DECLS
