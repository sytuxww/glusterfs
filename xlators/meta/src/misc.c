/*
   Copyright (c) 2006-2012 Red Hat, Inc. <http://www.redhat.com>
   This file is part of GlusterFS.

   This file is licensed to you under your choice of the GNU Lesser
   General Public License, version 3 or any later version (LGPLv3 or
   later), or the GNU General Public License, version 2 (GPLv2), in all
   cases as published by the Free Software Foundation.
*/
#include <unistd.h>
#include <sys/uio.h>

#ifndef _CONFIG_H
#define _CONFIG_H
#include "config.h"
#endif

#include "xlator.h"
#include "meta.h"

#define min(x,y)   ((x) < (y) ? (x) : (y))

/* /.meta/version */
static const char *version_str = PACKAGE_NAME " " PACKAGE_VERSION "\n";

int32_t
meta_version_readv (call_frame_t *frame, xlator_t *this,
		    dict_t *fd, size_t size, off_t offset)
{
  static int version_size;
  version_size = strlen (version_str);
  
  struct iovec vec;
  vec.iov_base = version_str + offset;
  vec.iov_len  = min (version_size - offset, size);

  STACK_UNWIND (frame, vec.iov_len, 0, &vec, 1);
  return 0;
}

int32_t
meta_version_getattr (call_frame_t *frame, 
			      xlator_t *this,
			      const char *path)
{
  meta_private_t *priv = (meta_private_t *) this->private;
  meta_dirent_t *root = priv->tree;
  meta_dirent_t *file = lookup_meta_entry (root, path, NULL);
  file->stbuf->st_size = strlen (version_str);
  STACK_UNWIND (frame, 0, 0, file->stbuf);
}

struct xlator_fops meta_version_fops = {
  .readv   = meta_version_readv,
  .getattr = meta_version_getattr
};

