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


#ifndef __SCSM_XSMP_SERVER_H
#define __SCSM_XSMP_SERVER_H

#include <glib-object.h>

#include "scsm-store.h"

G_BEGIN_DECLS

#define SCSM_TYPE_XSMP_SERVER         (scsm_xsmp_server_get_type ())
#define SCSM_XSMP_SERVER(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), SCSM_TYPE_XSMP_SERVER, GsmXsmpServer))
#define SCSM_XSMP_SERVER_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), SCSM_TYPE_XSMP_SERVER, GsmXsmpServerClass))
#define SCSM_IS_XSMP_SERVER(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), SCSM_TYPE_XSMP_SERVER))
#define SCSM_IS_XSMP_SERVER_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), SCSM_TYPE_XSMP_SERVER))
#define SCSM_XSMP_SERVER_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), SCSM_TYPE_XSMP_SERVER, GsmXsmpServerClass))

typedef struct GsmXsmpServerPrivate GsmXsmpServerPrivate;

typedef struct
{
        GObject            parent;
        GsmXsmpServerPrivate *priv;
} GsmXsmpServer;

typedef struct
{
        GObjectClass   parent_class;
} GsmXsmpServerClass;

GType               scsm_xsmp_server_get_type                       (void);

GsmXsmpServer *     scsm_xsmp_server_new                            (GsmStore      *client_store);
void                scsm_xsmp_server_start                          (GsmXsmpServer *server);
void                scsm_xsmp_server_stop_accepting_new_clients     (GsmXsmpServer *server);
void                scsm_xsmp_server_start_accepting_new_clients    (GsmXsmpServer *server);

G_END_DECLS

#endif /* __SCSM_XSMP_SERVER_H */
