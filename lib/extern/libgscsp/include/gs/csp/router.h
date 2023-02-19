#ifndef LIBGSCSP_INCLUDE_GS_CSP_ROUTER_H
#define LIBGSCSP_INCLUDE_GS_CSP_ROUTER_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   CSP router task, with support for stopping/terminating router task.
*/

#include <gs/util/thread.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Start CSP router (task/thread).
   @warning THis is a warning
   @param[in] stack_size stack size in bytes, minimum 300 bytes.
   @param[in] priority task priority, normally GS_THREAD_PRIORITY_HIGH.
   @return_gs_error_t
*/
gs_error_t gs_csp_router_task_start(size_t stack_size, gs_thread_priority_t priority);

/**
   Stop CSP router task.

   Signal stop to the router and waits for it to terminate (join).
   @note Join is performed, which may hang forever if the router doesn't respond to the stop request.
   @param[in] timeout_ms timeout waiting for connections to close, 0 = skips connection check.
   @return_gs_error_t
*/
gs_error_t gs_csp_router_task_stop_with_timeout(uint32_t timeout_ms);

/**
   Stop CSP router task.

   Calls gs_csp_router_task_stop_with_timeout() with timeout of 30000mS.
   @return_gs_error_t
*/
gs_error_t gs_csp_router_task_stop(void);

#ifdef __cplusplus
}
#endif
#endif
