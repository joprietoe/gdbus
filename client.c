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
 *   Implementations for Client
 */

#include "client.h"
#include "client-bindings.h"

static void client_class_init(ClientClass *klass);
static void client_class_release(ClientClass *klass);

GType client_get_type(void)
{
  static GType client_type = 0;

  if(!client_type)
  {
    static const GTypeInfo client_info = {
      sizeof(ClientClass),                 /* class structure size */
      NULL,                                /* base class initializer */
      NULL,                                /* base class finalizer */
      (GClassInitFunc)client_class_init,   /* class initializer */
      NULL,                                /* class finalizer */
      NULL,                                /* class data */
      sizeof(Client),                      /* instance structure size */
      1,                                   /* preallocated instances */
      NULL,                                /* instance initializers */
      NULL
    };

    client_type = g_type_register_static(
      G_TYPE_OBJECT,                       /* parent class */
      "Client",
      &client_info,
      0);
  }

  return client_type;
}

static void
client_echo_reply (DBusGProxy *proxy, char *answer, GError *error, gpointer userdata);

static void client_class_init(ClientClass *klass)
{
	GError *error = NULL;

	/* Init the DBus connection, per-klass */
	klass->connection = dbus_g_bus_get (DBUS_BUS_SESSION, &error);
	if (klass->connection == NULL)
	{
		g_warning("Unable to connect to dbus: %s", error->message);
		g_error_free (error);
		return;
	}

/* This won't trigger activation! */
  klass->proxy = dbus_g_proxy_new_for_name (klass->connection,
                                            "org.gnome.ServiceName",
                                            "/org/gnome/ServiceName",
                                            "org.gnome.ServiceName");

  g_message("ClientClass successfully initialized");
}

static void client_send_async(Client *client)
{
  ClientClass *klass = CLIENT_GET_CLASS( client );

/* The method call will trigger activation and prints results asynchronously */
  org_gnome_ServiceName_echo_string_async(klass->proxy,
                                          "The async request string we want echo-ed",
                                          client_echo_reply, client);
}

static void client_send_sync(Client *client)
{
	GError *error = NULL;
  ClientClass *klass = CLIENT_GET_CLASS( client );
  gchar *result;

/* The method call will trigger activation and blocks until result arives */
  if (!org_gnome_ServiceName_echo_string (klass->proxy,
                                          "The sync request string we want echo-ed",
                                          &result, &error))
  {
    /* Method failed, the GError is set, let's warn everyone */
    g_warning ("Woops remote method failed: %s", error->message);
    g_error_free (error);
    return;
  }

  g_print ("We got the folowing result from synchronous request: %s\n", result);

/* Cleanup */
  g_free (result);
}

static void
client_echo_reply (DBusGProxy *proxy, char *answer, GError *error, gpointer userdata)
{
	Client *client = CLIENT(userdata);

	if (error!= NULL)
	{
		g_warning ("An error occured while calling echo_string remote method: %s", error->message);
		g_error_free (error);
		return;
	}

	g_print ("We got an echo reply, result: %s\n", answer);
}


int main()
{
  Client *client;
	GMainLoop *loop;

  g_type_init(); /* initialize type system */

	if (!g_thread_supported ())
		g_thread_init (NULL);

	dbus_g_thread_init ();

	loop = g_main_loop_new (NULL, FALSE);
  client = g_object_new( TYPE_CLIENT, NULL );

  client_send_async( client );
  client_send_sync( client );

  g_main_loop_run (loop);
  return 0;
}
