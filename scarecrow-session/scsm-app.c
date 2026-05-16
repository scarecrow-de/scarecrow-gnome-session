/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2007 Novell, Inc.
 * Copyright (C) 2008 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glib.h>
#include <string.h>

#include "scsm-app.h"
#include "io.github.scarecrow_de.SessionManager.App.h"

#define SCSM_APP_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), SCSM_TYPE_APP, ScsmAppPrivate))

/* If a component crashes twice within a minute, we count that as a fatal error */
#define _SCSM_APP_RESPAWN_RATELIMIT_SECONDS 60

struct _ScsmAppPrivate
{
        char            *id;
        char            *app_id;
        int              phase;
        char            *startup_id;
        gboolean         registered;
        GTimeVal         last_restart_time;
        GDBusConnection *connection;
        ScsmExportedApp  *skeleton;
};


enum {
        EXITED,
        DIED,
        LAST_SIGNAL
};

static guint32 app_serial = 1;

static guint signals[LAST_SIGNAL] = { 0 };

enum {
        PROP_0,
        PROP_ID,
        PROP_STARTUP_ID,
        PROP_PHASE,
        PROP_REGISTERED,
        LAST_PROP
};

G_DEFINE_TYPE (ScsmApp, scsm_app, G_TYPE_OBJECT)

GQuark
scsm_app_error_quark (void)
{
        static GQuark ret = 0;
        if (ret == 0) {
                ret = g_quark_from_static_string ("scsm_app_error");
        }

        return ret;

}

static gboolean
scsm_app_get_app_id (ScsmExportedApp        *skeleton,
                    GDBusMethodInvocation *invocation,
                    ScsmApp                *app)
{
        const gchar *id;

        id = SCSM_APP_GET_CLASS (app)->impl_get_app_id (app);
        scsm_exported_app_complete_get_app_id (skeleton, invocation, id);

        return TRUE;
}

static gboolean
scsm_app_get_startup_id (ScsmExportedApp        *skeleton,
                        GDBusMethodInvocation *invocation,
                        ScsmApp                *app)
{
        const gchar *id;

        id = g_strdup (app->priv->startup_id);
        scsm_exported_app_complete_get_startup_id (skeleton, invocation, id);

        return TRUE;
}

static gboolean
scsm_app_get_phase (ScsmExportedApp        *skeleton,
                   GDBusMethodInvocation *invocation,
                   ScsmApp                *app)
{
        scsm_exported_app_complete_get_phase (skeleton, invocation, app->priv->phase);
        return TRUE;
}

static guint32
get_next_app_serial (void)
{
        guint32 serial;

        serial = app_serial++;

        if ((gint32)app_serial < 0) {
                app_serial = 1;
        }

        return serial;
}

static gboolean
register_app (ScsmApp *app)
{
        GError *error;
        ScsmExportedApp *skeleton;

        error = NULL;
        app->priv->connection = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, &error);
        if (error != NULL) {
                g_critical ("error getting session bus: %s", error->message);
                g_error_free (error);
                return FALSE;
        }

        skeleton = scsm_exported_app_skeleton_new ();
        app->priv->skeleton = skeleton;
        g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON (skeleton),
                                          app->priv->connection, app->priv->id,
                                          &error);

        if (error != NULL) {
                g_critical ("error registering app on session bus: %s", error->message);
                g_error_free (error);
                return FALSE;
        }

        g_signal_connect (skeleton, "handle-get-app-id",
                          G_CALLBACK (scsm_app_get_app_id), app);
        g_signal_connect (skeleton, "handle-get-phase",
                          G_CALLBACK (scsm_app_get_phase), app);
        g_signal_connect (skeleton, "handle-get-startup-id",
                          G_CALLBACK (scsm_app_get_startup_id), app);

        return TRUE;
}

static GObject *
scsm_app_constructor (GType                  type,
                     guint                  n_construct_properties,
                     GObjectConstructParam *construct_properties)
{
        ScsmApp    *app;
        gboolean   res;

        app = SCSM_APP (G_OBJECT_CLASS (scsm_app_parent_class)->constructor (type,
                                                                           n_construct_properties,
                                                                           construct_properties));

        g_free (app->priv->id);
        app->priv->id = g_strdup_printf ("/io/github/scarecrow_de/SessionManager/App%u", get_next_app_serial ());

        res = register_app (app);
        if (! res) {
                g_warning ("Unable to register app with session bus");
        }

        return G_OBJECT (app);
}

static void
scsm_app_init (ScsmApp *app)
{
        app->priv = SCSM_APP_GET_PRIVATE (app);
}

static void
scsm_app_set_phase (ScsmApp *app,
                   int     phase)
{
        g_return_if_fail (SCSM_IS_APP (app));

        app->priv->phase = phase;
}

static void
scsm_app_set_id (ScsmApp     *app,
                const char *id)
{
        g_return_if_fail (SCSM_IS_APP (app));

        g_free (app->priv->id);

        app->priv->id = g_strdup (id);
        g_object_notify (G_OBJECT (app), "id");

}
static void
scsm_app_set_startup_id (ScsmApp     *app,
                        const char *startup_id)
{
        g_return_if_fail (SCSM_IS_APP (app));

        g_free (app->priv->startup_id);

        app->priv->startup_id = g_strdup (startup_id);
        g_object_notify (G_OBJECT (app), "startup-id");

}

static void
scsm_app_set_property (GObject      *object,
                      guint         prop_id,
                      const GValue *value,
                      GParamSpec   *pspec)
{
        ScsmApp *app = SCSM_APP (object);

        switch (prop_id) {
        case PROP_STARTUP_ID:
                scsm_app_set_startup_id (app, g_value_get_string (value));
                break;
        case PROP_ID:
                scsm_app_set_id (app, g_value_get_string (value));
                break;
        case PROP_PHASE:
                scsm_app_set_phase (app, g_value_get_int (value));
                break;
        case PROP_REGISTERED:
                scsm_app_set_registered (app, g_value_get_boolean (value));
                break;
        default:
                break;
        }
}

static void
scsm_app_get_property (GObject    *object,
                      guint       prop_id,
                      GValue     *value,
                      GParamSpec *pspec)
{
        ScsmApp *app = SCSM_APP (object);

        switch (prop_id) {
        case PROP_STARTUP_ID:
                g_value_set_string (value, app->priv->startup_id);
                break;
        case PROP_ID:
                g_value_set_string (value, app->priv->id);
                break;
        case PROP_PHASE:
                g_value_set_int (value, app->priv->phase);
                break;
        case PROP_REGISTERED:
                g_value_set_boolean (value, app->priv->registered);
                break;
        default:
                break;
        }
}

static void
scsm_app_dispose (GObject *object)
{
        ScsmApp *app = SCSM_APP (object);

        g_free (app->priv->startup_id);
        app->priv->startup_id = NULL;

        g_free (app->priv->id);
        app->priv->id = NULL;

        if (app->priv->skeleton != NULL) {
                g_dbus_interface_skeleton_unexport_from_connection (G_DBUS_INTERFACE_SKELETON (app->priv->skeleton),
                                                                    app->priv->connection);
                g_clear_object (&app->priv->skeleton);
        }

        g_clear_object (&app->priv->connection);

        G_OBJECT_CLASS (scsm_app_parent_class)->dispose (object);
}

static void
scsm_app_class_init (ScsmAppClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);

        object_class->set_property = scsm_app_set_property;
        object_class->get_property = scsm_app_get_property;
        object_class->dispose = scsm_app_dispose;
        object_class->constructor = scsm_app_constructor;

        klass->impl_start = NULL;
        klass->impl_get_app_id = NULL;
        klass->impl_get_autorestart = NULL;
        klass->impl_provides = NULL;
        klass->impl_get_provides = NULL;
        klass->impl_is_running = NULL;

        g_object_class_install_property (object_class,
                                         PROP_PHASE,
                                         g_param_spec_int ("phase",
                                                           "Phase",
                                                           "Phase",
                                                           -1,
                                                           G_MAXINT,
                                                           -1,
                                                           G_PARAM_READWRITE));
        g_object_class_install_property (object_class,
                                         PROP_ID,
                                         g_param_spec_string ("id",
                                                              "ID",
                                                              "ID",
                                                              NULL,
                                                              G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
        g_object_class_install_property (object_class,
                                         PROP_STARTUP_ID,
                                         g_param_spec_string ("startup-id",
                                                              "startup ID",
                                                              "Session management startup ID",
                                                              NULL,
                                                              G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

        g_object_class_install_property (object_class,
                                         PROP_REGISTERED,
                                         g_param_spec_boolean ("registered",
                                                               "Registered",
                                                               "Registered",
                                                               FALSE,
                                                               G_PARAM_READWRITE));

        signals[EXITED] =
                g_signal_new ("exited",
                              G_OBJECT_CLASS_TYPE (object_class),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (ScsmAppClass, exited),
                              NULL, NULL, NULL,
                              G_TYPE_NONE,
                              1, G_TYPE_UCHAR);
        signals[DIED] =
                g_signal_new ("died",
                              G_OBJECT_CLASS_TYPE (object_class),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (ScsmAppClass, died),
                              NULL, NULL, NULL,
                              G_TYPE_NONE,
                              1, G_TYPE_INT);

        g_type_class_add_private (klass, sizeof (ScsmAppPrivate));
}

const char *
scsm_app_peek_id (ScsmApp *app)
{
        return app->priv->id;
}

const char *
scsm_app_peek_app_id (ScsmApp *app)
{
        return SCSM_APP_GET_CLASS (app)->impl_get_app_id (app);
}

const char *
scsm_app_peek_startup_id (ScsmApp *app)
{
        return app->priv->startup_id;
}

/**
 * scsm_app_peek_phase:
 * @app: a %ScsmApp
 *
 * Returns @app's startup phase.
 *
 * Return value: @app's startup phase
 **/
ScsmManagerPhase
scsm_app_peek_phase (ScsmApp *app)
{
        g_return_val_if_fail (SCSM_IS_APP (app), SCSM_MANAGER_PHASE_APPLICATION);

        return app->priv->phase;
}

gboolean
scsm_app_peek_is_disabled (ScsmApp *app)
{
        g_return_val_if_fail (SCSM_IS_APP (app), FALSE);

        if (SCSM_APP_GET_CLASS (app)->impl_is_disabled) {
                return SCSM_APP_GET_CLASS (app)->impl_is_disabled (app);
        } else {
                return FALSE;
        }
}

gboolean
scsm_app_peek_is_conditionally_disabled (ScsmApp *app)
{
        g_return_val_if_fail (SCSM_IS_APP (app), FALSE);

        if (SCSM_APP_GET_CLASS (app)->impl_is_conditionally_disabled) {
                return SCSM_APP_GET_CLASS (app)->impl_is_conditionally_disabled (app);
        } else {
                return FALSE;
        }
}

gboolean
scsm_app_is_running (ScsmApp *app)
{
        g_return_val_if_fail (SCSM_IS_APP (app), FALSE);

        if (SCSM_APP_GET_CLASS (app)->impl_is_running) {
                return SCSM_APP_GET_CLASS (app)->impl_is_running (app);
        } else {
                return FALSE;
        }
}

gboolean
scsm_app_peek_autorestart (ScsmApp *app)
{
        g_return_val_if_fail (SCSM_IS_APP (app), FALSE);

        if (SCSM_APP_GET_CLASS (app)->impl_get_autorestart) {
                return SCSM_APP_GET_CLASS (app)->impl_get_autorestart (app);
        } else {
                return FALSE;
        }
}

gboolean
scsm_app_provides (ScsmApp *app, const char *service)
{
        if (SCSM_APP_GET_CLASS (app)->impl_provides) {
                return SCSM_APP_GET_CLASS (app)->impl_provides (app, service);
        } else {
                return FALSE;
        }
}

char **
scsm_app_get_provides (ScsmApp *app)
{
        if (SCSM_APP_GET_CLASS (app)->impl_get_provides) {
                return SCSM_APP_GET_CLASS (app)->impl_get_provides (app);
        } else {
                return NULL;
        }
}

gboolean
scsm_app_has_autostart_condition (ScsmApp     *app,
                                 const char *condition)
{

        if (SCSM_APP_GET_CLASS (app)->impl_has_autostart_condition) {
                return SCSM_APP_GET_CLASS (app)->impl_has_autostart_condition (app, condition);
        } else {
                return FALSE;
        }
}

gboolean
scsm_app_start (ScsmApp  *app,
               GError **error)
{
        g_debug ("Starting app: %s", app->priv->id);
        return SCSM_APP_GET_CLASS (app)->impl_start (app, error);
}

gboolean
scsm_app_restart (ScsmApp  *app,
                 GError **error)
{
        GTimeVal current_time;
        g_debug ("Re-starting app: %s", app->priv->id);

        g_get_current_time (&current_time);
        if (app->priv->last_restart_time.tv_sec > 0
            && (current_time.tv_sec - app->priv->last_restart_time.tv_sec) < _SCSM_APP_RESPAWN_RATELIMIT_SECONDS) {
                g_warning ("App '%s' respawning too quickly", scsm_app_peek_app_id (app));
                g_set_error (error,
                             SCSM_APP_ERROR,
                             SCSM_APP_ERROR_RESTART_LIMIT,
                             "Component '%s' crashing too quickly",
                             scsm_app_peek_app_id (app));
                return FALSE;
        }
        app->priv->last_restart_time = current_time;

        return SCSM_APP_GET_CLASS (app)->impl_restart (app, error);
}

gboolean
scsm_app_stop (ScsmApp  *app,
              GError **error)
{
        return SCSM_APP_GET_CLASS (app)->impl_stop (app, error);
}

void
scsm_app_exited (ScsmApp *app,
                guchar  exit_code)
{
        g_return_if_fail (SCSM_IS_APP (app));

        g_signal_emit (app, signals[EXITED], 0, exit_code);
}

void
scsm_app_died (ScsmApp *app,
              int     signal)
{
        g_return_if_fail (SCSM_IS_APP (app));

        g_signal_emit (app, signals[DIED], 0, signal);
}

gboolean
scsm_app_get_registered (ScsmApp *app)
{
        g_return_val_if_fail (SCSM_IS_APP (app), FALSE);

        return app->priv->registered;
}

void
scsm_app_set_registered (ScsmApp   *app,
                        gboolean  registered)
{
        g_return_if_fail (SCSM_IS_APP (app));

        if (app->priv->registered != registered) {
                app->priv->registered = registered;
                g_object_notify (G_OBJECT (app), "registered");
        }
}

gboolean
scsm_app_save_to_keyfile (ScsmApp    *app,
                         GKeyFile  *keyfile,
                         GError   **error)
{
        g_debug ("Saving app: %s", app->priv->id);
        return SCSM_APP_GET_CLASS (app)->impl_save_to_keyfile (app, keyfile, error);
}
