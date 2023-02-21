#ifndef GS_UTIL_THREAD_H
#define GS_UTIL_THREAD_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Thread/task API based on POSIX standard.

   The thread API wraps POSIX \a pthread and FreeRTOS \a task.
*/

#include <gs/util/error.h>
#if __linux__
#include <pthread.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if __linux__
/**
   Thread handle.
*/
typedef pthread_t gs_thread_t;
#else
typedef struct gs_freertos_task_t * gs_thread_t;
#endif

/**
   Type used to declare thread stack buffer for gs_thread_create_with_stack.
*/
typedef uint32_t gs_stack_type_t;

/**
   Thread priorities.
   These values are mapped to platform specific values.
*/
typedef enum {
  /**
     Idle (lowest) priority.
     Typical use: Not much - runs when nothing else runs.
     FreeRTOS: Idle thread.
  */
  GS_THREAD_PRIORITY_IDLE = 5,
  /**
     Low priority.
     Typical use: Service applications, e.g. servicing requests from the outside.
     GOMspace: housekeeping, GOSH.
  */
  GS_THREAD_PRIORITY_LOW = 10,
  /**
     Normal priority.
     Typical use: Control - the primary application(s).
  */
  GS_THREAD_PRIORITY_NORMAL = 15,
  /**
     High priority.
     Typical use: Drivers off loading data from hardware to software buffers.
     GOMspace: csp_route_task.
  */
  GS_THREAD_PRIORITY_HIGH = 20,
  /**
     High priority.
     Typical use: Very time critical threads. No long, time consuming processing.
     FreeRTOS: Timer thread.
  */
  GS_THREAD_PRIORITY_CRITICAL = 25,
} gs_thread_priority_t;

/**
   Thread function.
*/
typedef void * (*gs_thread_func_t)(void * parameter);

/**
   Create thread as joinable.
   @note only supported on linux. The thread must be joined to free all resources.
*/
#define GS_THREAD_CREATE_JOINABLE     0x0001

/**
   Create thread (or task on some platforms).

   pthread/Posix supports exit value and join, FreeRTOS supports neither (perhaps in the future), so API is designed after Posix.

   FreeRTOS: a thread must always terminate with a call to gs_thread_exit().
   linux: a thread is by default created detached, unless #GS_THREAD_CREATE_JOINABLE is specified.

   @param[in] name name of thread. Ignored on Linux.
   @param[in] func function for thread to execute.
   @param[in] parameter parameter parsed to the thread function.
   @param[in] stack_size number of bytes to allocate for stack - not used/supported on all platforms. Ignored on Linux.
   @param[in] priority thread priority. Ignored on Linux.
   @param[in] flags flags to control creation, see #GS_THREAD_CREATE_JOINABLE.
   @param[out] handle handle to the created thread, use NULL if not wanted.
   @return_gs_error_t
*/
gs_error_t gs_thread_create(const char * const name,
                            gs_thread_func_t func,
                            void * parameter,
                            size_t stack_size,
                            gs_thread_priority_t priority,
                            uint32_t flags,
                            gs_thread_t * handle);

/**
   Create thread (or task on some platforms) with user supplied buffer for stack.

   pthread/Posix supports exit value and join, FreeRTOS supports neither (perhaps in the future), so API is designed after Posix.

   FreeRTOS: a thread must always terminate with a call to gs_thread_exit().
   FreeRTOS v9.0 must be compiled with configSUPPORT_STATIC_ALLOCTION set to 1 - otherwise warning log is printed and user supplied
   stack buffer is discarded
   linux: a thread is by default created detached, unless #GS_THREAD_CREATE_JOINABLE is specified.
   stack_buf is ignored.

   @param[in] name name of thread. Ignored on Linux.
   @param[in] func function for thread to execute.
   @param[in] parameter parameter parsed to the thread function.
   @param[in] stack_size size of the user supplied stack buffer - not used/supported on all platforms. Ignored on Linux.
   @param[in] stack_buf User supplied stack buffer - not used/supported on all platforms. Ignored on Linux.
   @param[in] priority thread priority. Ignored on Linux.
   @param[in] flags flags to control creation, see #GS_THREAD_CREATE_JOINABLE.
   @param[out] handle handle to the created thread, use NULL if not wanted.
   @return_gs_error_t
*/
gs_error_t gs_thread_create_with_stack(const char * const name,
                                       gs_thread_func_t func,
                                       void * parameter,
                                       size_t stack_size,
                                       gs_stack_type_t *stack_buf,
                                       gs_thread_priority_t priority,
                                       uint32_t flags,
                                       gs_thread_t * handle);

/**
   Exit current thread.
   @param[in] exit_value exit value.
*/
void gs_thread_exit(void * exit_value) __attribute__ ((noreturn));

/**
   Sleep for X milli-seconds.
   @note FreeRTOS: minimum sleep time depends on ticks per milli-second. A thread is suspended minimum 1 tick - unless \a time_ms is 0, in which case yield is called.
   @deprecated use gs_time_sleep_ms()
   @param[in] time_ms milli-seconds to sleep.
*/
void gs_thread_sleep_ms(uint32_t time_ms);

/**
   Join with a terminated thread.

   @note Only supported on Linux and primarily used for testing.
   @note This is not based on pthread_cancel(), so the user must have signaled the thread to stop - otherwise this will hang forever.

   @param[in] thread handle.
   @param[out] return_retval return value from thread, use NULL if not wanted.
   @return_gs_error_t
*/
gs_error_t gs_thread_join(gs_thread_t thread, void ** return_retval);

/**
   Block thread forever.
   
   Primarily used in Linux applications main() to block main thread.
*/
void gs_thread_block(void) __attribute__ ((noreturn));

#ifdef __cplusplus
}
#endif
#endif
