/* main.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <adwaita.h>

#include "unity-settings-window.h"

static void
quit_cb (GSimpleAction *action,
         GVariant      *parameter,
         gpointer       user_data)
{
  g_application_quit (user_data);
}

static const GActionEntry actions[] = {
  { .name = "quit", .activate = quit_cb },
};

static void
startup_cb (GApplication *app)
{
  g_action_map_add_action_entries (G_ACTION_MAP (app), actions, G_N_ELEMENTS (actions), app);
  gtk_application_set_accels_for_action (GTK_APPLICATION (app), "app.quit", (const gchar *[]) { "<Control>q", NULL });
}

static void
activate_cb (GApplication *app)
{
  GtkWindow *window = gtk_application_get_active_window (GTK_APPLICATION (app));

  if (window == NULL)
    window = unity_settings_window_new (GTK_APPLICATION (app));

  gtk_window_present (window);
}

gint
main (gint   argc,
      gchar *argv[])
{
  g_autoptr (AdwApplication) app = NULL;

  app = adw_application_new ("org.unity.Settings", G_APPLICATION_CAN_OVERRIDE_APP_ID);
  g_signal_connect (app, "startup", G_CALLBACK (startup_cb), NULL);
  g_signal_connect (app, "activate", G_CALLBACK (activate_cb), NULL);

  return g_application_run (G_APPLICATION (app), argc, argv);
}
