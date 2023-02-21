/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include "local.h"
#include <gs/util/watchdog/watchdog_task.h>
#include <gs/util/check.h>
#include <gs/util/time.h>

#define GS_SWWD_CHECK_INTERVAL_MS   1000    /* Check every 1 sec. */

static void * gs_swwd_monitor_task(void* parameter)
{
    gs_swwd_monitor_task_t * monitor = parameter;

    log_info("monitor task started");

    monitor->is_running = true;

    while(!monitor->do_stop) {

        gs_time_sleep_ms(GS_SWWD_CHECK_INTERVAL_MS);

        gs_swwd_check_expired_clients(NULL);
    }

    monitor->is_running = false;

    log_info("monitor task exiting");

    return NULL;
}

gs_error_t gs_swwd_monitor_task_start(void)
{
    gs_swwd_monitor_task_t * monitor = gs_swwd_get_monitor_task();
    GS_CHECK_SUPPORTED(monitor != NULL);          /* SWWD must be initialized */
    GS_CHECK_SUPPORTED(monitor->is_running == false); /* SWWD task must not already be running */

    /* Start the monitor task */
    gs_error_t error = gs_thread_create("SWWD", gs_swwd_monitor_task, monitor, 4000, GS_THREAD_PRIORITY_HIGH, 0, &monitor->task);
    if (error) {
        log_error("%s: gs_thread_create() failed, error: %s", __FUNCTION__, gs_error_string(error));
    }
    return error;
}

gs_error_t gs_swwd_monitor_task_stop(uint32_t timeout_s)
{
    gs_swwd_monitor_task_t * monitor = gs_swwd_get_monitor_task();
    GS_CHECK_SUPPORTED(monitor != NULL); /* SWWD must be initialized */

    /* Signal the task to stop */
    monitor->do_stop = true;

    /* Wait for the task to stop */
    const uint32_t timeout = GS_SWWD_CHECK_INTERVAL_MS + (timeout_s * 1000);
    uint32_t tm = 0;
    while(monitor->is_running && (tm < timeout)) {
        gs_thread_sleep_ms(100);
        tm += 100;
    }

    if (monitor->is_running) {
        return GS_ERROR_BUSY;
    }

    return GS_OK;
}

