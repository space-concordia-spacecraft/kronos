/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/watchdog/watchdog.h>
#include <gs/util/watchdog/watchdog_task.h>
#include <gs/util/thread.h>
#include <gs/util/log.h>

typedef struct {
    gs_thread_t task;
    bool is_running;
    bool do_stop;
} gs_swwd_monitor_task_t;

// Return monitor task instance
gs_swwd_monitor_task_t * gs_swwd_get_monitor_task(void);

GS_LOG_GROUP_EXTERN(gs_swwd_log);
#define LOG_DEFAULT gs_swwd_log
