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

#ifndef __SCSM_AUTOSTART_APP_H__
#define __SCSM_AUTOSTART_APP_H__

#include "scsm-app.h"

#include <X11/SM/SMlib.h>

G_BEGIN_DECLS

#define SCSM_TYPE_AUTOSTART_APP            (scsm_autostart_app_get_type ())
#define SCSM_AUTOSTART_APP(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCSM_TYPE_AUTOSTART_APP, GsmAutostartApp))
#define SCSM_AUTOSTART_APP_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SCSM_TYPE_AUTOSTART_APP, GsmAutostartAppClass))
#define SCSM_IS_AUTOSTART_APP(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCSM_TYPE_AUTOSTART_APP))
#define SCSM_IS_AUTOSTART_APP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SCSM_TYPE_AUTOSTART_APP))
#define SCSM_AUTOSTART_APP_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCSM_TYPE_AUTOSTART_APP, GsmAutostartAppClass))

typedef struct _GsmAutostartApp        GsmAutostartApp;
typedef struct _GsmAutostartAppClass   GsmAutostartAppClass;
typedef struct _GsmAutostartAppPrivate GsmAutostartAppPrivate;

struct _GsmAutostartApp
{
        GsmApp parent;

        GsmAutostartAppPrivate *priv;
};

struct _GsmAutostartAppClass
{
        GsmAppClass parent_class;

        /* signals */
        void     (*condition_changed)  (GsmApp  *app,
                                        gboolean condition);
};

GType   scsm_autostart_app_get_type           (void) G_GNUC_CONST;

GsmApp *scsm_autostart_app_new                (const char *desktop_file,
                                              gboolean    mask_systemd,
                                              GError    **error);

void    scsm_autostart_app_add_provides       (GsmAutostartApp *aapp,
                                              const char      *provides);

#define SCSM_AUTOSTART_APP_SYSTEMD_KEY     "X-GNOME-HiddenUnderSystemd"
#define SCSM_AUTOSTART_APP_ENABLED_KEY     "X-GNOME-Autostart-enabled"
#define SCSM_AUTOSTART_APP_PHASE_KEY       "X-GNOME-Autostart-Phase"
#define SCSM_AUTOSTART_APP_PROVIDES_KEY    "X-GNOME-Provides"
#define SCSM_AUTOSTART_APP_STARTUP_ID_KEY  "X-GNOME-Autostart-startup-id"
#define SCSM_AUTOSTART_APP_AUTORESTART_KEY "X-GNOME-AutoRestart"
#define SCSM_AUTOSTART_APP_DBUS_NAME_KEY   "X-GNOME-DBus-Name"
#define SCSM_AUTOSTART_APP_DBUS_PATH_KEY   "X-GNOME-DBus-Path"
#define SCSM_AUTOSTART_APP_DBUS_ARGS_KEY   "X-GNOME-DBus-Start-Arguments"
#define SCSM_AUTOSTART_APP_DISCARD_KEY     "X-GNOME-Autostart-discard-exec"

G_END_DECLS

#endif /* __SCSM_AUTOSTART_APP_H__ */
