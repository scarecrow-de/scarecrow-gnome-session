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

#ifndef __SCSM_INHIBITOR_FLAG_H__
#define __SCSM_INHIBITOR_FLAG_H__

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum {
        SCSM_INHIBITOR_FLAG_LOGOUT      = 1 << 0,
        SCSM_INHIBITOR_FLAG_SWITCH_USER = 1 << 1,
        SCSM_INHIBITOR_FLAG_SUSPEND     = 1 << 2,
        SCSM_INHIBITOR_FLAG_IDLE        = 1 << 3,
        SCSM_INHIBITOR_FLAG_AUTOMOUNT   = 1 << 4
} ScsmInhibitorFlag;

G_END_DECLS

#endif /* __SCSM_INHIBITOR_FLAG_H__ */
