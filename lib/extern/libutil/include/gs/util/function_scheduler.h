#ifndef GS_UTIL_FUNCTION_SCHEDULER
#define GS_UTIL_FUNCTION_SCHEDULER
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Function scheduler.

   Simple framework for invoking functions at intervals.

   Instead of creating a lot of tasks (which uses memory), this framework can be used to schedule execution of functions at specified intervals.

   Once setup, calling gs_function_scheduler_execute() will execute all functions timed out and return the time, until the next function has
   to be executed or max timeout specified (or max wait time supported on the platform).

   The API supports multiple schedulers, but is not thread-safe.

   @note Do NOT use for time critical control, as the actual time interval is influenced by the host thread (priority) and other scheduled functions.
*/

#include <gs/util/error.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Scheduler handle.
*/
typedef struct gs_function_scheduler gs_function_scheduler_t;

/**
   Returned by scheduler function, to repeat same interval
*/
#define GS_FUNCTION_SCHEDULER_REPEAT_INTERVAL  UINT32_MAX
    
/**
   Function callback.

   @return timeout in mS until next callback or use #GS_FUNCTION_SCHEDULER_REPEAT_INTERVAL to repeat interval.
*/
typedef uint32_t (*gs_function_scheduler_function_t)(void * user_data);

/**
   Initialize scheduler.
   Memory is allocated once for \a max_entries.
   @param[in] max_timeout_ms max timeout in mS.
   @param[in] max_entries max number of entries for this scheduler.
   @param[out] scheduler reference to created scheduler.
   @return_gs_error_t
*/
gs_error_t gs_function_scheduler_create(uint32_t max_timeout_ms, unsigned int max_entries, gs_function_scheduler_t ** scheduler);

/**
   Free scheduler (release resources).
   @param[in] scheduler scheduler.
   @return_gs_error_t
*/
gs_error_t gs_function_scheduler_destroy(gs_function_scheduler_t * scheduler);

/**
   Execute scheduled function(s) and returns number of mS until next execute must be called again.

   @note Return type is \a int to prevent overflow on platforms where int is less than 32 bits.

   @param[in] scheduler scheduler.
   @return next timeout in mS.
*/
int gs_function_scheduler_execute(gs_function_scheduler_t * scheduler);

/**
   Execute scheduled function(s) and returns number of mS until next execute must be called again.

   @note Return type is \a int to prevent overflow on platforms where int is less than 32 bits.
   @deprecated use gs_function_scheduler_execute()
   @param[in] scheduler scheduler.
   @return next timeout in mS.
*/
static inline int gs_function_scheduler_execute_ms(gs_function_scheduler_t * scheduler) {
    return gs_function_scheduler_execute(scheduler);
}

/**
   Register function to be executed at mS intervals.
   @param[in] scheduler scheduler.
   @param[in] first_timeout_ms mS until first execution.
   @param[in] func function to execute.
   @param[in] user_data function user data.
   @return_gs_error_t
*/
gs_error_t gs_function_scheduler_register(gs_function_scheduler_t * scheduler, uint32_t first_timeout_ms, gs_function_scheduler_function_t func, void * user_data);

/**
   Register function to be executed at mS intervals.
   @deprecated use gs_function_scheduler_register()
   @param[in] scheduler scheduler.
   @param[in] first_timeout_ms mS until first execution.
   @param[in] func function to execute.
   @param[in] user_data function user data.
   @return_gs_error_t
*/
static inline gs_error_t gs_function_scheduler_register_ms(gs_function_scheduler_t * scheduler, uint32_t first_timeout_ms, gs_function_scheduler_function_t func, void * user_data) {
    return gs_function_scheduler_register(scheduler, first_timeout_ms, func, user_data);
}

/**
   Modify entry by name (register or remove) function to be executed at mS intervals.

   If \a interval_ms is 0 or \a func is NULL, the enrty will be removed.

   @param[in] scheduler scheduler.
   @param[in] name name.
   @param[in] interval_ms interval in mS. If 0, any entry will be removed.
   @param[in] func function to execute. If NULL, any entry will be removed.
   @param[in] user_data function user data.
   @return_gs_error_t
*/
gs_error_t gs_function_scheduler_modify_by_name(gs_function_scheduler_t * scheduler, const char * name, uint32_t interval_ms, gs_function_scheduler_function_t func, void * user_data);

#ifdef __cplusplus
}
#endif
#endif
