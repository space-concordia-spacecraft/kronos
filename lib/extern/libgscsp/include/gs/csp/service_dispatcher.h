#ifndef LIBGSCSP_INCLUDE_GS_CSP_SERVICE_DISPATCHER_H
#define LIBGSCSP_INCLUDE_GS_CSP_SERVICE_DISPATCHER_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   CSP Service Dispatcher.

   The dispatcher is a task/thread, listening on a configured number of ports and forwards the the incoming connection
   to a configured CSP service handler.
   The dispatcher touches a software watchdog for every handled connection.
*/

#include <gs/csp/service_handler.h>
#include <gs/csp/port.h>
#include <gs/util/thread.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   CSP service dispatcher configuration.
*/
typedef struct {
    /**
       Name of dispatcher.
    */
    const char * name;
    /**
       Array of service handlers - index'ed by CSP port number.
    */
    const gs_csp_service_handler_t * handler_array;
    /**
       Array size of \a handler_array.
    */
    unsigned int handler_array_size;
    /**
       Bind to any unbound ports (CSP_ANY).
       Only one task/dispatcher can bind to any ports. This can be used to log unexpected incoming connections.
    */
    bool bind_any;
    /**
       Disable watchdog.
       The watchdog is created with a fixed timeout of 20 seconds.
    */
    bool disable_watchdog;
    /**
       CSP connection backlog.
       If 0 (zero), the backlog is set to 10.
    */
    size_t listen_backlog;
    /**
       Callback after timeout or processsed connection.
       The return value is the next timeout in milli seconds.
       Return UINT32_MAX to use default timeout.
    */
    unsigned int (*callback)(void);
    /**
       Socket options.
    */
    uint32_t socket_options;
} gs_csp_service_dispatcher_conf_t;

/**
   Basic CSP service handlers.

   Can be used to configure handlers for all basic CSP services.
*/
#define GS_CSP_BASIC_SERVICE_HANDLERS                     \
    [GS_CSP_CMP] = gs_csp_cmp_service_handler,            \
    [GS_CSP_PING] = gs_csp_ping_service_handler,          \
    [GS_CSP_PS] = gs_csp_ps_service_handler,              \
    [GS_CSP_MEMFREE] = gs_csp_mem_free_service_handler,   \
    [GS_CSP_REBOOT] = gs_csp_reboot_service_handler,      \
    [GS_CSP_BUF_FREE] = gs_csp_buf_free_service_handler,  \
    [GS_CSP_UPTIME] = gs_csp_uptime_service_handler

/**
   Service dispatcher handle.
   @see gs_csp_service_dispatcher_create(), gs_csp_service_dispatcher_close()
*/
typedef struct gs_csp_service_dispatcher * gs_csp_service_dispatcher_t;

/**
   Create service dispatcher (task/thread).

   @param[in] conf configuration - must remain valid as long as the dispatcher is running.
   @param[in] stack_size thread stack size.
   @param[in] priority thread priority.
   @param[out] return_handle created dispatcher - use NULL if not used.
   @return_gs_error_t
*/
gs_error_t gs_csp_service_dispatcher_create(const gs_csp_service_dispatcher_conf_t * conf,
                                            size_t stack_size,
                                            gs_thread_priority_t priority,
                                            gs_csp_service_dispatcher_t * return_handle);

/**
   Wake up service dispatcher.

   This will cause the disapther to wake up (from listing for new connections), and invoke the configured \a callback function,
   before listing for new connections.

   @param[in] handle dispatcher.
   @return_gs_error_t
*/
gs_error_t gs_csp_service_dispatcher_wake_up(gs_csp_service_dispatcher_t handle);

/**
   Destroy/close service dispatcher (for testing).

   Signal stop to the dispatcher and waits for it to terminate (join).

   @note Join is performed, which may hang forever if the dispatcher doesn't respond to the stop request.
   @param[in] handle dispatcher.
   @return_gs_error_t
*/
gs_error_t gs_csp_service_dispatcher_destroy(gs_csp_service_dispatcher_t handle);

#ifdef __cplusplus
}
#endif
#endif
