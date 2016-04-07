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
 *   Implementations for Server
 */

#include "server.h"
#include "server-bindings.h"

static void server_class_init(ServerClass *klass);

GType server_get_type(void)
{
  static GType server_type = 0;

  if(!server_type)
  {
    static const GTypeInfo server_info = {
      sizeof(ServerClass),                 /* class structure size */
      NULL,                                /* base class initializer */
      NULL,                                /* base class finalizer */
      (GClassInitFunc)server_class_init,   /* class initializer */
      NULL,                                /* class finalizer */
      NULL,                                /* class data */
      sizeof(Server),                      /* instance structure size */
      1,                                   /* preallocated instances */
      NULL,                                /* instance initializers */
      NULL
    };

    server_type = g_type_register_static(
      G_TYPE_OBJECT,                       /* parent class */
      "Server",
      &server_info,
      0);
  }

  return server_type;
}

static void server_class_init(ServerClass *klass)
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

	/* &dbus_glib__object_info is provided in the server-bindings.h file */
	/* OBJECT_TYPE_SERVER is the GType of your server object */
	dbus_g_object_type_install_info (TYPE_SERVER, &dbus_glib_server_object_object_info);

  g_message("ServerClass successfully initialized");
}


static gint server_init(Server *server)
{
	GError *error = NULL;
	DBusGProxy *driver_proxy;
	ServerClass *klass = SERVER_GET_CLASS (server);
	guint request_ret;
  gint retcode = 1;

	/* Register DBUS path */
	dbus_g_connection_register_g_object (klass->connection,
			"/org/gnome/ServiceName",
			G_OBJECT (server));

	/* Register the service name, the constant here are defined in dbus-glib-bindings.h */
	driver_proxy = dbus_g_proxy_new_for_name (klass->connection,
			DBUS_SERVICE_DBUS,
			DBUS_PATH_DBUS,
			DBUS_INTERFACE_DBUS);

	if(!org_freedesktop_DBus_request_name (driver_proxy,
			"org.gnome.ServiceName",
			0, &request_ret,    /* See tutorial for more infos about these */
			&error))
	{
		g_warning("Unable to register service: %s", error->message);
		g_error_free (error);
    retcode = 0;
	}
	g_object_unref (driver_proxy);

  return( retcode );
}


gboolean server_echo_string (Server *server, gchar *original, gchar **echo, GError **error)
{
	*echo = g_strdup (original);
  gboolean problem = FALSE;

	if (problem)
	{
		/* We have an error, set the gerror */
		g_set_error (error, g_quark_from_static_string ("echo"),
					0xdeadbeef,
					"Some random problem occured, you're screwed");
		return FALSE;
	}

	return TRUE;
}

int main()
{
  Server *server;
	GMainLoop *loop;

  g_type_init(); /* initialize type system */

	if (!g_thread_supported ())
		g_thread_init (NULL);

	dbus_g_thread_init ();

	loop = g_main_loop_new (NULL, FALSE);

  server = g_object_new( TYPE_SERVER, NULL );

  if( server_init( server ) )
  {
    g_main_loop_run (loop);
  }

  return 0;
}
