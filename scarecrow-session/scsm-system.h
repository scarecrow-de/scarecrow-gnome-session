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

#ifndef __SCSM_SYSTEM_H__
#define __SCSM_SYSTEM_H__

#include <glib.h>
#include <glib-object.h>

#include "scsm-inhibitor.h"

G_BEGIN_DECLS

#define SCSM_TYPE_SYSTEM             (scsm_system_get_type ())
#define SCSM_SYSTEM(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCSM_TYPE_SYSTEM, ScsmSystem))
#define SCSM_SYSTEM_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), SCSM_TYPE_SYSTEM, ScsmSystemInterface))
#define SCSM_IS_SYSTEM(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCSM_TYPE_SYSTEM))
#define SCSM_SYSTEM_GET_IFACE(obj)   (G_TYPE_INSTANCE_GET_INTERFACE((obj), SCSM_TYPE_SYSTEM, ScsmSystemInterface))
#define SCSM_SYSTEM_ERROR            (scsm_system_error_quark ())

typedef struct _ScsmSystem          ScsmSystem;
typedef struct _ScsmSystemInterface ScsmSystemInterface;
typedef enum   _ScsmSystemError     ScsmSystemError;

struct _ScsmSystemInterface
{
        GTypeInterface base_interface;

        void (* request_completed)    (ScsmSystem *system,
                                       GError    *error);

        void (* shutdown_prepared)    (ScsmSystem *system,
                                       gboolean   success);

        gboolean (* can_switch_user)  (ScsmSystem *system);
        gboolean (* can_stop)         (ScsmSystem *system);
        gboolean (* can_restart)      (ScsmSystem *system);
        gboolean (* can_restart_to_firmware_setup) (ScsmSystem *system);
        void     (* set_restart_to_firmware_setup) (ScsmSystem *system,
                                              gboolean   enable);
        gboolean (* can_suspend)      (ScsmSystem *system);
        gboolean (* can_hibernate)    (ScsmSystem *system);
        void     (* attempt_stop)     (ScsmSystem *system);
        void     (* attempt_restart)  (ScsmSystem *system);
        void     (* suspend)          (ScsmSystem *system);
        void     (* hibernate)        (ScsmSystem *system);
        void     (* set_session_idle) (ScsmSystem *system,
                                       gboolean   is_idle);
        gboolean (* is_login_session) (ScsmSystem *system);
        void     (* add_inhibitor)    (ScsmSystem        *system,
                                       const gchar      *id,
                                       ScsmInhibitorFlag  flags);
        void     (* remove_inhibitor) (ScsmSystem        *system,
                                       const gchar      *id);
        void     (* prepare_shutdown) (ScsmSystem   *system,
                                       gboolean     restart);
        void     (* complete_shutdown)(ScsmSystem   *system);
        gboolean (* is_last_session_for_user) (ScsmSystem *system);
};

enum _ScsmSystemError {
        SCSM_SYSTEM_ERROR_RESTARTING = 0,
        SCSM_SYSTEM_ERROR_STOPPING
};

GType      scsm_system_get_type         (void);

GQuark     scsm_system_error_quark      (void);

ScsmSystem *scsm_get_system              (void);

gboolean   scsm_system_can_switch_user  (ScsmSystem *system);

gboolean   scsm_system_can_stop         (ScsmSystem *system);

gboolean   scsm_system_can_restart      (ScsmSystem *system);

gboolean   scsm_system_can_restart_to_firmware_setup (ScsmSystem *system);

void       scsm_system_set_restart_to_firmware_setup (ScsmSystem *system,
                                                     gboolean   enable);

gboolean   scsm_system_can_suspend      (ScsmSystem *system);

gboolean   scsm_system_can_hibernate    (ScsmSystem *system);

void       scsm_system_attempt_stop     (ScsmSystem *system);

void       scsm_system_attempt_restart  (ScsmSystem *system);

void       scsm_system_suspend          (ScsmSystem *system);

void       scsm_system_hibernate        (ScsmSystem *system);

void       scsm_system_set_session_idle (ScsmSystem *system,
                                        gboolean   is_idle);

gboolean   scsm_system_is_login_session (ScsmSystem *system);

gboolean   scsm_system_is_last_session_for_user (ScsmSystem *system);

gboolean   scsm_system_is_active        (ScsmSystem *system);

void       scsm_system_add_inhibitor    (ScsmSystem        *system,
                                        const gchar      *id,
                                        ScsmInhibitorFlag  flags);

void       scsm_system_remove_inhibitor (ScsmSystem        *system,
                                        const gchar      *id);
void       scsm_system_prepare_shutdown  (ScsmSystem  *system,
                                         gboolean    restart);
void       scsm_system_complete_shutdown (ScsmSystem  *system);



G_END_DECLS

#endif /* __SCSM_SYSTEM_H__ */
