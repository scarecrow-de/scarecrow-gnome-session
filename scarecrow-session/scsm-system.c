/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2012 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <glib-object.h>
#include <glib/gi18n.h>

#include "scsm-system.h"

#include "scsm-systemd.h"

#ifdef HAVE_CONSOLEKIT
#include "scsm-consolekit.h"
#endif

enum {
        REQUEST_COMPLETED,
        SHUTDOWN_PREPARED,
        LAST_SIGNAL
};

enum {
        PROP_0,
        PROP_ACTIVE
};

static guint signals[LAST_SIGNAL] = { 0 };

G_DEFINE_INTERFACE (ScsmSystem, scsm_system, G_TYPE_OBJECT)

static void
scsm_system_default_init (ScsmSystemInterface *iface)
{
        GParamSpec *pspec;
        signals [REQUEST_COMPLETED] =
                g_signal_new ("request-completed",
                              SCSM_TYPE_SYSTEM,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (ScsmSystemInterface, request_completed),
                              NULL, NULL, NULL,
                              G_TYPE_NONE,
                              1, G_TYPE_POINTER);
        signals[SHUTDOWN_PREPARED] =
                 g_signal_new ("shutdown-prepared",
                               SCSM_TYPE_SYSTEM,
                               G_SIGNAL_RUN_LAST,
                               G_STRUCT_OFFSET (ScsmSystemInterface, shutdown_prepared),
                               NULL, NULL, NULL,
                               G_TYPE_NONE,
                               1, G_TYPE_BOOLEAN);
        pspec = g_param_spec_boolean ("active",
                                      "Active",
                                      "Whether or not session is active",
                                      TRUE,
                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
        g_object_interface_install_property (iface, pspec);
}

typedef GObject ScsmSystemNull;
typedef GObjectClass ScsmSystemNullClass;

static void do_nothing (void) { }
static gboolean return_true (void) { return TRUE; }
static gboolean return_false (void) { return TRUE; }

static void
scsm_system_null_init_iface (ScsmSystemInterface *iface)
{
        iface->can_switch_user   = (void *) return_false;
        iface->can_stop          = (void *) return_false;
        iface->can_restart       = (void *) return_false;
        iface->can_restart_to_firmware_setup = (void *) return_false;
        iface->set_restart_to_firmware_setup = (void *) do_nothing;
        iface->can_suspend       = (void *) return_false;
        iface->can_hibernate     = (void *) return_false;
        iface->attempt_stop      = (void *) do_nothing;
        iface->attempt_restart   = (void *) do_nothing;
        iface->suspend           = (void *) do_nothing;
        iface->hibernate         = (void *) do_nothing;
        iface->set_session_idle  = (void *) do_nothing;
        iface->is_login_session  = (void *) return_true;
        iface->add_inhibitor     = (void *) do_nothing;
        iface->remove_inhibitor  = (void *) do_nothing;
        iface->prepare_shutdown  = (void *) do_nothing;
        iface->complete_shutdown = (void *) do_nothing;
        iface->is_last_session_for_user = (void *) return_false;
}

static void
scsm_system_null_init (ScsmSystemNull *gsn)
{
}

static void
scsm_system_null_get_property (GObject *object, guint prop_id,
                              GValue *value, GParamSpec *pspec)
{
        g_value_set_boolean (value, TRUE);
}

static void
scsm_system_null_class_init (ScsmSystemNullClass *class)
{
        class->get_property = scsm_system_null_get_property;
        class->set_property = (void *) do_nothing;

        g_object_class_override_property (class, 1, "active");
}

static GType scsm_system_null_get_type (void);
G_DEFINE_TYPE_WITH_CODE (ScsmSystemNull, scsm_system_null, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (SCSM_TYPE_SYSTEM, scsm_system_null_init_iface))

GQuark
scsm_system_error_quark (void)
{
        static GQuark error_quark = 0;

        if (error_quark == 0) {
                error_quark = g_quark_from_static_string ("scsm-system-error");
        }

        return error_quark;
}

gboolean
scsm_system_can_switch_user (ScsmSystem *system)
{
        return SCSM_SYSTEM_GET_IFACE (system)->can_switch_user (system);
}

gboolean
scsm_system_can_stop (ScsmSystem *system)
{
        return SCSM_SYSTEM_GET_IFACE (system)->can_stop (system);
}

gboolean
scsm_system_can_restart (ScsmSystem *system)
{
        return SCSM_SYSTEM_GET_IFACE (system)->can_restart (system);
}

gboolean
scsm_system_can_restart_to_firmware_setup (ScsmSystem *system)
{
        return SCSM_SYSTEM_GET_IFACE (system)->can_restart_to_firmware_setup (system);
}

void
scsm_system_set_restart_to_firmware_setup (ScsmSystem *system,
                                          gboolean   enable)
{
        SCSM_SYSTEM_GET_IFACE (system)->set_restart_to_firmware_setup (system, enable);
}

gboolean
scsm_system_can_suspend (ScsmSystem *system)
{
        return SCSM_SYSTEM_GET_IFACE (system)->can_suspend (system);
}

gboolean
scsm_system_can_hibernate (ScsmSystem *system)
{
        return SCSM_SYSTEM_GET_IFACE (system)->can_hibernate (system);
}

void
scsm_system_attempt_stop (ScsmSystem *system)
{
        SCSM_SYSTEM_GET_IFACE (system)->attempt_stop (system);
}

void
scsm_system_attempt_restart (ScsmSystem *system)
{
        SCSM_SYSTEM_GET_IFACE (system)->attempt_restart (system);
}

void
scsm_system_suspend (ScsmSystem *system)
{
        SCSM_SYSTEM_GET_IFACE (system)->suspend (system);
}

void
scsm_system_hibernate (ScsmSystem *system)
{
        SCSM_SYSTEM_GET_IFACE (system)->hibernate (system);
}

void
scsm_system_set_session_idle (ScsmSystem *system,
                             gboolean   is_idle)
{
        SCSM_SYSTEM_GET_IFACE (system)->set_session_idle (system, is_idle);
}

void
scsm_system_add_inhibitor (ScsmSystem        *system,
                          const gchar      *id,
                          ScsmInhibitorFlag  flag)
{
        SCSM_SYSTEM_GET_IFACE (system)->add_inhibitor (system, id, flag);
}

void
scsm_system_remove_inhibitor (ScsmSystem   *system,
                             const gchar *id)
{
        SCSM_SYSTEM_GET_IFACE (system)->remove_inhibitor (system, id);
}

gboolean
scsm_system_is_login_session (ScsmSystem *system)
{
        return SCSM_SYSTEM_GET_IFACE (system)->is_login_session (system);
}

gboolean
scsm_system_is_last_session_for_user (ScsmSystem *system)
{
        return SCSM_SYSTEM_GET_IFACE (system)->is_last_session_for_user (system);
}

/**
 * scsm_system_is_active:
 *
 * Returns: %TRUE if the current session is in the foreground
 * Since: 3.8
 */
gboolean
scsm_system_is_active (ScsmSystem *system)
{
        gboolean is_active;
        g_object_get ((GObject*)system, "active", &is_active, NULL);
        return is_active;
}

void
scsm_system_prepare_shutdown  (ScsmSystem *system,
                              gboolean   restart)
{
        SCSM_SYSTEM_GET_IFACE (system)->prepare_shutdown (system, restart);
}

void
scsm_system_complete_shutdown (ScsmSystem *system)
{
        SCSM_SYSTEM_GET_IFACE (system)->complete_shutdown (system);
}

ScsmSystem *
scsm_get_system (void)
{
        static ScsmSystem *system = NULL;

        if (system == NULL) {
                system = SCSM_SYSTEM (scsm_systemd_new ());
                if (system != NULL) {
                        g_debug ("Using systemd for session tracking");
                }
        }

#ifdef HAVE_CONSOLEKIT
        if (system == NULL) {
                system = SCSM_SYSTEM (scsm_consolekit_new ());
                if (system != NULL) {
                        g_debug ("Using ConsoleKit for session tracking");
                }
        }
#endif

        if (system == NULL) {
                system = g_object_new (scsm_system_null_get_type (), NULL);
                g_warning ("Using null backend for session tracking");
        }

        return g_object_ref (system);
}
