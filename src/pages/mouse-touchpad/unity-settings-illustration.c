/* unity-settings-illustration.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-settings-illustration.h"

struct _UnitySettingsIllustration
{
  GtkWidget       parent_instance;

  GtkPicture     *picture;
  GtkCheckButton *check_button;
  gchar          *resource;
};

G_DEFINE_FINAL_TYPE (UnitySettingsIllustration, unity_settings_illustration, GTK_TYPE_WIDGET)

typedef enum
{
  PROP_RESOURCE = 1,
  PROP_CHECK_BUTTON,
} UnitySettingsIllustrationProperty;

static GParamSpec *properties[PROP_CHECK_BUTTON + 1];

static GtkSvg *
get_svg (UnitySettingsIllustration *self)
{
  return GTK_SVG (gtk_picture_get_paintable (self->picture));
}

static void
enter_cb (UnitySettingsIllustration *self)
{
  if (get_svg (self) != NULL)
    gtk_svg_play (get_svg (self));
}

static void
leave_cb (UnitySettingsIllustration *self)
{
  if (get_svg (self) != NULL)
    gtk_svg_pause (get_svg (self));
}

static void
released_cb (UnitySettingsIllustration *self)
{
  if (self->check_button != NULL)
    gtk_widget_activate (GTK_WIDGET (self->check_button));
}

static void
unity_settings_illustration_map (GtkWidget *widget)
{
  UnitySettingsIllustration *self = UNITY_SETTINGS_ILLUSTRATION (widget);

  GTK_WIDGET_CLASS (unity_settings_illustration_parent_class)->map (widget);

  if (get_svg (self) != NULL)
    gtk_svg_set_frame_clock (get_svg (self), gtk_widget_get_frame_clock (widget));
}

static void
unity_settings_illustration_unmap (GtkWidget *widget)
{
  UnitySettingsIllustration *self = UNITY_SETTINGS_ILLUSTRATION (widget);

  if (get_svg (self) != NULL)
    {
      gtk_svg_pause (get_svg (self));
      gtk_svg_set_frame_clock (get_svg (self), NULL);
    }

  GTK_WIDGET_CLASS (unity_settings_illustration_parent_class)->unmap (widget);
}

static void
unity_settings_illustration_get_property (GObject    *object,
                                          guint       prop_id,
                                          GValue     *value,
                                          GParamSpec *pspec)
{
  UnitySettingsIllustration *self = UNITY_SETTINGS_ILLUSTRATION (object);

  switch ((UnitySettingsIllustrationProperty) prop_id)
    {
    case PROP_RESOURCE:
      g_value_set_string (value, self->resource);
      break;

    case PROP_CHECK_BUTTON:
      g_value_set_object (value, self->check_button);
      break;
    }
}

static void
unity_settings_illustration_set_property (GObject      *object,
                                          guint         prop_id,
                                          const GValue *value,
                                          GParamSpec   *pspec)
{
  UnitySettingsIllustration *self = UNITY_SETTINGS_ILLUSTRATION (object);

  switch ((UnitySettingsIllustrationProperty) prop_id)
    {
    case PROP_RESOURCE:
      unity_settings_illustration_set_resource (self, g_value_get_string (value));
      break;

    case PROP_CHECK_BUTTON:
      unity_settings_illustration_set_check_button (self, g_value_get_object (value));
      break;
    }
}

static void
unity_settings_illustration_dispose (GObject *object)
{
  UnitySettingsIllustration *self = UNITY_SETTINGS_ILLUSTRATION (object);

  gtk_widget_dispose_template (GTK_WIDGET (self), UNITY_SETTINGS_TYPE_ILLUSTRATION);
  g_clear_object (&self->check_button);
  g_clear_pointer (&self->resource, g_free);

  G_OBJECT_CLASS (unity_settings_illustration_parent_class)->dispose (object);
}

static void
unity_settings_illustration_class_init (UnitySettingsIllustrationClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->get_property = unity_settings_illustration_get_property;
  object_class->set_property = unity_settings_illustration_set_property;
  object_class->dispose = unity_settings_illustration_dispose;
  widget_class->map = unity_settings_illustration_map;
  widget_class->unmap = unity_settings_illustration_unmap;

  /**
   * UnitySettingsIllustration:resource:
   *
   * The resource path of the svg. It plays while the pointer is over the illustration.
   */
  properties[PROP_RESOURCE] =
    g_param_spec_string ("resource", NULL, NULL, NULL,
                         G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);

  /**
   * UnitySettingsIllustration:check-button:
   *
   * The radio that a click on the illustration selects.
   */
  properties[PROP_CHECK_BUTTON] =
    g_param_spec_object ("check-button", NULL, NULL, GTK_TYPE_CHECK_BUTTON,
                         G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, G_N_ELEMENTS (properties), properties);

  gtk_widget_class_set_css_name (widget_class, "illustration");
  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/Settings/unity-settings-illustration.ui");
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsIllustration, picture);
  gtk_widget_class_bind_template_callback (widget_class, enter_cb);
  gtk_widget_class_bind_template_callback (widget_class, leave_cb);
  gtk_widget_class_bind_template_callback (widget_class, released_cb);
}

static void
unity_settings_illustration_init (UnitySettingsIllustration *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

void
unity_settings_illustration_set_resource (UnitySettingsIllustration *self,
                                          const gchar               *resource)
{
  g_autoptr (GtkSvg) svg = NULL;

  g_return_if_fail (UNITY_SETTINGS_IS_ILLUSTRATION (self));

  if (!g_set_str (&self->resource, resource))
    return;

  if (resource != NULL)
    svg = gtk_svg_new_from_resource (resource);

  gtk_picture_set_paintable (self->picture, GDK_PAINTABLE (svg));

  if (svg != NULL && gtk_widget_get_mapped (GTK_WIDGET (self)))
    gtk_svg_set_frame_clock (svg, gtk_widget_get_frame_clock (GTK_WIDGET (self)));

  g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_RESOURCE]);
}

void
unity_settings_illustration_set_check_button (UnitySettingsIllustration *self,
                                              GtkCheckButton            *check_button)
{
  g_return_if_fail (UNITY_SETTINGS_IS_ILLUSTRATION (self));

  if (g_set_object (&self->check_button, check_button))
    g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_CHECK_BUTTON]);
}
