#ifndef LIBGSCSP_INCLUDE_GS_CSP_ERROR_H
#define LIBGSCSP_INCLUDE_GS_CSP_ERROR_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
    @file

    Converting CSP error codes to #gs_error_t
*/

#include <csp/csp_error.h>
#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Convert CSP error.
   
   @param[in] csp_error CSP error
   @return GS error code representing the CSP error.
*/
gs_error_t gs_csp_error(int csp_error);

#ifdef __cplusplus
}
#endif
#endif
