#ifndef GS_CSP_RTABLE_H
#define GS_CSP_RTABLE_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   CSP routing table support.
*/

#include <gs/csp/csp.h>
#include <csp/csp_rtable.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Load routing table.
   Extension to csp_rtable_load().

   @param[in] rtable string containing the routing table to set/load.
   @param[in] set_default_route if \a true, sets a default route if no routing table specified and only one interface configured.
   @param[in] use_command_line_option if \a true (and command line supported), use command line options to configure routing.
   @return_gs_error_t
*/
gs_error_t gs_csp_rtable_load(const char * rtable, bool set_default_route, bool use_command_line_option);

#ifdef __cplusplus
}
#endif
#endif
