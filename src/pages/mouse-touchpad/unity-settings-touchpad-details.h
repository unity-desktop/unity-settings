/* unity-settings-touchpad-details.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <glib.h>

G_BEGIN_DECLS

gboolean unity_settings_touchpad_details_get_defaults (gchar **click_method,
                                                      gchar **scroll_method);

G_END_DECLS
