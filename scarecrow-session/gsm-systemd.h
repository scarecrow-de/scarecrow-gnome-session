/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2012 Red Hat, Inc.
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
 *	Matthias Clasen <mclasen@redhat.com>
 */

#ifndef __SCSM_SYSTEMD_H__
#define __SCSM_SYSTEMD_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define SCSM_TYPE_SYSTEMD             (scsm_systemd_get_type ())
#define SCSM_SYSTEMD(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCSM_TYPE_SYSTEMD, GsmSystemd))
#define SCSM_SYSTEMD_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), SCSM_TYPE_SYSTEMD, GsmSystemdClass))
#define SCSM_IS_SYSTEMD(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCSM_TYPE_SYSTEMD))
#define SCSM_IS_SYSTEMD_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCSM_TYPE_SYSTEMD))
#define SCSM_SYSTEMD_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), SCSM_TYPE_SYSTEMD, GsmSystemdClass))

typedef struct _GsmSystemd        GsmSystemd;
typedef struct _GsmSystemdClass   GsmSystemdClass;
typedef struct _GsmSystemdPrivate GsmSystemdPrivate;

struct _GsmSystemd
{
        GObject            parent;

        GsmSystemdPrivate *priv;
};

struct _GsmSystemdClass
{
        GObjectClass parent_class;
};

GType         scsm_systemd_get_type (void);

GsmSystemd   *scsm_systemd_new      (void) G_GNUC_MALLOC;

G_END_DECLS

#endif /* __SCSM_SYSTEMD_H__ */
