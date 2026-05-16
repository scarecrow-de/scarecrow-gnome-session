/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2010 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *    Ray Strode <rstrode@redhat.com>
 */

#ifndef __SCSM_SHELL_H__
#define __SCSM_SHELL_H__

#include <glib.h>
#include <glib-object.h>

#include "scsm-store.h"

G_BEGIN_DECLS

#define SCSM_TYPE_SHELL             (scsm_shell_get_type ())
#define SCSM_SHELL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCSM_TYPE_SHELL, ScsmShell))
#define SCSM_SHELL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), SCSM_TYPE_SHELL, ScsmShellClass))
#define SCSM_IS_SHELL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCSM_TYPE_SHELL))
#define SCSM_IS_SHELL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCSM_TYPE_SHELL))
#define SCSM_SHELL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), SCSM_TYPE_SHELL, ScsmShellClass))
#define SCSM_SHELL_ERROR            (scsm_shell_error_quark ())

typedef struct _ScsmShell        ScsmShell;
typedef struct _ScsmShellClass   ScsmShellClass;
typedef struct _ScsmShellPrivate ScsmShellPrivate;

typedef enum
{
    SCSM_SHELL_END_SESSION_DIALOG_TYPE_LOGOUT = 0,
    SCSM_SHELL_END_SESSION_DIALOG_TYPE_SHUTDOWN,
    SCSM_SHELL_END_SESSION_DIALOG_TYPE_RESTART,
} ScsmShellEndSessionDialogType;

struct _ScsmShell
{
        GObject               parent;

        ScsmShellPrivate *priv;
};

struct _ScsmShellClass
{
        GObjectClass parent_class;

        void (* end_session_dialog_opened)        (ScsmShell *shell);
        void (* end_session_dialog_open_failed)   (ScsmShell *shell);
        void (* end_session_dialog_closed)        (ScsmShell *shell);
        void (* end_session_dialog_canceled)      (ScsmShell *shell);

        void (* end_session_dialog_confirmed_logout)   (ScsmShell *shell);
        void (* end_session_dialog_confirmed_shutdown) (ScsmShell *shell);
        void (* end_session_dialog_confirmed_reboot)   (ScsmShell *shell);

};

GType            scsm_shell_get_type           (void);

ScsmShell        *scsm_shell_new                (void);

ScsmShell        *scsm_get_shell                (void);
gboolean         scsm_shell_is_running         (ScsmShell *shell);

gboolean         scsm_shell_open_end_session_dialog (ScsmShell *shell,
                                                    ScsmShellEndSessionDialogType type,
                                                    ScsmStore *inhibitors);
void             scsm_shell_close_end_session_dialog (ScsmShell *shell);

G_END_DECLS

#endif /* __SCSM_SHELL_H__ */
