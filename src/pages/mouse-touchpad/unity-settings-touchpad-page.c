/* unity-settings-touchpad-page.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-settings-touchpad-page.h"

#include "unity-settings-illustration.h"
#include "unity-settings-touchpad-details.h"

#define INPUT_SCHEMA_ID "org.wayfire.input"

struct _UnitySettingsTouchpadPage
{
  AdwBin                     parent_instance;

  GSettings                 *settings;
  AdwSwitchRow              *touchpad_typing_row;
  AdwSwitchRow              *touchpad_external_mouse_row;
  GtkScale                  *touchpad_speed_scale;
  GtkSwitch                 *click_method_switch;
  UnitySettingsIllustration *click_method_default_image;
  GtkSwitch                 *scroll_method_switch;
  UnitySettingsIllustration *scroll_method_default_image;
  GtkCheckButton            *touchpad_scroll_traditional;
  GtkCheckButton            *touchpad_scroll_natural;
  GtkSwitch                 *tap_and_drag_switch;
  GtkCheckButton            *drag_lock_off;
  GtkCheckButton            *drag_lock_on;
  GtkSwitch                 *finger_drag_switch;
};

G_DEFINE_FINAL_TYPE (UnitySettingsTouchpadPage, unity_settings_touchpad_page, ADW_TYPE_BIN)

static gboolean
method_enabled_get_mapping (GValue   *value,
                            GVariant *variant,
                            gpointer  user_data)
{
  g_value_set_boolean (value, g_strcmp0 (g_variant_get_string (variant, NULL), "none") != 0);

  return TRUE;
}

static GVariant *
method_enabled_set_mapping (const GValue       *value,
                            const GVariantType *type,
                            gpointer            user_data)
{
  return g_variant_new_string (g_value_get_boolean (value) ? "default" : "none");
}

static gboolean
finger_drag_enabled_get_mapping (GValue   *value,
                                 GVariant *variant,
                                 gpointer  user_data)
{
  const gchar *fingers = g_variant_get_string (variant, NULL);

  g_value_set_boolean (value, g_str_equal (fingers, "3fg-drag") || g_str_equal (fingers, "4fg-drag"));

  return TRUE;
}

static GVariant *
finger_drag_enabled_set_mapping (const GValue       *value,
                                 const GVariantType *type,
                                 gpointer            user_data)
{
  return g_variant_new_string (g_value_get_boolean (value) ? "3fg-drag" : "none");
}

static void
show_default_methods (UnitySettingsTouchpadPage *self)
{
  g_autofree gchar *click  = NULL;
  g_autofree gchar *scroll = NULL;

  if (!unity_settings_touchpad_details_get_defaults (&click, &scroll))
    return;

  if (g_str_equal (click, "clickfinger"))
    unity_settings_illustration_set_resource (self->click_method_default_image,
                                              "/org/unity/Settings/mouse-touchpad/two-finger-push.svg");

  if (g_str_equal (scroll, "edge"))
    unity_settings_illustration_set_resource (self->scroll_method_default_image,
                                              "/org/unity/Settings/mouse-touchpad/edge-scroll.svg");
}

static void
unity_settings_touchpad_page_dispose (GObject *object)
{
  UnitySettingsTouchpadPage *self = UNITY_SETTINGS_TOUCHPAD_PAGE (object);

  gtk_widget_dispose_template (GTK_WIDGET (self), UNITY_SETTINGS_TYPE_TOUCHPAD_PAGE);
  g_clear_object (&self->settings);

  G_OBJECT_CLASS (unity_settings_touchpad_page_parent_class)->dispose (object);
}

static void
unity_settings_touchpad_page_class_init (UnitySettingsTouchpadPageClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_settings_touchpad_page_dispose;

  g_type_ensure (UNITY_SETTINGS_TYPE_ILLUSTRATION);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/Settings/unity-settings-touchpad-page.ui");
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsTouchpadPage, touchpad_typing_row);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsTouchpadPage, touchpad_external_mouse_row);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsTouchpadPage, touchpad_speed_scale);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsTouchpadPage, click_method_switch);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsTouchpadPage, click_method_default_image);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsTouchpadPage, scroll_method_switch);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsTouchpadPage, scroll_method_default_image);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsTouchpadPage, touchpad_scroll_traditional);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsTouchpadPage, touchpad_scroll_natural);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsTouchpadPage, tap_and_drag_switch);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsTouchpadPage, drag_lock_off);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsTouchpadPage, drag_lock_on);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsTouchpadPage, finger_drag_switch);
}

static void
unity_settings_touchpad_page_init (UnitySettingsTouchpadPage *self)
{
  g_autoptr (GSimpleActionGroup) group         = g_simple_action_group_new ();
  g_autoptr (GAction)            click_method  = NULL;
  g_autoptr (GAction)            scroll_method = NULL;
  g_autoptr (GAction)            finger_drag   = NULL;
  g_autoptr (GAction)            tap_to_click  = NULL;

  gtk_widget_init_template (GTK_WIDGET (self));

  self->settings = g_settings_new (INPUT_SCHEMA_ID);
  show_default_methods (self);

  g_settings_bind (self->settings, "disable-touchpad-while-typing", self->touchpad_typing_row, "active",
                   G_SETTINGS_BIND_DEFAULT);
  g_settings_bind (self->settings, "disable-touchpad-while-mouse", self->touchpad_external_mouse_row, "active",
                   G_SETTINGS_BIND_DEFAULT);
  g_settings_bind (self->settings, "touchpad-cursor-speed",
                   gtk_range_get_adjustment (GTK_RANGE (self->touchpad_speed_scale)), "value",
                   G_SETTINGS_BIND_DEFAULT);
  g_settings_bind_with_mapping (self->settings, "click-method", self->click_method_switch, "active",
                                G_SETTINGS_BIND_DEFAULT, method_enabled_get_mapping, method_enabled_set_mapping,
                                NULL, NULL);
  g_settings_bind_with_mapping (self->settings, "scroll-method", self->scroll_method_switch, "active",
                                G_SETTINGS_BIND_DEFAULT, method_enabled_get_mapping, method_enabled_set_mapping,
                                NULL, NULL);
  /* As gnome-control-center: check the radio once, then bind only the "true" one. */
  if (g_settings_get_boolean (self->settings, "natural-scroll"))
    gtk_check_button_set_active (self->touchpad_scroll_natural, TRUE);
  else
    gtk_check_button_set_active (self->touchpad_scroll_traditional, TRUE);

  g_settings_bind (self->settings, "natural-scroll", self->touchpad_scroll_natural, "active",
                   G_SETTINGS_BIND_DEFAULT);
  g_settings_bind (self->settings, "tap-and-drag", self->tap_and_drag_switch, "active", G_SETTINGS_BIND_DEFAULT);

  if (g_settings_get_boolean (self->settings, "drag-lock"))
    gtk_check_button_set_active (self->drag_lock_on, TRUE);
  else
    gtk_check_button_set_active (self->drag_lock_off, TRUE);

  g_settings_bind (self->settings, "drag-lock", self->drag_lock_on, "active", G_SETTINGS_BIND_DEFAULT);
  g_settings_bind_with_mapping (self->settings, "x-3fg-drag", self->finger_drag_switch, "active",
                                G_SETTINGS_BIND_DEFAULT, finger_drag_enabled_get_mapping,
                                finger_drag_enabled_set_mapping, NULL, NULL);

  click_method = g_settings_create_action (self->settings, "click-method");
  scroll_method = g_settings_create_action (self->settings, "scroll-method");
  g_action_map_add_action (G_ACTION_MAP (group), click_method);
  finger_drag = g_settings_create_action (self->settings, "x-3fg-drag");
  g_action_map_add_action (G_ACTION_MAP (group), scroll_method);
  g_action_map_add_action (G_ACTION_MAP (group), finger_drag);
  tap_to_click = g_settings_create_action (self->settings, "tap-to-click");
  g_action_map_add_action (G_ACTION_MAP (group), tap_to_click);
  gtk_widget_insert_action_group (GTK_WIDGET (self), "touchpad", G_ACTION_GROUP (group));
}
