#ifndef GS_UTIL_UNISTD_H
#define GS_UTIL_UNISTD_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   GomSpace extensions to standard \a unistd.h.
*/

#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Get current working directory.

   @note Linux uses standard getcwd().

   @param[out] buf user supplied buffer for returning path.
   @param[in] bufsize size of \a buf.
   @return #GS_ERROR_NOT_FOUND if no current directory is set.
   @return #GS_ERROR_RANGE if \a buf is too small
   @return_gs_error_t
*/
gs_error_t gs_getcwd(char * buf, size_t bufsize);

#ifdef __cplusplus
}
#endif
#endif
