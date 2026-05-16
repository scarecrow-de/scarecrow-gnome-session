/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
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

#ifndef __SCSM_INHIBITOR_H__
#define __SCSM_INHIBITOR_H__

#include <glib-object.h>
#include <sys/types.h>

#include "scsm-inhibitor-flag.h"

G_BEGIN_DECLS

#define SCSM_TYPE_INHIBITOR            (scsm_inhibitor_get_type ())
#define SCSM_INHIBITOR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCSM_TYPE_INHIBITOR, ScsmInhibitor))
#define SCSM_INHIBITOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SCSM_TYPE_INHIBITOR, ScsmInhibitorClass))
#define SCSM_IS_INHIBITOR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCSM_TYPE_INHIBITOR))
#define SCSM_IS_INHIBITOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SCSM_TYPE_INHIBITOR))
#define SCSM_INHIBITOR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCSM_TYPE_INHIBITOR, ScsmInhibitorClass))

typedef struct _ScsmInhibitor        ScsmInhibitor;
typedef struct _ScsmInhibitorClass   ScsmInhibitorClass;

typedef struct ScsmInhibitorPrivate ScsmInhibitorPrivate;

struct _ScsmInhibitor
{
        GObject              parent;
        ScsmInhibitorPrivate *priv;
};

struct _ScsmInhibitorClass
{
        GObjectClass parent_class;
};

typedef enum
{
        SCSM_INHIBITOR_ERROR_GENERAL = 0,
        SCSM_INHIBITOR_ERROR_NOT_SET,
        SCSM_INHIBITOR_NUM_ERRORS
} ScsmInhibitorError;

#define SCSM_INHIBITOR_ERROR scsm_inhibitor_error_quark ()
GQuark         scsm_inhibitor_error_quark          (void);

GType          scsm_inhibitor_get_type             (void) G_GNUC_CONST;

ScsmInhibitor * scsm_inhibitor_new                  (const char    *app_id,
                                                   guint          toplevel_xid,
                                                   guint          flags,
                                                   const char    *reason,
                                                   const char    *bus_name,
                                                   guint          cookie);
ScsmInhibitor * scsm_inhibitor_new_for_client       (const char    *client_id,
                                                   const char    *app_id,
                                                   guint          flags,
                                                   const char    *reason,
                                                   const char    *bus_name,
                                                   guint          cookie);

const char *   scsm_inhibitor_peek_id              (ScsmInhibitor  *inhibitor);
const char *   scsm_inhibitor_peek_app_id          (ScsmInhibitor  *inhibitor);
const char *   scsm_inhibitor_peek_client_id       (ScsmInhibitor  *inhibitor);
const char *   scsm_inhibitor_peek_reason          (ScsmInhibitor  *inhibitor);
const char *   scsm_inhibitor_peek_bus_name        (ScsmInhibitor  *inhibitor);
guint          scsm_inhibitor_peek_cookie          (ScsmInhibitor  *inhibitor);
guint          scsm_inhibitor_peek_flags           (ScsmInhibitor  *inhibitor);
guint          scsm_inhibitor_peek_toplevel_xid    (ScsmInhibitor  *inhibitor);

G_END_DECLS

#endif /* __SCSM_INHIBITOR_H__ */
