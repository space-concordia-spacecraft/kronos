#ifndef GS_UTIL_QUEUE_H
#define GS_UTIL_QUEUE_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Queue.

   The queue API wraps FreeRTOS \a queue.
*/

#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

#if __linux__
/**
   Queue handle.
*/
typedef struct gs_pthread_queue * gs_queue_t;
#else
typedef struct gs_freertos_queue_t * gs_queue_t;
#endif

/**
   Create queue.

   @param[in] items max number of items on the queue.
   @param[in] item_size size of item (bytes).
   @param[out] queue created queue.
   @return_gs_error_t
*/
gs_error_t gs_queue_create(size_t items, size_t item_size, gs_queue_t * queue);

/**
   Destroy queue - free resources.

   @param[in] queue handle.
   @return_gs_error_t
*/
gs_error_t gs_queue_destroy(gs_queue_t queue);

/**
   Enqueue object on queue.
   @param[in] queue handle.
   @param[in] value pointer to object, size specified at gs_queue_create().
   @param_int_timeout_ms
   @return_gs_error_timeout
   @return_gs_error_t
*/
gs_error_t gs_queue_enqueue(gs_queue_t queue, const void *value, int timeout_ms);

/**
   Enqueue object on queue from within an ISR.
   @param[in] queue handle.
   @param[in] value pointer to object, size specified at gs_queue_create().
   @param[in] cswitch context switch.
   @return GS_ERROR_FULL if queue is full.
   @return_gs_error_t
*/
gs_error_t gs_queue_enqueue_isr(gs_queue_t queue, const void * value, gs_context_switch_t * cswitch);

/**
   Dequeue object from queue.
   @param[in] queue handle.
   @param[out] buf element - size specified in gs_queue_create().
   @param_int_timeout_ms
   @return_gs_error_timeout
   @return_gs_error_t
*/
gs_error_t gs_queue_dequeue(gs_queue_t queue, int timeout_ms, void *buf);

/**
   Dequeue object from queue from within an ISR.
   @param[in] queue handle.
   @param[in] cswitch context switch.
   @param[out] buf element - size specified in gs_queue_create().
   @return GS_ERROR_NOT_FOUND if no elements in queue.
   @return_gs_error_t
*/
gs_error_t gs_queue_dequeue_isr(gs_queue_t queue, gs_context_switch_t * cswitch, void * buf);

/**
   Return queue size.
   @param[in] queue handle.
   @return queue size
*/
unsigned int gs_queue_size(gs_queue_t queue);

/**
   Return queue size from within an ISR.
   @param[in] queue handle.
   @return queue size
*/
unsigned int gs_queue_size_isr(gs_queue_t queue);

#ifdef __cplusplus
}
#endif
#endif
