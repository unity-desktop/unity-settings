/* unity-settings-appearance-page.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-settings-appearance-page.h"

#include <glib/gi18n.h>

#include "cc-background-chooser.h"
#include "unity-settings-style-preview.h"

#define INTERFACE_SCHEMA_ID   "org.gnome.desktop.interface"
#define BACKGROUND_SCHEMA_ID  "org.gnome.desktop.background"
#define STYLESHEETS_SCHEMA_ID "org.unity.stylesheets"

struct _UnitySettingsAppearancePage
{
  AdwBin                     parent_instance;

  GSettings                 *interface;
  GSettings                 *background;
  CcBackgroundItem          *current;
  UnitySettingsStylePreview *preview;
  AdwComboRow               *style_row;
  AdwSwitchRow              *translucency_row;
  GtkLabel                  *accent_label;
  CcBackgroundChooser       *chooser;
};

G_DEFINE_FINAL_TYPE (UnitySettingsAppearancePage, unity_settings_appearance_page, ADW_TYPE_BIN)

/* Color names, as in gnome-control-center. */
static const gchar *
get_accent_label (GDesktopAccentColor color)
{
  switch (color)
    {
    case G_DESKTOP_ACCENT_COLOR_BLUE:
      return _("Blue");
    case G_DESKTOP_ACCENT_COLOR_TEAL:
      return _("Teal");
    case G_DESKTOP_ACCENT_COLOR_GREEN:
      return _("Green");
    case G_DESKTOP_ACCENT_COLOR_YELLOW:
      return _("Yellow");
    case G_DESKTOP_ACCENT_COLOR_ORANGE:
      return _("Orange");
    case G_DESKTOP_ACCENT_COLOR_RED:
      return _("Red");
    case G_DESKTOP_ACCENT_COLOR_PINK:
      return _("Pink");
    case G_DESKTOP_ACCENT_COLOR_PURPLE:
      return _("Purple");
    case G_DESKTOP_ACCENT_COLOR_SLATE:
      return _("Slate");
    }

  g_assert_not_reached ();
}

static void
reload_color_scheme (UnitySettingsAppearancePage *self)
{
  GDesktopColorScheme scheme = g_settings_get_enum (self->interface, "color-scheme");

  adw_combo_row_set_selected (self->style_row, scheme);
  unity_settings_style_preview_set_color_scheme (self->preview, scheme);
}

static void
reload_accent_color (UnitySettingsAppearancePage *self)
{
  gtk_label_set_label (self->accent_label, get_accent_label (g_settings_get_enum (self->interface, "accent-color")));
}

static void
style_selected_cb (UnitySettingsAppearancePage *self)
{
  GDesktopColorScheme scheme = adw_combo_row_get_selected (self->style_row);

  if (scheme != (GDesktopColorScheme) g_settings_get_enum (self->interface, "color-scheme"))
    g_settings_set_enum (self->interface, "color-scheme", scheme);
}

/* Builds the current background from the settings, as gnome-control-center does. */
static void
reload_current (UnitySettingsAppearancePage *self)
{
  g_autofree gchar *uri      = g_settings_get_string (self->background, "picture-uri");
  g_autofree gchar *uri_dark = g_settings_get_string (self->background, "picture-uri-dark");
  g_autofree gchar *pcolor   = g_settings_get_string (self->background, "primary-color");
  g_autofree gchar *scolor   = g_settings_get_string (self->background, "secondary-color");

  g_clear_object (&self->current);
  self->current = cc_background_item_new (*uri != '\0' ? uri : NULL);
  g_object_set (self->current,
                "name", _("Current background"),
                "uri-dark", uri_dark,
                "placement", g_settings_get_enum (self->background, "picture-options"),
                "shading", g_settings_get_enum (self->background, "color-shading-type"),
                "primary-color", pcolor,
                "secondary-color", scolor,
                NULL);
  cc_background_item_load (self->current, NULL);

  cc_background_chooser_set_active_item (self->chooser, self->current);
  unity_settings_style_preview_set_item (self->preview, self->current);
}

static gboolean
background_changed_cb (UnitySettingsAppearancePage *self)
{
  reload_current (self);

  return FALSE;
}

/* Writes all keys in one apply, so the page reloads once. */
static void
background_chosen_cb (UnitySettingsAppearancePage *self,
                      CcBackgroundItem            *item)
{
  g_autoptr (GSettings) settings = g_settings_new (BACKGROUND_SCHEMA_ID);
  const gchar          *uri      = cc_background_item_get_uri (item);
  const gchar          *uri_dark = cc_background_item_get_uri_dark (item);
  CcBackgroundItemFlags flags    = cc_background_item_get_flags (item);

  g_settings_delay (settings);

  g_settings_set_string (settings, "picture-uri", uri);
  g_settings_set_string (settings, "picture-uri-dark", uri_dark != NULL && *uri_dark != '\0' ? uri_dark : uri);

  if ((flags & CC_BACKGROUND_ITEM_HAS_PLACEMENT) ||
      (uri != NULL && g_settings_get_enum (settings, "picture-options") == G_DESKTOP_BACKGROUND_STYLE_NONE))
    g_settings_set_enum (settings, "picture-options", cc_background_item_get_placement (item));

  if (flags & CC_BACKGROUND_ITEM_HAS_SHADING)
    g_settings_set_enum (settings, "color-shading-type", cc_background_item_get_shading (item));

  g_settings_set_string (settings, "primary-color", cc_background_item_get_pcolor (item));
  g_settings_set_string (settings, "secondary-color", cc_background_item_get_scolor (item));

  g_settings_apply (settings);
}

static void
add_picture_cb (UnitySettingsAppearancePage *self)
{
  cc_background_chooser_select_file (self->chooser);
}

static void
unity_settings_appearance_page_dispose (GObject *object)
{
  UnitySettingsAppearancePage *self = UNITY_SETTINGS_APPEARANCE_PAGE (object);

  gtk_widget_dispose_template (GTK_WIDGET (self), UNITY_SETTINGS_TYPE_APPEARANCE_PAGE);
  g_clear_object (&self->interface);
  g_clear_object (&self->background);
  g_clear_object (&self->current);

  G_OBJECT_CLASS (unity_settings_appearance_page_parent_class)->dispose (object);
}

static void
unity_settings_appearance_page_class_init (UnitySettingsAppearancePageClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_settings_appearance_page_dispose;

  g_type_ensure (CC_TYPE_BACKGROUND_CHOOSER);
  g_type_ensure (UNITY_SETTINGS_TYPE_STYLE_PREVIEW);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/Settings/unity-settings-appearance-page.ui");
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsAppearancePage, preview);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsAppearancePage, style_row);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsAppearancePage, translucency_row);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsAppearancePage, accent_label);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsAppearancePage, chooser);
  gtk_widget_class_bind_template_callback (widget_class, background_chosen_cb);
  gtk_widget_class_bind_template_callback (widget_class, add_picture_cb);
}

static void
unity_settings_appearance_page_init (UnitySettingsAppearancePage *self)
{
  g_autoptr (GSimpleActionGroup) group       = g_simple_action_group_new ();
  g_autoptr (GSettings)          stylesheets = g_settings_new (STYLESHEETS_SCHEMA_ID);
  g_autoptr (GAction)            accent      = NULL;

  gtk_widget_init_template (GTK_WIDGET (self));

  self->interface = g_settings_new (INTERFACE_SCHEMA_ID);
  g_signal_connect_object (self->interface, "changed::color-scheme", G_CALLBACK (reload_color_scheme), self,
                           G_CONNECT_SWAPPED);
  g_signal_connect_object (self->interface, "changed::accent-color", G_CALLBACK (reload_accent_color), self,
                           G_CONNECT_SWAPPED);
  reload_color_scheme (self);
  reload_accent_color (self);
  g_signal_connect_object (self->style_row, "notify::selected", G_CALLBACK (style_selected_cb), self,
                           G_CONNECT_SWAPPED);

  g_settings_bind (stylesheets, "translucency", self->translucency_row, "active", G_SETTINGS_BIND_DEFAULT);

  accent = g_settings_create_action (self->interface, "accent-color");
  g_action_map_add_action (G_ACTION_MAP (group), accent);
  gtk_widget_insert_action_group (GTK_WIDGET (self), "appearance", G_ACTION_GROUP (group));

  self->background = g_settings_new (BACKGROUND_SCHEMA_ID);
  g_signal_connect_object (self->background, "change-event", G_CALLBACK (background_changed_cb), self,
                           G_CONNECT_SWAPPED);
  reload_current (self);
}
