/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/sem.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>

gs_error_t gs_sem_create(unsigned int initialValue, gs_sem_t * sem)
{
    if (sem == NULL) {
        return GS_ERROR_ARG;
    }

    *sem = malloc(sizeof(sem_t));
    if (*sem == NULL) {
        return GS_ERROR_ALLOC;
    }

    int res = sem_init(*sem, 0, initialValue);
    if (res < 0) {
        res = gs_error(errno);
        free(*sem);
    }

    return res;
}

gs_error_t gs_sem_destroy(gs_sem_t sem)
{
    int res = GS_OK;
    if (sem) {
        res = sem_destroy(sem);
        if (res < 0) {
            res = gs_error(errno);
        }
        free(sem);
    }
    return res;
}

gs_error_t gs_sem_wait(gs_sem_t sem, int timeout_ms)
{
    int res;

    if (timeout_ms < 0) {
        res = sem_wait(sem);
    } else {
        struct timespec ts;
        res = clock_gettime(CLOCK_REALTIME, &ts);
        if (res == 0) {
            const uint32_t ms = (uint32_t)timeout_ms;
            uint32_t sec = ms / 1000;
            uint32_t nsec = (ms - (1000 * sec)) * 1000000;

            ts.tv_sec += sec;

            if (ts.tv_nsec + nsec >= 1000000000) {
                ts.tv_sec++;
            }

            ts.tv_nsec = (ts.tv_nsec + nsec) % 1000000000;

            res = sem_timedwait(sem, &ts);
        }
    }
    if (res < 0) {
        res = gs_error(errno);
    }
    return res;
}

gs_error_t gs_sem_post(gs_sem_t sem)
{
    int res = sem_post(sem);
    if (res < 0) {
        res = gs_error(errno);
    }
    return res;
}

gs_error_t gs_sem_post_isr(gs_sem_t sem, gs_context_switch_t * cswitch)
{
    (void) cswitch;
    int res = sem_post(sem);
    if (res < 0) {
        res = gs_error(errno);
    }
    return res;
}
