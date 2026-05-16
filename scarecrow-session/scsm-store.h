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


#ifndef __SCSM_STORE_H
#define __SCSM_STORE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define SCSM_TYPE_STORE         (scsm_store_get_type ())
#define SCSM_STORE(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), SCSM_TYPE_STORE, ScsmStore))
#define SCSM_STORE_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), SCSM_TYPE_STORE, ScsmStoreClass))
#define SCSM_IS_STORE(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), SCSM_TYPE_STORE))
#define SCSM_IS_STORE_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), SCSM_TYPE_STORE))
#define SCSM_STORE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), SCSM_TYPE_STORE, ScsmStoreClass))

typedef struct ScsmStorePrivate ScsmStorePrivate;

typedef struct
{
        GObject          parent;
        ScsmStorePrivate *priv;
} ScsmStore;

typedef struct
{
        GObjectClass   parent_class;

        void          (* added)    (ScsmStore   *store,
                                    const char *id);
        void          (* removed)  (ScsmStore   *store,
                                    const char *id);
} ScsmStoreClass;

typedef enum
{
         SCSM_STORE_ERROR_GENERAL
} ScsmStoreError;

#define SCSM_STORE_ERROR scsm_store_error_quark ()

typedef gboolean (*ScsmStoreFunc) (const char *id,
                                  GObject    *object,
                                  gpointer    user_data);

GQuark              scsm_store_error_quark              (void);
GType               scsm_store_get_type                 (void);

ScsmStore *          scsm_store_new                      (void);

gboolean            scsm_store_get_locked               (ScsmStore    *store);
void                scsm_store_set_locked               (ScsmStore    *store,
                                                        gboolean     locked);

guint               scsm_store_size                     (ScsmStore    *store);
gboolean            scsm_store_add                      (ScsmStore    *store,
                                                        const char  *id,
                                                        GObject     *object);
void                scsm_store_clear                    (ScsmStore    *store);
gboolean            scsm_store_remove                   (ScsmStore    *store,
                                                        const char  *id);

void                scsm_store_foreach                  (ScsmStore    *store,
                                                        ScsmStoreFunc func,
                                                        gpointer     user_data);
guint               scsm_store_foreach_remove           (ScsmStore    *store,
                                                        ScsmStoreFunc func,
                                                        gpointer     user_data);
GObject *           scsm_store_find                     (ScsmStore    *store,
                                                        ScsmStoreFunc predicate,
                                                        gpointer     user_data);
GObject *           scsm_store_lookup                   (ScsmStore    *store,
                                                        const char  *id);


G_END_DECLS

#endif /* __SCSM_STORE_H */
