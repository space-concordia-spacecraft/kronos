/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/thread.h>
#include <gs/util/lock.h>
#include <unistd.h>
#include <gs/util/time.h>

gs_error_t gs_thread_create(const char * const name,
                            gs_thread_func_t func,
                            void * parameter,
                            size_t stack_size,
                            gs_thread_priority_t priority,
                            uint32_t flags,
                            gs_thread_t * return_handle)
{
    gs_time_uptime(); // force initialize of static offset
    gs_lock_init(); // initialize global lock.

    pthread_attr_t attr;
    int res = pthread_attr_init(&attr);
    if (res) {
        return GS_ERROR_ALLOC;
    }

    if (flags & GS_THREAD_CREATE_JOINABLE) {
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    } else {
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    }

    gs_thread_t handle;
    res = pthread_create(&handle, &attr, func, parameter);
    pthread_attr_destroy(&attr);
    if (res) {
        return GS_ERROR_ALLOC;
    }

    if (return_handle) {
        *return_handle = handle;
    }

    return GS_OK;
}

gs_error_t gs_thread_create_with_stack(const char * const name,
                                       gs_thread_func_t func,
                                       void * parameter,
                                       size_t stack_size,
                                       gs_stack_type_t *stack,
                                       gs_thread_priority_t priority,
                                       uint32_t flags,
                                       gs_thread_t * return_handle)
{
    return gs_thread_create(name, func, parameter, stack_size, priority, flags, return_handle);
}

void gs_thread_exit(void * exitValue)
{
    pthread_exit(exitValue);
}

void gs_thread_sleep_ms(uint32_t time_ms)
{
    gs_time_sleep_ms(time_ms);
}

gs_error_t gs_thread_join(gs_thread_t thread, void ** return_retval)
{
    gs_error_t error = GS_ERROR_ARG;
    void * retval = 0;
    if (thread) {
        int res = pthread_join(thread, &retval);
        if (res == 0) {
            error = GS_OK;
        } else {
            retval = 0;
        }
    }
    if (return_retval) {
        *return_retval = retval;
    }
    return error;
}

void gs_thread_block(void)
{
    /* Wait here forever */
    for (;;) {
        gs_time_sleep_ms(10000);
    }
}
