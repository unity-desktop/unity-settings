/* unity-settings-style-preview.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-settings-style-preview.h"

#include "cc-background-paintable.h"

#define THUMBNAIL_WIDTH  300
#define THUMBNAIL_HEIGHT (THUMBNAIL_WIDTH * 3 / 4)

struct _UnitySettingsStylePreview
{
  AdwBin                        parent_instance;

  GnomeDesktopThumbnailFactory *thumbnails;
  CcBackgroundItem             *item;
  GDesktopColorScheme           color_scheme;
  GtkPicture                   *picture;
  GtkSvg                       *windows;
};

G_DEFINE_FINAL_TYPE (UnitySettingsStylePreview, unity_settings_style_preview, ADW_TYPE_BIN)

static void
update (UnitySettingsStylePreview *self)
{
  gboolean                          dark      = self->color_scheme == G_DESKTOP_COLOR_SCHEME_PREFER_DARK;
  g_autoptr (CcBackgroundPaintable) paintable = NULL;

  gtk_svg_set_state (self->windows, self->color_scheme);

  if (self->item == NULL)
    return;

  paintable = cc_background_paintable_new (self->thumbnails, self->item,
                                           dark ? CC_BACKGROUND_PAINT_DARK : CC_BACKGROUND_PAINT_LIGHT,
                                           THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, GTK_WIDGET (self));
  gtk_picture_set_paintable (self->picture, GDK_PAINTABLE (paintable));
}

static void
unity_settings_style_preview_dispose (GObject *object)
{
  UnitySettingsStylePreview *self = UNITY_SETTINGS_STYLE_PREVIEW (object);

  gtk_widget_dispose_template (GTK_WIDGET (self), UNITY_SETTINGS_TYPE_STYLE_PREVIEW);
  g_clear_object (&self->item);
  g_clear_object (&self->thumbnails);

  G_OBJECT_CLASS (unity_settings_style_preview_parent_class)->dispose (object);
}

static void
unity_settings_style_preview_class_init (UnitySettingsStylePreviewClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_settings_style_preview_dispose;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/Settings/unity-settings-style-preview.ui");
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsStylePreview, picture);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsStylePreview, windows);

  gtk_widget_class_set_accessible_role (widget_class, GTK_ACCESSIBLE_ROLE_PRESENTATION);
}

static void
unity_settings_style_preview_init (UnitySettingsStylePreview *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  self->thumbnails = gnome_desktop_thumbnail_factory_new (GNOME_DESKTOP_THUMBNAIL_SIZE_LARGE);
}

void
unity_settings_style_preview_set_item (UnitySettingsStylePreview *self,
                                       CcBackgroundItem          *item)
{
  g_return_if_fail (UNITY_SETTINGS_IS_STYLE_PREVIEW (self));
  g_return_if_fail (CC_IS_BACKGROUND_ITEM (item));

  if (g_set_object (&self->item, item))
    update (self);
}

void
unity_settings_style_preview_set_color_scheme (UnitySettingsStylePreview *self,
                                               GDesktopColorScheme        color_scheme)
{
  g_return_if_fail (UNITY_SETTINGS_IS_STYLE_PREVIEW (self));

  if (self->color_scheme == color_scheme)
    return;

  self->color_scheme = color_scheme;
  update (self);
}
