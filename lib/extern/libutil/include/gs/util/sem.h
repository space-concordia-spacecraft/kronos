#ifndef GS_UTIL_SEM_H
#define GS_UTIL_SEM_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Semaphore.

   The semaphore API wraps POSIX \a semaphore and FreeRTOS \a counted semaphore.

   Main difference is that FreeRTOS uses different API calls, when called from within
   an ISR routine.
*/

#include <gs/util/error.h>
#if __linux__
#include <semaphore.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if __linux__
/**
   Semaphore handle.
*/
typedef sem_t * gs_sem_t;
#else
typedef struct gs_freertos_sem_t * gs_sem_t;
#endif

/**
   Create semaphore.
   @param[in] initialValue initial value.
   @param[out] sem created semaphore.
   @return_gs_error_t
*/
gs_error_t gs_sem_create(unsigned int initialValue, gs_sem_t * sem);

/**
   Destroy semaphore - free resources.
   @param[in] sem handle.
   @return_gs_error_t
*/
gs_error_t gs_sem_destroy(gs_sem_t sem);

/**
   Wait for semaphore to be signaled.
   @param[in] sem handle.
   @param_int_timeout_ms
   @return_gs_error_timeout
   @return_gs_error_t
*/
gs_error_t gs_sem_wait(gs_sem_t sem, int timeout_ms);

/**
   Post/signal semaphore.
   @param[in] sem handle.
   @return_gs_error_t
*/
gs_error_t gs_sem_post(gs_sem_t sem);

/**
   Post/signal semaphore from within a ISR.
   @param[in] sem handle.
   @param[in] cswitch context switch.
   @return_gs_error_t
*/
gs_error_t gs_sem_post_isr(gs_sem_t sem, gs_context_switch_t * cswitch);

#ifdef __cplusplus
}
#endif
#endif
