/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/timestamp.h>

int timestamp_add(gs_timestamp_t * base, const gs_timestamp_t * add)
{
    if (!base || !add)
        return -1;

    base->tv_sec += add->tv_sec;
    if (base->tv_nsec + add->tv_nsec >= GS_TIMESTAMP_NSEC_PER_SEC) {
        base->tv_sec++;
        base->tv_nsec = (base->tv_nsec + add->tv_nsec) % GS_TIMESTAMP_NSEC_PER_SEC;
    } else {
        base->tv_nsec += add->tv_nsec;
    }

    return 0;
}

int timestamp_diff(gs_timestamp_t * base, const gs_timestamp_t * diff)
{
    if (!base || !diff)
        return -1;

    base->tv_sec -= diff->tv_sec;
    if (base->tv_nsec >= diff->tv_nsec) {
        base->tv_nsec -= diff->tv_nsec;
    } else {
        base->tv_sec--;
        base->tv_nsec = (base->tv_nsec + GS_TIMESTAMP_NSEC_PER_SEC) - diff->tv_nsec;
    }

    return 0;
}

/* Test is timestamp is greater or equal */
int timestamp_ge(const gs_timestamp_t * base, const gs_timestamp_t * test)
{
    if (!base || !test)
        return -1;

    if (test->tv_sec > base->tv_sec ||
        (test->tv_sec == base->tv_sec &&
         test->tv_nsec > base->tv_nsec)) {
        return 1;
    }

    return 0;
}

int timestamp_copy(const gs_timestamp_t * from, gs_timestamp_t * to)
{
    if (!from || !to)
        return -1;

    to->tv_sec = from->tv_sec;
    to->tv_nsec = from->tv_nsec;

    return 0;
}
