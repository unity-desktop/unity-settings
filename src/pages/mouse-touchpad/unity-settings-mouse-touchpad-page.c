/* unity-settings-mouse-touchpad-page.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-settings-mouse-touchpad-page.h"

#include "unity-settings-mouse-page.h"
#include "unity-settings-touchpad-page.h"

struct _UnitySettingsMouseTouchpadPage
{
  AdwBin parent_instance;
};

G_DEFINE_FINAL_TYPE (UnitySettingsMouseTouchpadPage, unity_settings_mouse_touchpad_page, ADW_TYPE_BIN)

static void
unity_settings_mouse_touchpad_page_dispose (GObject *object)
{
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_SETTINGS_TYPE_MOUSE_TOUCHPAD_PAGE);

  G_OBJECT_CLASS (unity_settings_mouse_touchpad_page_parent_class)->dispose (object);
}

static void
unity_settings_mouse_touchpad_page_class_init (UnitySettingsMouseTouchpadPageClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_settings_mouse_touchpad_page_dispose;

  g_type_ensure (UNITY_SETTINGS_TYPE_MOUSE_PAGE);
  g_type_ensure (UNITY_SETTINGS_TYPE_TOUCHPAD_PAGE);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/Settings/unity-settings-mouse-touchpad-page.ui");
}

static void
unity_settings_mouse_touchpad_page_init (UnitySettingsMouseTouchpadPage *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}
