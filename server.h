/*
 *   Source Code from the DBus Activation Tutorial
 *   from Raphael Slinckx
 *
 *   This code illustrates how to requrest the dbus daemon
 *   to automatically start a program that provides a given
 *   service. For more detailed information refer also to
 *   http://raphael.slinckx.net/blog/documents/dbus-tutorial
 *   where all source has taken from.
 *
 *   Provision of all glue code to form compilable application
 *   by Otto Linnemann
 *
 *   Declarations for Server
 */

#ifndef SERVER_H
#define SERVER_H

#include <dbus/dbus-glib-bindings.h>

/* Standard GObject class structures, etc */

#define TYPE_SERVER                  (server_get_type())
#define SERVER(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_SERVER, Server))
#define IS_SERVER(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_SERVER))
#define SERVER_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_SERVER, ServerClass))
#define IS_SERVER_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_SERVER))
#define SERVER_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_SERVER, ServerClass))

typedef struct
{
  GObjectClass parent_class;

	DBusGConnection *connection;
} ServerClass;

typedef struct
{
  GObject parnet_instance;

} Server;


GType server_get_type(void);
gboolean server_echo_string(Server *server, gchar *original, gchar **echo, GError **error);

#endif /* #ifndef SERVER_H */
