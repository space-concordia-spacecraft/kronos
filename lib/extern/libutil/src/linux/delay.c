/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/delay.h>
#include <unistd.h>
#include <gs/util/time.h>

void gs_delay_us(uint32_t time_us)
{
    uint64_t ns = time_us;
    ns *= 1000LL;
    gs_time_sleep_ns(ns);
}

uint16_t gs_delay_ts_get(void)
{
    return 0;
}

void gs_delay_from_ts(uint16_t ts, uint16_t delay)
{

}
