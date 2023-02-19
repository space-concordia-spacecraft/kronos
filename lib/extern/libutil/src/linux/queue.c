/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
 * Inspired by c-pthread-queue by Matthew Dickinson
 * http://code.google.com/p/c-pthread-queue/
 */

#include <gs/util/queue.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>

#define PTHREAD_QUEUE_ARG      GS_ERROR_ARG
#define PTHREAD_QUEUE_EMPTY    GS_ERROR_NOT_FOUND
#define PTHREAD_QUEUE_FULL     GS_ERROR_FULL
#define PTHREAD_QUEUE_TIMEOUT  GS_ERROR_TIMEOUT
#define PTHREAD_QUEUE_OK       GS_OK

typedef struct gs_pthread_queue {
    uint8_t * buffer;
    size_t size;
    size_t item_size;
    size_t items;
    size_t in;
    size_t out;
    pthread_mutex_t mutex;
    pthread_cond_t cond_full;
    pthread_cond_t cond_empty;
} pthread_queue_t;

static pthread_queue_t * pthread_queue_create(size_t length, size_t item_size)
{
    pthread_queue_t * q = malloc(sizeof(pthread_queue_t));
	
    if (q != NULL) {
        q->buffer = malloc(length*item_size);
        if (q->buffer != NULL) {
            q->size = length;
            q->item_size = item_size;
            q->items = 0;
            q->in = 0;
            q->out = 0;
            if (pthread_mutex_init(&(q->mutex), NULL) || pthread_cond_init(&(q->cond_full), NULL) || pthread_cond_init(&(q->cond_empty), NULL)) {
                free(q->buffer);
                free(q);
                q = NULL;
            }
        } else {
            free(q);
            q = NULL;
        }
    }

    return q;
}

static void pthread_queue_delete(pthread_queue_t * q)
{
    if (q) {
        free(q->buffer);
        free(q);
    }
}

static int pthread_queue_enqueue(pthread_queue_t * queue, const void * value, uint32_t timeout)
{
    /* Calculate timeout */
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts)) {
        return PTHREAD_QUEUE_ARG;
    }

    uint32_t sec = timeout / 1000;
    uint32_t nsec = (timeout - 1000 * sec) * 1000000;

    ts.tv_sec += sec;

    if (ts.tv_nsec + nsec > 1000000000)
        ts.tv_sec++;

    ts.tv_nsec = (ts.tv_nsec + nsec) % 1000000000;

    /* Get queue lock */
    pthread_mutex_lock(&(queue->mutex));

    while (queue->items == queue->size) {
        int ret = -1;
        if (timeout) {
            ret = pthread_cond_timedwait(&(queue->cond_full), &(queue->mutex), &ts);
        }
        if (ret) {
            pthread_mutex_unlock(&(queue->mutex));
            return PTHREAD_QUEUE_TIMEOUT;
        }
    }

    /* Coby object from input buffer */
    memcpy(queue->buffer+(queue->in * queue->item_size), value, queue->item_size);
    queue->items++;
    queue->in = (queue->in + 1) % queue->size;
    pthread_mutex_unlock(&(queue->mutex));

    /* Nofify blocked threads */
    pthread_cond_broadcast(&(queue->cond_empty));

    return PTHREAD_QUEUE_OK;
}

static int pthread_queue_dequeue(pthread_queue_t * queue, void * buf, uint32_t timeout)
{
    /* Calculate timeout */
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts)) {
        return PTHREAD_QUEUE_ARG;
    }
	
    uint32_t sec = timeout / 1000;
    uint32_t nsec = (timeout - 1000 * sec) * 1000000;

    ts.tv_sec += sec;
	
    if (ts.tv_nsec + nsec > 1000000000)
        ts.tv_sec++;

    ts.tv_nsec = (ts.tv_nsec + nsec) % 1000000000;
	
    /* Get queue lock */
    pthread_mutex_lock(&(queue->mutex));
    while (queue->items == 0) {
        int ret = -1;
        if (timeout) {
            ret = pthread_cond_timedwait(&(queue->cond_empty), &(queue->mutex), &ts);
        }
        if (ret) {
            pthread_mutex_unlock(&(queue->mutex));
            return PTHREAD_QUEUE_TIMEOUT;
        }
    }

    /* Coby object to output buffer */
    memcpy(buf, queue->buffer+(queue->out * queue->item_size), queue->item_size);
    queue->items--;
    queue->out = (queue->out + 1) % queue->size;
    pthread_mutex_unlock(&(queue->mutex));
	
    /* Nofify blocked threads */
    pthread_cond_broadcast(&(queue->cond_full));

    return PTHREAD_QUEUE_OK;
}

static size_t pthread_queue_items(pthread_queue_t * queue)
{
    pthread_mutex_lock(&(queue->mutex));
    size_t items = queue->items;
    pthread_mutex_unlock(&(queue->mutex));
    return items;
}

gs_error_t gs_queue_create(size_t items, size_t item_size, gs_queue_t * queue)
{
    if (queue == NULL) {
        return GS_ERROR_ARG;
    }
    pthread_queue_t * q = pthread_queue_create(items, item_size);
    if (q == NULL) {
        return GS_ERROR_ALLOC;
    }
    *queue = q;
    return GS_OK;
}

gs_error_t gs_queue_destroy(gs_queue_t queue)
{
    pthread_queue_delete(queue);
    return GS_OK;
}

gs_error_t gs_queue_enqueue(gs_queue_t queue, const void *value, int timeout_ms)
{
    return pthread_queue_enqueue(queue, value, (timeout_ms >= 0) ? timeout_ms : INT_MAX);
}

gs_error_t gs_queue_enqueue_isr(gs_queue_t queue, const void * value, gs_context_switch_t * cswitch)
{
    (void) cswitch;
    gs_error_t error = gs_queue_enqueue(queue, value, 0);
    return (error != GS_ERROR_TIMEOUT) ? error : GS_ERROR_FULL;
}

gs_error_t gs_queue_dequeue(gs_queue_t queue, int timeout_ms, void *buf)
{
    return pthread_queue_dequeue(queue, buf, (timeout_ms >= 0) ? timeout_ms : INT_MAX);
}

gs_error_t gs_queue_dequeue_isr(gs_queue_t queue, gs_context_switch_t * cswitch, void *buf)
{
    (void) cswitch;
    gs_error_t error = gs_queue_dequeue(queue, 0, buf);
    return (error != GS_ERROR_TIMEOUT) ? error : GS_ERROR_NOT_FOUND;
}

unsigned int gs_queue_size(gs_queue_t queue)
{
    if (queue) {
        return pthread_queue_items(queue);
    }
    return 0;
}

unsigned int gs_queue_size_isr(gs_queue_t queue)
{
    return gs_queue_size(queue);
}
