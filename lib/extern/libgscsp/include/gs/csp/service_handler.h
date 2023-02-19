#ifndef LIBGSCSP_INCLUDE_GS_CSP_SERVICE_HANDLER_H
#define LIBGSCSP_INCLUDE_GS_CSP_SERVICE_HANDLER_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   CSP Service handler.
*/

#include <csp/csp.h>
#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   CSP Service handler.

   It is the handler's responsibility to process all pakcets on the connection and close the connection when done - even if a failure
   is returned.

   @param[in] conn incoming connection.
   @return_gs_error_t
*/
typedef gs_error_t (*gs_csp_service_handler_t)(csp_conn_t * conn);

/**
   Service handler for CSP Management Protocol.

   Invokes standard libcsp service handler.

   @param[in] conn incoming connection.
   @return_gs_error_t
*/
gs_error_t gs_csp_cmp_service_handler(csp_conn_t * conn);

/**
   Service handler for ping.

   Invokes standard libcsp service handler.

   @param[in] conn incoming connection.
   @return_gs_error_t
*/
gs_error_t gs_csp_ping_service_handler(csp_conn_t * conn);

/**
   Service handler for getting process list.

   Invokes standard libcsp service handler.

   @param[in] conn incoming connection.
   @return_gs_error_t
*/
gs_error_t gs_csp_ps_service_handler(csp_conn_t * conn);

/**
   Service handler for getting memory free.

   Invokes GomSpace handler, which doesn't use malloc() to determine \a free memory.

   @param[in] conn incoming connection.
   @return_gs_error_t
*/
gs_error_t gs_csp_mem_free_service_handler(csp_conn_t * conn);


/**
   Service handler for reboot (reset) of node.

   Invokes standard libcsp service handler.

   @param[in] conn incoming connection.
   @return_gs_error_t
*/
gs_error_t gs_csp_reboot_service_handler(csp_conn_t * conn);

/**
   Service handler for getting free CSP buffers.

   Invokes standard libcsp service handler.

   @param[in] conn incoming connection.
   @return_gs_error_t
*/
gs_error_t gs_csp_buf_free_service_handler(csp_conn_t * conn);

/**
   Service handler for getting uptime (in seconds).

   Invokes GomSpace handler (works on Linux).

   @param[in] conn incoming connection.
   @return_gs_error_t
*/
gs_error_t gs_csp_uptime_service_handler(csp_conn_t * conn);

#ifdef __cplusplus
}
#endif
#endif
