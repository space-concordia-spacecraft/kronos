/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/clock.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#if !__AVR__
#include <time.h>
#endif

gs_error_t gs_clock_to_iso8601_string2(uint32_t utc_sec, char * buf, size_t buf_size)
{
    if ((buf == NULL) || (buf_size == 0)) {
        return GS_ERROR_ARG;
    }

#if __AVR__
    int res = snprintf(buf, buf_size, "%"PRIu32"Z", utc_sec);
    if ((res < 0) || ((size_t)res >= buf_size)) {
        buf[buf_size - 1] = 0;
        return GS_ERROR_RANGE;
    }
#else
    const time_t time_seconds = (time_t) utc_sec;
    struct tm tm_buf;
    struct tm * tm = gmtime_r(&time_seconds, &tm_buf);
    if (tm == NULL) {
        int res = snprintf(buf, buf_size, "%ldZ", time_seconds);
        if ((res < 0) || ((size_t)res >= buf_size)) {
            buf[buf_size - 1] = 0;
        }
        return GS_ERROR_DATA;
    }

    // ISO8601 timestamp: 2017-03-30T06:20:45Z
    int res = snprintf(buf, buf_size, "%04d-%02d-%02dT%02d:%02d:%02dZ",
                       tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
                       tm->tm_hour, tm->tm_min, tm->tm_sec);
    if ((res < 0) || ((size_t)res >= buf_size)) {
        buf[buf_size - 1] = 0;
        return GS_ERROR_RANGE;
    }
#endif

    return GS_OK;
}

gs_error_t gs_clock_to_iso8601_string(const gs_timestamp_t * utc_time, char * buf, size_t buf_size)
{
    if (utc_time == NULL) {
        return GS_ERROR_ARG;
    }

    return gs_clock_to_iso8601_string2(utc_time->tv_sec, buf, buf_size);
}

gs_error_t gs_clock_from_string(const char * str, gs_timestamp_t * ts)
{
    if (!str || !str[0] || !ts) {
        return GS_ERROR_ARG;
    }

    // check for <seconds>.<nano-seconds>
    {
        uint32_t sec;
        uint32_t nsec;
        int res = sscanf(str, "%" SCNu32 ".%" SCNu32, &sec, &nsec);
        if (res == 2) {
            ts->tv_sec = sec;
            ts->tv_nsec = nsec;
            return GS_OK;
        }
    }

#if !__AVR__
    // check for ISO8601
    {
        struct tm tm;
        memset(&tm, 0, sizeof(tm)); // no daylight saving
        //int res = sscanf(str, "%" SCNd32 "-%" SCNd32 "-%" SCNd32 "T%" SCNd32 ":%" SCNd32 ":%" SCNd32 "Z",
        int res = sscanf(str, "%d-%d-%dT%d:%d:%dZ",
                         &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
                         &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
        if ((res == 6) &&
            (tm.tm_year >= 1970) && (tm.tm_year <= 2038) &&
            (tm.tm_mon >= 1) && (tm.tm_mon <= 12) &&
            (tm.tm_mday >= 1) && (tm.tm_mday <= 31) &&
            (tm.tm_hour >= 0) && (tm.tm_hour <= 23) &&
            (tm.tm_min >= 0) && (tm.tm_min <= 59) &&
            (tm.tm_sec >= 0) && (tm.tm_sec <= 60))
        {
            tm.tm_year -= 1900;
            tm.tm_mon -= 1;

#if __linux__
            // not posix compliant
            time_t sec = timegm(&tm);
#else
            // embedded platforms do not have timezones/daylight-saving - so standard mktime works
            time_t sec = mktime(&tm);
#endif
            if (sec >= 0) {
                ts->tv_sec = (uint32_t) sec;
                ts->tv_nsec = 0;
                return GS_OK;
            }
        }
    }
#endif

    return GS_ERROR_DATA;
}
