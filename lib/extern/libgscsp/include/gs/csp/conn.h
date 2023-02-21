#ifndef LIBGSCSP_INCLUDE_GS_CSP_CONN_H
#define LIBGSCSP_INCLUDE_GS_CSP_CONN_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Extensions to standard libcsp connection interface.
*/

#include <gs/util/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Return number of open connections.

   @return number of open connections.
*/
unsigned int gs_csp_conn_get_open(void);

#ifdef __cplusplus
}
#endif
#endif
