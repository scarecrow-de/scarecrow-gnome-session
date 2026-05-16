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

#ifndef __SCSM_CLIENT_H__
#define __SCSM_CLIENT_H__

#include <glib.h>
#include <glib-object.h>
#include <sys/types.h>

G_BEGIN_DECLS

#define SCSM_TYPE_CLIENT            (scsm_client_get_type ())
#define SCSM_CLIENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCSM_TYPE_CLIENT, ScsmClient))
#define SCSM_CLIENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SCSM_TYPE_CLIENT, ScsmClientClass))
#define SCSM_IS_CLIENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCSM_TYPE_CLIENT))
#define SCSM_IS_CLIENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SCSM_TYPE_CLIENT))
#define SCSM_CLIENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCSM_TYPE_CLIENT, ScsmClientClass))

typedef struct _ScsmApp           ScsmApp;
typedef struct _ScsmClient        ScsmClient;
typedef struct _ScsmClientClass   ScsmClientClass;

typedef struct ScsmClientPrivate ScsmClientPrivate;

typedef enum {
        SCSM_CLIENT_UNREGISTERED = 0,
        SCSM_CLIENT_REGISTERED,
        SCSM_CLIENT_FINISHED,
        SCSM_CLIENT_FAILED
} ScsmClientStatus;

typedef enum {
        SCSM_CLIENT_RESTART_NEVER = 0,
        SCSM_CLIENT_RESTART_IF_RUNNING,
        SCSM_CLIENT_RESTART_ANYWAY,
        SCSM_CLIENT_RESTART_IMMEDIATELY
} ScsmClientRestartStyle;

typedef enum {
        SCSM_CLIENT_END_SESSION_FLAG_FORCEFUL = 1 << 0,
        SCSM_CLIENT_END_SESSION_FLAG_SAVE     = 1 << 1,
        SCSM_CLIENT_END_SESSION_FLAG_LAST     = 1 << 2
} ScsmClientEndSessionFlag;

struct _ScsmClient
{
        GObject           parent;
        ScsmClientPrivate *priv;
};

struct _ScsmClientClass
{
        GObjectClass parent_class;

        /* signals */
        void         (*disconnected)               (ScsmClient  *client);
        void         (*end_session_response)       (ScsmClient  *client,
                                                    gboolean    ok,
                                                    gboolean    do_last,
                                                    gboolean    cancel,
                                                    const char *reason);

        /* virtual methods */
        char *                (*impl_get_app_name)           (ScsmClient *client);
        ScsmClientRestartStyle (*impl_get_restart_style_hint) (ScsmClient *client);
        guint                 (*impl_get_unix_process_id)    (ScsmClient *client);
        gboolean              (*impl_query_end_session)      (ScsmClient *client,
                                                              ScsmClientEndSessionFlag flags,
                                                              GError   **error);
        gboolean              (*impl_end_session)            (ScsmClient *client,
                                                              ScsmClientEndSessionFlag flags,
                                                              GError   **error);
        gboolean              (*impl_cancel_end_session)     (ScsmClient *client,
                                                              GError   **error);
        gboolean              (*impl_stop)                   (ScsmClient *client,
                                                              GError   **error);
        GKeyFile *            (*impl_save)                   (ScsmClient *client,
                                                              ScsmApp    *app,
                                                              GError   **error);
};

typedef enum
{
        SCSM_CLIENT_ERROR_GENERAL = 0,
        SCSM_CLIENT_ERROR_NOT_REGISTERED,
        SCSM_CLIENT_NUM_ERRORS
} ScsmClientError;

#define SCSM_CLIENT_ERROR scsm_client_error_quark ()
GQuark                scsm_client_error_quark                (void);

GType                 scsm_client_get_type                   (void) G_GNUC_CONST;

const char           *scsm_client_peek_id                    (ScsmClient  *client);


const char *          scsm_client_peek_startup_id            (ScsmClient  *client);
const char *          scsm_client_peek_app_id                (ScsmClient  *client);
guint                 scsm_client_peek_restart_style_hint    (ScsmClient  *client);
guint                 scsm_client_peek_status                (ScsmClient  *client);


char                 *scsm_client_get_app_name               (ScsmClient  *client);
void                  scsm_client_set_app_id                 (ScsmClient  *client,
                                                             const char *app_id);
void                  scsm_client_set_status                 (ScsmClient  *client,
                                                             guint       status);

gboolean              scsm_client_end_session                (ScsmClient  *client,
                                                             guint       flags,
                                                             GError    **error);
gboolean              scsm_client_query_end_session          (ScsmClient  *client,
                                                             guint       flags,
                                                             GError    **error);
gboolean              scsm_client_cancel_end_session         (ScsmClient  *client,
                                                             GError    **error);

void                  scsm_client_disconnected               (ScsmClient  *client);

GKeyFile             *scsm_client_save                       (ScsmClient  *client,
                                                             ScsmApp     *app,
                                                             GError    **error);

gboolean              scsm_client_stop                       (ScsmClient  *client,
                                                             GError    **error);

/* private */

void                  scsm_client_end_session_response       (ScsmClient  *client,
                                                             gboolean    is_ok,
                                                             gboolean    do_last,
                                                             gboolean    cancel,
                                                             const char *reason);

G_END_DECLS

#endif /* __SCSM_CLIENT_H__ */
