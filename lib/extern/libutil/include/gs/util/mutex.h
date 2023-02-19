#ifndef GS_UTIL_MUTEX_H
#define GS_UTIL_MUTEX_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Mutex (recursive).

   The mutex API wraps POSIX \a pthread_mutex and FreeRTOS \a mutex.

   @note Mutex can not be used from within an ISR routine - use gs_sem instead.
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
   Mutex handle.
*/
typedef pthread_mutex_t * gs_mutex_t;
#else
typedef struct gs_freertos_mutex_t * gs_mutex_t;
#endif

/**
   Create mutex.
   @param[out] mutex handle.
   @return error code.
*/
gs_error_t gs_mutex_create(gs_mutex_t * mutex);

/**
   Destroy mutex - free resources.
   @param[in] mutex handle.
   @return error code.
*/
gs_error_t gs_mutex_destroy(gs_mutex_t mutex);

/**
   Lock mutex.
   @param[in] mutex handle.
   @return error code.
*/
gs_error_t gs_mutex_lock(gs_mutex_t mutex);

/**
   Unlock mutex.
   @param[in] mutex handle.
   @return error code.
*/
gs_error_t gs_mutex_unlock(gs_mutex_t mutex);

#ifdef __cplusplus
}
#endif
#endif
