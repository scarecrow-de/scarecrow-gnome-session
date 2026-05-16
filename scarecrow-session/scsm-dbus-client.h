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

#ifndef __SCSM_DBUS_CLIENT_H__
#define __SCSM_DBUS_CLIENT_H__

#include "scsm-client.h"

G_BEGIN_DECLS

#define SCSM_TYPE_DBUS_CLIENT            (scsm_dbus_client_get_type ())
#define SCSM_DBUS_CLIENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCSM_TYPE_DBUS_CLIENT, ScsmDBusClient))
#define SCSM_DBUS_CLIENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SCSM_TYPE_DBUS_CLIENT, ScsmDBusClientClass))
#define SCSM_IS_DBUS_CLIENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCSM_TYPE_DBUS_CLIENT))
#define SCSM_IS_DBUS_CLIENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SCSM_TYPE_DBUS_CLIENT))
#define SCSM_DBUS_CLIENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCSM_TYPE_DBUS_CLIENT, ScsmDBusClientClass))

typedef struct _ScsmDBusClient        ScsmDBusClient;
typedef struct _ScsmDBusClientClass   ScsmDBusClientClass;

typedef struct ScsmDBusClientPrivate  ScsmDBusClientPrivate;

struct _ScsmDBusClient
{
        ScsmClient             parent;
        ScsmDBusClientPrivate *priv;
};

struct _ScsmDBusClientClass
{
        ScsmClientClass parent_class;
};

GType          scsm_dbus_client_get_type           (void) G_GNUC_CONST;

ScsmClient *    scsm_dbus_client_new                (const char     *startup_id,
                                                   const char     *bus_name);
const char *   scsm_dbus_client_get_bus_name       (ScsmDBusClient  *client);

G_END_DECLS

#endif /* __SCSM_DBUS_CLIENT_H__ */
