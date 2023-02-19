#ifndef GS_UTIL_TIME_H
#define GS_UTIL_TIME_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Releative time.
*/

#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Converts minutes to seconds.
*/
#define GS_TIME_MINS_TO_SECS(m)   (m * 60)

/**
   Converts hours to seconds.
*/
#define GS_TIME_HOURS_TO_SECS(h)  (h * GS_TIME_MINS_TO_SECS(60))

/**
   Converts days to seconds.
*/
#define GS_TIME_DAYS_TO_SECS(d)   (d * GS_TIME_HOURS_TO_SECS(24))

/**
   Return relative time (milli seconds).
   @note This will eventually wrap on all platforms - platform must wrap on 32 bit.
   @return relativ milli seconds
*/
uint32_t gs_time_rel_ms(void);

/**
   Return relative time (milli seconds).
   @note This will eventually wrap on all platforms - platform must wrap on 32 bit.
   @return relativ milli seconds
*/
uint32_t gs_time_rel_ms_isr(void);

/**
   Returns seconds since process started.
   @note On some platforms (e.g. Linux), first call will set offset and 
   first call it therefor not thread-safe.
   @return seconds since boot (or process startup).
*/
uint32_t gs_time_uptime(void);

/**
   Return time difference, compensating for time wrap due to 32 bit.
   @note the function can not detect multiple time wraps, so function using it should
   take action within 32 bit time.
   @param[in] ref_ms reference time.
   @param[in] now_ms current time.
   @returns ms difference, compensating for time wrapping (if now_ms is less than ref_ms).
*/
uint32_t gs_time_diff_ms(uint32_t ref_ms, uint32_t now_ms);

/**
   Sleep for X milli-seconds.
   No busy waiting.
   @note FreeRTOS: minimum sleep time depends on ticks per second. Suspends execution for minimum 1 tick - unless \a time is 0, in which case yield is called.
   @param[in] time_ms milli-seconds to sleep.
*/
void gs_time_sleep_ms(uint32_t time_ms);

/**
   Sleep X milli-seconds relative to reference.

   This sleep function uses a reference \a ref_ms to compensate for variance in processing time.

   No busy waiting.

   @param[in,out] ref_ms time reference.
   @param[in] sleep_ms how many milli-seconds to sleep - relative to reference.
   @return \a true if sleep time relative to last reference couldn't be done (reference reset), \a false if normal sleep was done.
*/
bool gs_time_sleep_until_ms(uint32_t * ref_ms, uint32_t sleep_ms);

/**
   Sleep for X nano-seconds.
   No busy waiting.
   @note FreeRTOS: minimum sleep time depends on ticks per second. Suspends execution for minimum 1 tick - unless \a time is 0, in which case yield is called.
   @param[in] time_ns nano-seconds to sleep.
*/
void gs_time_sleep_ns(uint64_t time_ns);

/**
   Convert elapsed time in seconds to string of days/hours/minutes/seconds.

   @param[in] seconds seconds.
   @param[out] buf user supplied buffer for output.
   @param[in] buf_size size of \a buf.
   @return_gs_error_t
*/
gs_error_t gs_time_to_string(uint32_t seconds, char * buf, size_t buf_size);

#ifdef __cplusplus
}
#endif
#endif
