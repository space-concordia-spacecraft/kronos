#ifndef GS_UTIL_CLOCK_H
#define GS_UTIL_CLOCK_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Get/set time (including RTC), convert to/from string.
*/

#include <gs/util/error.h>
#include <gs/util/timestamp.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Returns real time/clock (UTC - time since Epoch/1970).

   If the platform supports a Real Time Clock, the RTC is normally read on first call. An offset is calculated for the relative clock, which
   then is used to calculate the actual time.

   @note clock_get_time() is proto-typed in libcsp as weak, but with different argument which MUST match gs_timestamp_t.
   @param[out] time user allocated buffer, contaning the current UTC time.
*/
void gs_clock_get_time(gs_timestamp_t * time);

/**
   Set real time/clock (UTC).
   If the platform supports a Real Time Clock, the RTC is also updated.
   @param[in] time UTC time.
   @return_gs_error_t
*/
gs_error_t gs_clock_set_time(const gs_timestamp_t * time);

/**
   Returns elapsed time since some unspecified starting point.
   @param[out] time user allocated buffer, receives elapsed time.
   @see gs_time_rel_ms()
*/
void gs_clock_get_monotonic(gs_timestamp_t * time);

/**
   Returns number of elapsed nano-seconds since some unspecified starting point.
   @return nano-seconds.
*/
uint64_t gs_clock_get_nsec(void);

/**
   Buffer length for containing full ISO8601 timestamp - including zero (0) termination.
*/
#define GS_CLOCK_ISO8601_BUFFER_LENGTH  21

/**
   Convert UTC to a ISO8601 string.
   ISO8601 timestamp: 2017-03-30T06:20:45Z
   @param[in] utc_time UTC time.
   @param[out] buffer user allocated buffer.
   @param[in] buffer_size size of \a buf.
   @return_gs_error_t
*/
gs_error_t gs_clock_to_iso8601_string(const gs_timestamp_t * utc_time, char * buffer, size_t buffer_size);

/**
   Convert UTC to a ISO8601 string.
   ISO8601 timestamp: 2017-03-30T06:20:45Z
   @param[in] utc_sec UTC seconds.
   @param[out] buffer user allocated buffer.
   @param[in] buffer_size size of \a buf.
   @return_gs_error_t
*/
gs_error_t gs_clock_to_iso8601_string2(uint32_t utc_sec, char * buffer, size_t buffer_size);

/**
   Convert string (UTC time) to timstamp.
   Parse string as:
   1. \<seconds\>.\<nano-seconds\> - number of seconds elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
   2. YYYY-MM-DDTHH:MM:SSZ - ISO8601
   @param[in] str time
   @param[out] ts time
   @return_gs_error_t
*/
gs_error_t gs_clock_from_string(const char * str, gs_timestamp_t * ts);

#ifdef __cplusplus
}
#endif
#endif
