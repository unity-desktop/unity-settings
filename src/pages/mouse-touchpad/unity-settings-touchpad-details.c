/* unity-settings-touchpad-details.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-settings-touchpad-details.h"

#include <gio/gunixsocketaddress.h>
#include <json-glib/json-glib.h>

/**
 * unity_settings_touchpad_details_get_defaults:
 * @click_method: (out): the click method that libinput picks for "default"
 * @scroll_method: (out): the scroll method that libinput picks for "default"
 *
 * Asks the touchpad-details Wayfire plugin about the first touchpad.
 *
 * Returns: %TRUE if the plugin answered for a touchpad
 */
gboolean
unity_settings_touchpad_details_get_defaults (gchar **click_method,
                                              gchar **scroll_method)
{
  static const gchar            request[]  = "{\"method\":\"touchpad-details/list\",\"data\":{}}";
  const gchar                  *path       = g_getenv ("WAYFIRE_SOCKET");
  guint32                       length     = sizeof request - 1;
  gsize                         read       = 0;
  g_autoptr (GSocketClient)     client     = NULL;
  g_autoptr (GSocketAddress)    address    = NULL;
  g_autoptr (GSocketConnection) connection = NULL;
  g_autoptr (JsonParser)        parser     = NULL;
  g_autofree gchar             *reply      = NULL;
  GOutputStream                *output;
  GInputStream                 *input;
  JsonNode                     *touchpads;
  JsonObject                   *touchpad;

  if (path == NULL)
    return FALSE;

  client = g_socket_client_new ();
  g_socket_client_set_timeout (client, 1);
  address = g_unix_socket_address_new (path);
  connection = g_socket_client_connect (client, G_SOCKET_CONNECTABLE (address), NULL, NULL);
  if (connection == NULL)
    return FALSE;

  output = g_io_stream_get_output_stream (G_IO_STREAM (connection));
  input = g_io_stream_get_input_stream (G_IO_STREAM (connection));

  /* Wayfire frames each message with its length in host byte order. */
  if (!g_output_stream_write_all (output, &length, sizeof length, NULL, NULL, NULL) ||
      !g_output_stream_write_all (output, request, length, NULL, NULL, NULL) ||
      !g_input_stream_read_all (input, &length, sizeof length, &read, NULL, NULL) ||
      read != sizeof length || length > G_MAXUINT16)
    return FALSE;

  reply = g_malloc (length);
  if (!g_input_stream_read_all (input, reply, length, &read, NULL, NULL) || read != length)
    return FALSE;

  parser = json_parser_new ();
  if (!json_parser_load_from_data (parser, reply, length, NULL) ||
      !JSON_NODE_HOLDS_OBJECT (json_parser_get_root (parser)))
    return FALSE;

  touchpads = json_object_get_member (json_node_get_object (json_parser_get_root (parser)), "touchpads");
  if (touchpads == NULL || !JSON_NODE_HOLDS_ARRAY (touchpads) ||
      json_array_get_length (json_node_get_array (touchpads)) == 0)
    return FALSE;

  touchpad = json_array_get_object_element (json_node_get_array (touchpads), 0);
  *click_method = g_strdup (json_object_get_string_member_with_default (touchpad, "click-method", "default"));
  *scroll_method = g_strdup (json_object_get_string_member_with_default (touchpad, "scroll-method", "default"));

  return TRUE;
}
