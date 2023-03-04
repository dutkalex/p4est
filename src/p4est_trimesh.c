/*
  This file is part of p4est.
  p4est is a C library to manage a collection (a forest) of multiple
  connected adaptive quadtrees or octrees in parallel.

  Copyright (C) 2010 The University of Texas System
  Additional copyright (C) 2011 individual authors
  Written by Carsten Burstedde, Lucas C. Wilcox, and Tobin Isaac

  p4est is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  p4est is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with p4est; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include <p4est_iterate.h>
#include <p4est_trimesh.h>

static void
iter_volume1 (p4est_iter_volume_info_t * vi, void *user_data)
{
}

static void
iter_face1 (p4est_iter_face_info_t * fi, void *user_data)
{
}

static void
iter_corner1 (p4est_iter_corner_info_t * ci, void *user_data)
{
}

p4est_trimesh_t    *
p4est_trimesh_new (p4est_t * p4est, p4est_ghost_t * ghost, int with_edge)
{
  P4EST_ASSERT (p4est_is_balanced (p4est, P4EST_CONNECT_FULL));

  p4est_trimesh_t    *tm;
  p4est_lnodes_t     *ln;

  tm = P4EST_ALLOC (p4est_trimesh_t, 1);
  memset (tm, 0, sizeof (p4est_trimesh_t));

  ln = tm->lnodes = P4EST_ALLOC (p4est_lnodes_t, 1);
  memset (ln, 0, sizeof (p4est_lnodes_t));
  ln->mpicomm = p4est->mpicomm;
  ln->sharers = sc_array_new (sizeof (p4est_lnodes_rank_t));

  p4est_iterate (p4est, ghost, NULL, iter_volume1, iter_face1, iter_corner1);

  return tm;
}

void
p4est_trimesh_destroy (p4est_trimesh_t * tm)
{
  P4EST_ASSERT (tm != NULL);
  P4EST_ASSERT (tm->lnodes != NULL);

  p4est_lnodes_destroy (tm->lnodes);
  P4EST_FREE (tm);
}
