/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2009 Red Hat, Inc.
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

#ifndef __SCSM_PRESENCE_H__
#define __SCSM_PRESENCE_H__

#include <glib-object.h>
#include <sys/types.h>

#include "scsm-presence-flag.h"

G_BEGIN_DECLS

#define SCSM_TYPE_PRESENCE            (scsm_presence_get_type ())
#define SCSM_PRESENCE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCSM_TYPE_PRESENCE, ScsmPresence))
#define SCSM_PRESENCE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SCSM_TYPE_PRESENCE, ScsmPresenceClass))
#define SCSM_IS_PRESENCE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCSM_TYPE_PRESENCE))
#define SCSM_IS_PRESENCE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SCSM_TYPE_PRESENCE))
#define SCSM_PRESENCE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCSM_TYPE_PRESENCE, ScsmPresenceClass))

typedef struct _ScsmPresence        ScsmPresence;
typedef struct _ScsmPresenceClass   ScsmPresenceClass;

typedef struct ScsmPresencePrivate ScsmPresencePrivate;

struct _ScsmPresence
{
        GObject             parent;
        ScsmPresencePrivate *priv;
};

struct _ScsmPresenceClass
{
        GObjectClass parent_class;

        void          (* status_changed)        (ScsmPresence     *presence,
                                                 guint            status);
};

typedef enum
{
        SCSM_PRESENCE_ERROR_GENERAL = 0,
        SCSM_PRESENCE_NUM_ERRORS
} ScsmPresenceError;

#define SCSM_PRESENCE_ERROR scsm_presence_error_quark ()
GQuark         scsm_presence_error_quark          (void);

GType          scsm_presence_get_type             (void) G_GNUC_CONST;

ScsmPresence *  scsm_presence_new                  (void);

void           scsm_presence_set_idle_enabled     (ScsmPresence  *presence,
                                                  gboolean      enabled);
void           scsm_presence_set_idle_timeout     (ScsmPresence  *presence,
                                                  guint         n_seconds);

G_END_DECLS

#endif /* __SCSM_PRESENCE_H__ */
