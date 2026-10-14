/* unity-settings-sidebar-item.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-settings-sidebar-item.h"

struct _UnitySettingsSidebarItem
{
  AdwSidebarItem parent_instance;

  GType          page_type;
};

G_DEFINE_FINAL_TYPE (UnitySettingsSidebarItem, unity_settings_sidebar_item, ADW_TYPE_SIDEBAR_ITEM)

typedef enum
{
  PROP_PAGE_TYPE = 1,
} UnitySettingsSidebarItemProperty;

static GParamSpec *properties[PROP_PAGE_TYPE + 1];

static void
unity_settings_sidebar_item_get_property (GObject    *object,
                                          guint       prop_id,
                                          GValue     *value,
                                          GParamSpec *pspec)
{
  UnitySettingsSidebarItem *self = UNITY_SETTINGS_SIDEBAR_ITEM (object);

  switch ((UnitySettingsSidebarItemProperty) prop_id)
    {
    case PROP_PAGE_TYPE:
      g_value_set_gtype (value, self->page_type);
      break;
    }
}

static void
unity_settings_sidebar_item_set_property (GObject      *object,
                                          guint         prop_id,
                                          const GValue *value,
                                          GParamSpec   *pspec)
{
  UnitySettingsSidebarItem *self = UNITY_SETTINGS_SIDEBAR_ITEM (object);

  switch ((UnitySettingsSidebarItemProperty) prop_id)
    {
    case PROP_PAGE_TYPE:
      self->page_type = g_value_get_gtype (value);
      break;
    }
}

static void
unity_settings_sidebar_item_class_init (UnitySettingsSidebarItemClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = unity_settings_sidebar_item_get_property;
  object_class->set_property = unity_settings_sidebar_item_set_property;

  /**
   * UnitySettingsSidebarItem:page-type:
   *
   * The widget type that the window creates when the item is selected.
   */
  properties[PROP_PAGE_TYPE] =
    g_param_spec_gtype ("page-type", NULL, NULL, GTK_TYPE_WIDGET,
                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, G_N_ELEMENTS (properties), properties);
}

static void
unity_settings_sidebar_item_init (UnitySettingsSidebarItem *self)
{
}

GType
unity_settings_sidebar_item_get_page_type (UnitySettingsSidebarItem *self)
{
  g_return_val_if_fail (UNITY_SETTINGS_IS_SIDEBAR_ITEM (self), G_TYPE_NONE);

  return self->page_type;
}
