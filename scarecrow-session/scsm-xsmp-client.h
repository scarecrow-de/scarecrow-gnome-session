/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2007 Novell, Inc.
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

#ifndef __SCSM_XSMP_CLIENT_H__
#define __SCSM_XSMP_CLIENT_H__

#include "scsm-client.h"

#include <X11/SM/SMlib.h>

G_BEGIN_DECLS

#define SCSM_TYPE_XSMP_CLIENT            (scsm_xsmp_client_get_type ())
#define SCSM_XSMP_CLIENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCSM_TYPE_XSMP_CLIENT, ScsmXSMPClient))
#define SCSM_XSMP_CLIENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SCSM_TYPE_XSMP_CLIENT, ScsmXSMPClientClass))
#define SCSM_IS_XSMP_CLIENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCSM_TYPE_XSMP_CLIENT))
#define SCSM_IS_XSMP_CLIENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SCSM_TYPE_XSMP_CLIENT))
#define SCSM_XSMP_CLIENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCSM_TYPE_XSMP_CLIENT, ScsmXSMPClientClass))

typedef struct _ScsmXSMPClient        ScsmXSMPClient;
typedef struct _ScsmXSMPClientClass   ScsmXSMPClientClass;

typedef struct ScsmXSMPClientPrivate  ScsmXSMPClientPrivate;

struct _ScsmXSMPClient
{
        ScsmClient             parent;
        ScsmXSMPClientPrivate *priv;
};

struct _ScsmXSMPClientClass
{
        ScsmClientClass parent_class;

        /* signals */
        gboolean (*register_request)     (ScsmXSMPClient  *client,
                                          char          **client_id);
        void     (*register_confirmed)   (ScsmXSMPClient  *client,
                                          const char     *client_id);
        gboolean (*logout_request)       (ScsmXSMPClient  *client,
                                          gboolean        prompt);


        void     (*saved_state)          (ScsmXSMPClient  *client);

        void     (*request_phase2)       (ScsmXSMPClient  *client);

        void     (*request_interaction)  (ScsmXSMPClient  *client);
        void     (*interaction_done)     (ScsmXSMPClient  *client,
                                          gboolean        cancel_shutdown);

        void     (*save_yourself_done)   (ScsmXSMPClient  *client);

};

GType       scsm_xsmp_client_get_type             (void) G_GNUC_CONST;

ScsmClient  *scsm_xsmp_client_new                  (IceConn         ice_conn);

void        scsm_xsmp_client_connect              (ScsmXSMPClient  *client,
                                                  SmsConn         conn,
                                                  unsigned long  *mask_ret,
                                                  SmsCallbacks   *callbacks_ret);

void        scsm_xsmp_client_save_state           (ScsmXSMPClient  *client);
void        scsm_xsmp_client_save_yourself        (ScsmXSMPClient  *client,
                                                  gboolean        save_state);
void        scsm_xsmp_client_save_yourself_phase2 (ScsmXSMPClient  *client);
void        scsm_xsmp_client_interact             (ScsmXSMPClient  *client);
void        scsm_xsmp_client_shutdown_cancelled   (ScsmXSMPClient  *client);

G_END_DECLS

#endif /* __SCSM_XSMP_CLIENT_H__ */
