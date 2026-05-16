/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2008 Jon McCann <jmccann@redhat.com>
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
 *	Jon McCann <jmccann@redhat.com>
 */

#ifndef __SCSM_CONSOLEKIT_H__
#define __SCSM_CONSOLEKIT_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define SCSM_TYPE_CONSOLEKIT             (scsm_consolekit_get_type ())
#define SCSM_CONSOLEKIT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCSM_TYPE_CONSOLEKIT, ScsmConsolekit))
#define SCSM_CONSOLEKIT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), SCSM_TYPE_CONSOLEKIT, ScsmConsolekitClass))
#define SCSM_IS_CONSOLEKIT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCSM_TYPE_CONSOLEKIT))
#define SCSM_IS_CONSOLEKIT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCSM_TYPE_CONSOLEKIT))
#define SCSM_CONSOLEKIT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), SCSM_TYPE_CONSOLEKIT, ScsmConsolekitClass))

typedef struct _ScsmConsolekit        ScsmConsolekit;
typedef struct _ScsmConsolekitClass   ScsmConsolekitClass;
typedef struct _ScsmConsolekitPrivate ScsmConsolekitPrivate;

struct _ScsmConsolekit
{
        GObject               parent;

        ScsmConsolekitPrivate *priv;
};

struct _ScsmConsolekitClass
{
        GObjectClass parent_class;
};

GType            scsm_consolekit_get_type        (void);

ScsmConsolekit   *scsm_consolekit_new             (void) G_GNUC_MALLOC;

G_END_DECLS

#endif /* __SCSM_CONSOLEKIT_H__ */
