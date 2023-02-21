/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/time.h>
#include <time.h>
#include <errno.h>
#include <gs/util/timestamp.h>

uint32_t gs_time_rel_ms(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        return 0;
    }

    return (uint32_t)((ts.tv_sec * 1000) + (ts.tv_nsec/1000000));
}

uint32_t gs_time_rel_ms_isr(void)
{
    return gs_time_rel_ms();
}

static uint32_t uptime_offset = 0;
uint32_t gs_time_uptime(void)
{
    uint32_t seconds;
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        seconds = 0;
    } else {
        seconds = (uint32_t) ts.tv_sec;
    }
    if (uptime_offset == 0) {
        uptime_offset = seconds;
    }
    return (seconds - uptime_offset);
}

void gs_time_sleep_ns(uint64_t time_ns)
{
    struct timespec req, rem;
    req.tv_sec = (time_ns / GS_TIMESTAMP_NSEC_PER_SEC);
    req.tv_nsec = (time_ns % GS_TIMESTAMP_NSEC_PER_SEC);

    while ((nanosleep(&req, &rem) < 0) && (errno == EINTR)) {
        req = rem;
    }
}

void gs_time_sleep_ms(uint32_t time_ms)
{
    uint64_t ns = time_ms;
    ns *= 1000000LL;
    gs_time_sleep_ns(ns);
}
