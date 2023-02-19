/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/clock.h>
#include <gs/util/rtc.h>
#include <time.h>
#include <errno.h>

void gs_clock_get_time(gs_timestamp_t * time)
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);

    time->tv_sec = (uint32_t) now.tv_sec;
    time->tv_nsec = (uint32_t) now.tv_nsec;
}

gs_error_t gs_clock_set_time(const gs_timestamp_t * time)
{
    struct timespec now;
    now.tv_sec = time->tv_sec;
    now.tv_nsec = time->tv_nsec;

    int res = clock_settime(CLOCK_REALTIME, &now);
    if (res != 0) {
        return gs_error(errno);
    }

    gs_error_t error = GS_OK;
    if (gs_rtc_supported() == GS_OK) {
        error = gs_rtc_set_time(time);
    }

    return error;
}

void gs_clock_get_monotonic(gs_timestamp_t * time)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    time->tv_sec = (uint32_t) now.tv_sec;
    time->tv_nsec = (uint32_t) now.tv_nsec;
}

uint64_t gs_clock_get_nsec(void)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    return (((uint64_t)now.tv_sec) * GS_TIMESTAMP_NSEC_PER_SEC) + ((uint64_t)now.tv_nsec);
}

/**
   Required by libcsp.
   Proto-typed in ./libcsp/include/csp/arch/csp_clock.h, but with different argumet!

   __attribute__((weak)) extern void clock_get_time(csp_timestamp_t * time);
   __attribute__((weak)) extern void clock_set_time(csp_timestamp_t * time);
*/
void clock_get_time(gs_timestamp_t * time)
{
    gs_clock_get_time(time);
}

void clock_set_time(const gs_timestamp_t * time)
{
    gs_clock_set_time(time);
}
