/* unity-settings-sidebar-item.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_SETTINGS_TYPE_SIDEBAR_ITEM (unity_settings_sidebar_item_get_type ())

G_DECLARE_FINAL_TYPE (UnitySettingsSidebarItem, unity_settings_sidebar_item, UNITY_SETTINGS, SIDEBAR_ITEM, AdwSidebarItem)

GType unity_settings_sidebar_item_get_page_type (UnitySettingsSidebarItem *self);

G_END_DECLS
