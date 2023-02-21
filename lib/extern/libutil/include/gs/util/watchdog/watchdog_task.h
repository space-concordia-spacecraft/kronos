#ifndef GS_UTIL_WATCHDOG_WATCHDOG_TASK_H
#define GS_UTIL_WATCHDOG_WATCHDOG_TASK_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Software Watchdog server/task interface

   The Software Watchdog task implements the core (backend) functionality of the the software watchdog.
   The Client API for the SW watchdog is implemented in watchdog.h

   @note This API is not thread safe!
*/

#include <gs/util/watchdog/watchdog.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Start the Software Watchdog monitor task if the SWWD is to be used as a
   separate task (active mode).
   In this case the SWWD task will monitor expired clients in the background
   and the polling API gs_swwd_check_expired_clients() needs not to be called by
   the user.

   @return_gs_error_t
*/

gs_error_t gs_swwd_monitor_task_start();

/**
   Stops the Software Watchdog monitor task

   @param[in] timeout_s Maximum number of seconds to allow this operation to complete.

   @return_gs_error_t
*/
gs_error_t gs_swwd_monitor_task_stop(uint32_t timeout_s);

#ifdef __cplusplus
}
#endif
#endif
