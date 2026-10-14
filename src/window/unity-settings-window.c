/* unity-settings-window.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-settings-window.h"

#include "unity-settings-appearance-page.h"
#include "unity-settings-sidebar-item.h"

struct _UnitySettingsWindow
{
  AdwApplicationWindow    parent_instance;

  AdwNavigationSplitView *split_view;
  AdwSidebar             *sidebar;
  AdwSidebarItem         *active_item;
};

G_DEFINE_FINAL_TYPE (UnitySettingsWindow, unity_settings_window, ADW_TYPE_APPLICATION_WINDOW)

static void
update_content (UnitySettingsWindow *self)
{
  AdwSidebarItem *item = adw_sidebar_get_selected_item (self->sidebar);
  GtkWidget      *page;

  if (item == NULL || item == self->active_item)
    return;

  self->active_item = item;

  page = g_object_new (unity_settings_sidebar_item_get_page_type (UNITY_SETTINGS_SIDEBAR_ITEM (item)), NULL);
  adw_navigation_split_view_set_content (self->split_view,
                                         adw_navigation_page_new (page, adw_sidebar_item_get_title (item)));
}

static void
sidebar_activated_cb (UnitySettingsWindow *self,
                      guint                index)
{
  update_content (self);
  adw_navigation_split_view_set_show_content (self->split_view, TRUE);
}

static void
unity_settings_window_class_init (UnitySettingsWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_add_binding_action (widget_class, GDK_KEY_w, GDK_CONTROL_MASK, "window.close", NULL);

  g_type_ensure (UNITY_SETTINGS_TYPE_SIDEBAR_ITEM);
  g_type_ensure (UNITY_SETTINGS_TYPE_APPEARANCE_PAGE);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/Settings/unity-settings-window.ui");
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsWindow, split_view);
  gtk_widget_class_bind_template_child (widget_class, UnitySettingsWindow, sidebar);
  gtk_widget_class_bind_template_callback (widget_class, sidebar_activated_cb);
}

static void
unity_settings_window_init (UnitySettingsWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  update_content (self);
}

GtkWindow *
unity_settings_window_new (GtkApplication *application)
{
  g_return_val_if_fail (GTK_IS_APPLICATION (application), NULL);

  return g_object_new (UNITY_SETTINGS_TYPE_WINDOW, "application", application, NULL);
}
