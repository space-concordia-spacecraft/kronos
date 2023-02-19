#ifndef LIBGSCSP_INCLUDE_GS_CSP_LOG_H
#define LIBGSCSP_INCLUDE_GS_CSP_LOG_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   CSP log hook.
*/

#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Hook into CSP log system.
   All CSP logs will be logged through Log (libutil).

   @return_gs_error_t
*/
gs_error_t gs_csp_log_init(void);

#ifdef __cplusplus
}
#endif
#endif
