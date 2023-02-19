#ifndef LIBGSCSP_INCLUDE_GS_CSP_COMMAND_H
#define LIBGSCSP_INCLUDE_GS_CSP_COMMAND_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   CSP commands.
*/

#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Register CSP commands.
   @return_gs_error_t
*/
gs_error_t gs_csp_register_commands(void);

#ifdef __cplusplus
}
#endif
#endif
