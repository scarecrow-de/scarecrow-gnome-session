/* scsm-util.h
 * Copyright (C) 2008 Lucas Rocha.
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

#ifndef __GSM_UTIL_H__
#define __GSM_UTIL_H__

#include <glib.h>

G_BEGIN_DECLS

#define IS_STRING_EMPTY(x) ((x)==NULL||(x)[0]=='\0')

char *      scsm_util_find_desktop_file_for_app_name (const char *app_name,
                                                     gboolean    look_in_saved_session,
                                                     gboolean    autostart_first);

gchar      *scsm_util_get_empty_tmp_session_dir      (void);

const char *scsm_util_get_saved_session_dir          (void);

gchar**     scsm_util_get_app_dirs                   (void);

gchar**     scsm_util_get_autostart_dirs             (void);
void        scsm_util_set_autostart_dirs             (char **dirs);

gchar **    scsm_util_get_desktop_dirs               (gboolean include_saved_session,
                                                     gboolean autostart_first);

gboolean    scsm_util_text_is_blank                  (const char *str);

void        scsm_util_init_error                     (gboolean    fatal,
                                                     const char *format, ...) G_GNUC_PRINTF (2, 3);

char *      scsm_util_generate_startup_id            (void);

void        scsm_util_setenv                         (const char *variable,
                                                     const char *value);
const char * const * scsm_util_listenv               (void);
const char * const * scsm_util_get_variable_blacklist(void);

gboolean    scsm_util_export_activation_environment  (GError     **error);
#ifdef HAVE_SYSTEMD
gboolean    scsm_util_export_user_environment        (GError     **error);
gboolean    scsm_util_start_systemd_unit             (const char  *unit,
                                                     const char  *mode,
                                                     GError     **error);
gboolean    scsm_util_systemd_reset_failed           (GError     **error);

#endif

void        scsm_quit                                (void);

G_END_DECLS

#endif /* __GSM_UTIL_H__ */
