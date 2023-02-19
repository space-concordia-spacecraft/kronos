/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/csp/router.h>
#include <gs/csp/csp.h>
#include <gs/csp/conn.h>
#include <csp/csp_autoconfig.h>
#include <gs/util/log.h>
#include <gs/util/check.h>
#include <gs/util/time.h>
#include "../lib/libcsp/src/csp_qfifo.h" // internal libcsp header -> FIFO_TIMEOUT, csp_qfifo_wake_up()
#include <../lib/libcsp/src/csp_conn.h>  // internal libcsp header
#include "local.h"

typedef struct {
    bool run;
    gs_thread_t thread;
} gs_csp_router_t;

static gs_csp_router_t gs_csp_router;

static void * gs_csp_router_task(void *param)
{
    /* Here there be routing */
    while (gs_csp_router.run) {
        csp_route_work(FIFO_TIMEOUT);
    }
    log_info("CSP router task terminating");
    gs_thread_exit(0);
}

gs_error_t gs_csp_router_task_stop_with_timeout(uint32_t timeout_ms)
{
    if ((gs_csp_router.run == false) || (gs_csp_router.thread == 0)) {
        // router not running
        return GS_OK;
    }

    // wait for RDP connections to close
    if (timeout_ms > 0) {
        unsigned int open = gs_csp_conn_get_open();
        if (open) {
            log_info("Waiting up to %"PRIu32" mS for %u connection(s) to timeout/close ...", timeout_ms, open);
            const uint32_t start_ms = gs_time_rel_ms();
            while (gs_csp_conn_get_open() && (gs_time_diff_ms(start_ms, gs_time_rel_ms()) < timeout_ms)) {
                gs_time_sleep_ms(200);
            }
        }
    }

    log_info("Waiting for CSP router task to stop ...");
    gs_csp_router.run = false;
    csp_qfifo_wake_up();
    gs_error_t error = gs_thread_join(gs_csp_router.thread, NULL);
    memset(&gs_csp_router, 0, sizeof(gs_csp_router));
    log_info("CSP router task stopped, error: %d", error);
    return error;
}

gs_error_t gs_csp_router_task_start(size_t stack_size, gs_thread_priority_t priority)
{
    if (gs_csp_router.run) {
        return GS_ERROR_IN_USE;
    }

    gs_csp_router.run = true;
    gs_error_t error = gs_thread_create("RTE", gs_csp_router_task, NULL, stack_size, priority,
                                        GS_THREAD_CREATE_JOINABLE, &gs_csp_router.thread);
    if (error) {
        memset(&gs_csp_router, 0, sizeof(gs_csp_router));
    }
    return error;
}

gs_error_t gs_csp_router_task_stop(void)
{
    return gs_csp_router_task_stop_with_timeout(30000);
}
