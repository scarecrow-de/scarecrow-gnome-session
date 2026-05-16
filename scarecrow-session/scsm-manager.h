/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2008 William Jon McCann <jmccann@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef __SCSM_MANAGER_H
#define __SCSM_MANAGER_H

#include <glib-object.h>

#include "scsm-store.h"
#include "scsm-manager-logout-mode.h"

G_BEGIN_DECLS

#define SCSM_TYPE_MANAGER         (scsm_manager_get_type ())
#define SCSM_MANAGER(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), SCSM_TYPE_MANAGER, ScsmManager))
#define SCSM_MANAGER_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), SCSM_TYPE_MANAGER, ScsmManagerClass))
#define SCSM_IS_MANAGER(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), SCSM_TYPE_MANAGER))
#define SCSM_IS_MANAGER_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), SCSM_TYPE_MANAGER))
#define SCSM_MANAGER_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), SCSM_TYPE_MANAGER, ScsmManagerClass))

typedef struct ScsmManagerPrivate ScsmManagerPrivate;

typedef struct
{
        GObject            parent;
        ScsmManagerPrivate *priv;
} ScsmManager;

typedef struct
{
        GObjectClass   parent_class;

        void          (* phase_changed)       (ScsmManager      *manager,
                                               const char      *phase);
} ScsmManagerClass;

typedef enum {
        /* scsm's own startup/initialization phase */
        SCSM_MANAGER_PHASE_STARTUP = 0,
        /* scarecrow-initial-setup */
        SCSM_MANAGER_PHASE_EARLY_INITIALIZATION,
        /* scarecrow-keyring-daemon */
        SCSM_MANAGER_PHASE_PRE_DISPLAY_SERVER,
        /* wayland compositor and XWayland */
        SCSM_MANAGER_PHASE_DISPLAY_SERVER,
        /* xrandr setup, scarecrow-settings-daemon, etc */
        SCSM_MANAGER_PHASE_INITIALIZATION,
        /* window/compositing managers */
        SCSM_MANAGER_PHASE_WINDOW_MANAGER,
        /* apps that will create _NET_WM_WINDOW_TYPE_PANEL windows */
        SCSM_MANAGER_PHASE_PANEL,
        /* apps that will create _NET_WM_WINDOW_TYPE_DESKTOP windows */
        SCSM_MANAGER_PHASE_DESKTOP,
        /* everything else */
        SCSM_MANAGER_PHASE_APPLICATION,
        /* done launching */
        SCSM_MANAGER_PHASE_RUNNING,
        /* shutting down */
        SCSM_MANAGER_PHASE_QUERY_END_SESSION,
        SCSM_MANAGER_PHASE_END_SESSION,
        SCSM_MANAGER_PHASE_EXIT
} ScsmManagerPhase;

typedef enum
{
        SCSM_MANAGER_ERROR_GENERAL = 0,
        SCSM_MANAGER_ERROR_NOT_IN_INITIALIZATION,
        SCSM_MANAGER_ERROR_NOT_IN_RUNNING,
        SCSM_MANAGER_ERROR_ALREADY_REGISTERED,
        SCSM_MANAGER_ERROR_NOT_REGISTERED,
        SCSM_MANAGER_ERROR_INVALID_OPTION,
        SCSM_MANAGER_ERROR_LOCKED_DOWN,
        SCSM_MANAGER_NUM_ERRORS
} ScsmManagerError;

#define SCSM_MANAGER_ERROR scsm_manager_error_quark ()
GQuark              scsm_manager_error_quark                    (void);

GType               scsm_manager_get_type                       (void);

ScsmManager *        scsm_manager_new                            (ScsmStore       *client_store,
                                                                gboolean        failsafe,
                                                                gboolean        systemd_managed);
ScsmManager *        scsm_manager_get                            (void);

gboolean            scsm_manager_get_failsafe                   (ScsmManager     *manager);
gboolean            scsm_manager_get_systemd_managed            (ScsmManager     *manager);

gboolean            scsm_manager_add_autostart_app              (ScsmManager     *manager,
                                                                const char     *path,
                                                                const char     *provides);
gboolean            scsm_manager_add_required_app               (ScsmManager     *manager,
                                                                const char     *path,
                                                                const char     *provides);
gboolean            scsm_manager_add_autostart_apps_from_dir    (ScsmManager     *manager,
                                                                const char     *path);
gboolean            scsm_manager_add_legacy_session_apps        (ScsmManager     *manager,
                                                                const char     *path);

void                scsm_manager_start                          (ScsmManager     *manager);

char *              _scsm_manager_get_default_session           (ScsmManager     *manager);

void                _scsm_manager_set_active_session            (ScsmManager     *manager,
                                                                const char     *session_name,
                                                                gboolean        is_fallback);

void                _scsm_manager_set_renderer                  (ScsmManager     *manager,
                                                                const char     *renderer);

gboolean            scsm_manager_logout                         (ScsmManager     *manager,
                                                                guint           logout_mode,
                                                                GError        **error);

gboolean            scsm_manager_set_phase                      (ScsmManager     *manager,
                                                                ScsmManagerPhase phase);

G_END_DECLS

#endif /* __SCSM_MANAGER_H */
