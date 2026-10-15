/* unity-settings-mouse-page.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-settings-mouse-page.h"

#include "unity-settings-illustration.h"

#define INPUT_SCHEMA_ID "org.wayfire.input"

struct _UnitySettingsMousePage
{
  AdwBin          parent_instance;

  GSettings      *settings;
  AdwToggleGroup *primary_toggle_group;
  GtkScale       *mouse_speed_scale;
  GtkSwitch      *mouse_accel_switch;
  GtkCheckButton *mouse_scroll_traditional;
  GtkCheckButton *mouse_scroll_natural;
};

G_DEFINE_FINAL_TYPE (UnitySettingsMousePage, unity_settings_mouse_page, ADW_TYPE_BIN)

static void
on_primary_button_changed_cb (UnitySettingsMousePage *self)
{
  const gchar *active_name = adw_toggle_group_get_active_name (self->primary_toggle_group);
  gboolean     left_handed;

  if (active_name == NULL)
    return;

  left_handed = g_strcmp0 (active_name, "right") == 0;

  if (g_settings_get_boolean (self->settings, "left-handed-mode") != left_handed)
    g_settings_set_boolean (self->settings, "left-handed-mode", left_handed);
}

static void
primary_toggle_right_click_pressed_cb (UnitySettingsMousePage *self,
                                       gint                    n_press,
                                       gdouble                 x,
                                       gdouble                 y)
{
  gdouble width = gtk_widget_get_width (GTK_WIDGET (self->primary_toggle_group));

  adw_toggle_group_set_active_name (self->primary_toggle_group, x < width / 2 ? "left" : "right");
}

static gboolean
mouse_accel_get_mapping (GValue   *value,
                         GVariant *variant,
                         gpointer  user_data)
{
  g_value_set_boolean (value, g_strcmp0 (g_variant_get_string (variant, NULL), "flat") != 0);

  return TRUE;
}

static GVariant *
mouse_accel_set_mapping (const GValue       *value,
                         const GVariantType *type,
                         gpointer            user_data)
{
  return g_variant_new_string (g_value_get_boolean (value) ? "default" : "flat");
}

static void
unity_settings_mouse_page_direction_changed (GtkWidget        *widget,
                                             GtkTextDirection  previous_direction)
{
  UnitySettingsMousePage *self = UNITY_SETTINGS_MOUSE_PAGE (widget);

  gtk_widget_set_direction (GTK_WIDGET (self->primary_toggle_group), GTK_TEXT_DIR_LTR);

  GTK_WIDGET_CLASS (unity_settings_mouse_page_parent_class)->direction_changed (widget, previous_direction);
}

static void
unity_settings_mouse_page_dispose (GObject *object)
{
  UnitySettingsMousePage *self = UNITY_SETTINGS_MOUSE_PAGE (object);

  gtk_widget_dispose_template (GTK_WIDGET (self), UNITY_SETTINGS_TYPE_MOUSE_PAGE);
  g_clear_object (&self->settings);

  G_OBJECT_CLASS (unity_settings_mouse_page_parent_class)->dispose (object);
}

static void
unity_settings_mouse_page_class_init (UnitySettingsMousePageClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_settings_mouse_page_dispose;
  widget_class->direction_changed = unity_settings_mouse_page_direction_changed;

  g_type_ensure (UNITY_SETTINGS_TYPE_ILLUSTRATION);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/Settings/unity-settings-mouse-page.ui");
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsMousePage, primary_toggle_group);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsMousePage, mouse_speed_scale);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsMousePage, mouse_accel_switch);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsMousePage, mouse_scroll_traditional);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsMousePage, mouse_scroll_natural);
  gtk_widget_class_bind_template_callback (widget_class, on_primary_button_changed_cb);
  gtk_widget_class_bind_template_callback (widget_class, primary_toggle_right_click_pressed_cb);
}

static void
unity_settings_mouse_page_init (UnitySettingsMousePage *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  self->settings = g_settings_new (INPUT_SCHEMA_ID);

  adw_toggle_group_set_active_name (self->primary_toggle_group,
                                    g_settings_get_boolean (self->settings, "left-handed-mode") ? "right" : "left");

  g_settings_bind (self->settings, "mouse-cursor-speed",
                   gtk_range_get_adjustment (GTK_RANGE (self->mouse_speed_scale)), "value",
                   G_SETTINGS_BIND_DEFAULT);
  g_settings_bind_with_mapping (self->settings, "mouse-accel-profile", self->mouse_accel_switch, "active",
                                G_SETTINGS_BIND_DEFAULT, mouse_accel_get_mapping, mouse_accel_set_mapping, NULL, NULL);

  if (g_settings_get_boolean (self->settings, "mouse-natural-scroll"))
    gtk_check_button_set_active (self->mouse_scroll_natural, TRUE);
  else
    gtk_check_button_set_active (self->mouse_scroll_traditional, TRUE);

  g_settings_bind (self->settings, "mouse-natural-scroll", self->mouse_scroll_natural, "active",
                   G_SETTINGS_BIND_DEFAULT);
}
