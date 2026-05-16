/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2007-2008 William Jon McCann <mccann@jhu.edu>
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


#ifndef __GSM_STORE_H
#define __GSM_STORE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define GSM_TYPE_STORE         (scsm_store_get_type ())
#define GSM_STORE(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GSM_TYPE_STORE, GsmStore))
#define GSM_STORE_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), GSM_TYPE_STORE, GsmStoreClass))
#define GSM_IS_STORE(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GSM_TYPE_STORE))
#define GSM_IS_STORE_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), GSM_TYPE_STORE))
#define GSM_STORE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GSM_TYPE_STORE, GsmStoreClass))

typedef struct GsmStorePrivate GsmStorePrivate;

typedef struct
{
        GObject          parent;
        GsmStorePrivate *priv;
} GsmStore;

typedef struct
{
        GObjectClass   parent_class;

        void          (* added)    (GsmStore   *store,
                                    const char *id);
        void          (* removed)  (GsmStore   *store,
                                    const char *id);
} GsmStoreClass;

typedef enum
{
         GSM_STORE_ERROR_GENERAL
} GsmStoreError;

#define GSM_STORE_ERROR scsm_store_error_quark ()

typedef gboolean (*GsmStoreFunc) (const char *id,
                                  GObject    *object,
                                  gpointer    user_data);

GQuark              scsm_store_error_quark              (void);
GType               scsm_store_get_type                 (void);

GsmStore *          scsm_store_new                      (void);

gboolean            scsm_store_get_locked               (GsmStore    *store);
void                scsm_store_set_locked               (GsmStore    *store,
                                                        gboolean     locked);

guint               scsm_store_size                     (GsmStore    *store);
gboolean            scsm_store_add                      (GsmStore    *store,
                                                        const char  *id,
                                                        GObject     *object);
void                scsm_store_clear                    (GsmStore    *store);
gboolean            scsm_store_remove                   (GsmStore    *store,
                                                        const char  *id);

void                scsm_store_foreach                  (GsmStore    *store,
                                                        GsmStoreFunc func,
                                                        gpointer     user_data);
guint               scsm_store_foreach_remove           (GsmStore    *store,
                                                        GsmStoreFunc func,
                                                        gpointer     user_data);
GObject *           scsm_store_find                     (GsmStore    *store,
                                                        GsmStoreFunc predicate,
                                                        gpointer     user_data);
GObject *           scsm_store_lookup                   (GsmStore    *store,
                                                        const char  *id);


G_END_DECLS

#endif /* __GSM_STORE_H */
