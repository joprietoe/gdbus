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
 *   Declarations for Client
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <dbus/dbus-glib-bindings.h>

/* Standard GObject class structures, etc */

#define TYPE_CLIENT                  (client_get_type())
#define CLIENT(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_CLIENT, Client))
#define IS_CLIENT(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_CLIENT))
#define CLIENT_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_CLIENT, ClientClass))
#define IS_CLIENT_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_CLIENT))
#define CLIENT_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_CLIENT, ClientClass))

typedef struct
{
  GObjectClass parent_class;

	DBusGConnection *connection;
  DBusGProxy *proxy;
} ClientClass;

typedef struct
{
  GObject parnet_instance;

} Client;


GType client_get_type(void);

#endif /* #ifndef CLIENT_H */
