/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/time.h>
#include <gs/util/check.h>
#include <gs/util/bytebuffer.h>

uint32_t gs_time_diff_ms(uint32_t ref_ms, uint32_t now_ms)
{
    if (now_ms >= ref_ms) {
        return (now_ms - ref_ms);
    }

    // assuming time wrapped at max uint32_t
    return ((UINT32_MAX - ref_ms) + now_ms);
}

bool gs_time_sleep_until_ms(uint32_t * ref_ms, uint32_t sleep_ms)
{
    const uint32_t now = gs_time_rel_ms();
    *ref_ms += sleep_ms; // this is expected to be in the future
    uint32_t ms = gs_time_diff_ms(now, *ref_ms);
    if (ms > sleep_ms) {
        // we are behind - catch up, could be bad seed or too long processing
        *ref_ms = now;
        gs_time_sleep_ms(0); // yield - let others have a go
        return true;
    }
    gs_time_sleep_ms(ms);
    return false;
}

#if !(__AVR__)
gs_error_t gs_time_to_string(uint32_t seconds, char * buf, size_t buf_size)
{
    GS_CHECK_ARG((buf != NULL) && (buf_size > 0));

    gs_bytebuffer_t bb;
    gs_bytebuffer_init(&bb, buf, buf_size);

    const uint32_t days = (seconds / GS_TIME_DAYS_TO_SECS(1U));
    if (days) {
        seconds -= GS_TIME_DAYS_TO_SECS(days);
        gs_bytebuffer_printf(&bb, "%"PRIu32"d ", days);
    }

    const uint32_t hours = (seconds / GS_TIME_HOURS_TO_SECS(1U));
    if (days || hours) {
        seconds -= GS_TIME_HOURS_TO_SECS(hours);
        gs_bytebuffer_printf(&bb, "%"PRIu32"h ", hours);
    }
    
    const uint32_t mins = (seconds / GS_TIME_MINS_TO_SECS(1U));
    if (days || hours || mins) {
        seconds -= GS_TIME_MINS_TO_SECS(mins);
        gs_bytebuffer_printf(&bb, "%"PRIu32"m ", mins);
    }

    gs_bytebuffer_printf(&bb, "%"PRIu32"s", seconds);

    // force termination - ignore truncation
    buf[buf_size - 1] = 0;

    return GS_OK;
}
#endif
