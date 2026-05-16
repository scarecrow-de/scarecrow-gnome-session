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

#ifndef __SCSM_APP_H__
#define __SCSM_APP_H__

#include <glib-object.h>
#include <sys/types.h>


#include "scsm-manager.h"
#include "scsm-client.h"

G_BEGIN_DECLS

#define SCSM_TYPE_APP            (scsm_app_get_type ())
#define SCSM_APP(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCSM_TYPE_APP, ScsmApp))
#define SCSM_APP_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SCSM_TYPE_APP, ScsmAppClass))
#define SCSM_IS_APP(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCSM_TYPE_APP))
#define SCSM_IS_APP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SCSM_TYPE_APP))
#define SCSM_APP_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCSM_TYPE_APP, ScsmAppClass))

typedef struct _ScsmApp        ScsmApp;
typedef struct _ScsmAppClass   ScsmAppClass;
typedef struct _ScsmAppPrivate ScsmAppPrivate;

struct _ScsmApp
{
        GObject        parent;
        ScsmAppPrivate *priv;
};

struct _ScsmAppClass
{
        GObjectClass parent_class;

        /* signals */
        void        (*exited)       (ScsmApp *app,
                                     guchar  exit_code);
        void        (*died)         (ScsmApp *app,
                                     int     signal);

        /* virtual methods */
        gboolean    (*impl_start)                     (ScsmApp     *app,
                                                       GError    **error);
        gboolean    (*impl_restart)                   (ScsmApp     *app,
                                                       GError    **error);
        gboolean    (*impl_stop)                      (ScsmApp     *app,
                                                       GError    **error);
        gboolean    (*impl_provides)                  (ScsmApp     *app,
                                                       const char *service);
        char **     (*impl_get_provides)              (ScsmApp     *app);
        gboolean    (*impl_has_autostart_condition)   (ScsmApp     *app,
                                                       const char *service);
        gboolean    (*impl_is_running)                (ScsmApp     *app);

        gboolean    (*impl_get_autorestart)           (ScsmApp     *app);
        const char *(*impl_get_app_id)                (ScsmApp     *app);
        gboolean    (*impl_is_disabled)               (ScsmApp     *app);
        gboolean    (*impl_is_conditionally_disabled) (ScsmApp     *app);

        gboolean    (*impl_save_to_keyfile)           (ScsmApp     *app,
                                                       GKeyFile   *keyfile,
                                                       GError    **error);
};

typedef enum
{
        SCSM_APP_ERROR_GENERAL = 0,
        SCSM_APP_ERROR_RESTART_LIMIT,
        SCSM_APP_ERROR_START,
        SCSM_APP_ERROR_STOP,
        SCSM_APP_NUM_ERRORS
} ScsmAppError;

#define SCSM_APP_ERROR scsm_app_error_quark ()

GQuark           scsm_app_error_quark                    (void);
GType            scsm_app_get_type                       (void) G_GNUC_CONST;

gboolean         scsm_app_peek_autorestart               (ScsmApp     *app);

const char      *scsm_app_peek_id                        (ScsmApp     *app);
const char      *scsm_app_peek_app_id                    (ScsmApp     *app);
const char      *scsm_app_peek_startup_id                (ScsmApp     *app);
ScsmManagerPhase  scsm_app_peek_phase                     (ScsmApp     *app);
gboolean         scsm_app_peek_is_disabled               (ScsmApp     *app);
gboolean         scsm_app_peek_is_conditionally_disabled (ScsmApp     *app);

gboolean         scsm_app_start                          (ScsmApp     *app,
                                                         GError    **error);
gboolean         scsm_app_restart                        (ScsmApp     *app,
                                                         GError    **error);
gboolean         scsm_app_stop                           (ScsmApp     *app,
                                                         GError    **error);
gboolean         scsm_app_is_running                     (ScsmApp     *app);

void             scsm_app_exited                         (ScsmApp     *app,
                                                         guchar      exit_code);
void             scsm_app_died                           (ScsmApp     *app,
                                                         int         signal);

gboolean         scsm_app_provides                       (ScsmApp     *app,
                                                         const char *service);
char           **scsm_app_get_provides                   (ScsmApp     *app);
gboolean         scsm_app_has_autostart_condition        (ScsmApp     *app,
                                                         const char *condition);
gboolean         scsm_app_get_registered                 (ScsmApp     *app);
void             scsm_app_set_registered                 (ScsmApp     *app,
                                                         gboolean  registered);

gboolean         scsm_app_save_to_keyfile                (ScsmApp    *app,
                                                         GKeyFile  *keyfile,
                                                         GError   **error);

G_END_DECLS

#endif /* __SCSM_APP_H__ */
