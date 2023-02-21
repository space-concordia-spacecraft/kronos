#ifndef GS_UTIL_RTC_H
#define GS_UTIL_RTC_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Real Time Clock interface.

   The RTC driver is used by gs_clock_get_time() and gs_clock_set_time().
*/

#include <gs/util/error.h>
#include <gs/util/timestamp.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Platform supporting RTC must register the driver, before the rest of the system can access it.
   @see gs_rtc_register()
*/
typedef struct {
    /**
       Call-back for getting RTC time.
       @param[out] time user allocated struct for returning time.
    */
    gs_error_t (*get_time)(void * driver_data, gs_timestamp_t * time);
    /**
       Call-back for setting RTC time.
       @param[in] time user allocated struct for returning time.
    */
    gs_error_t (*set_time)(void * driver_data, const gs_timestamp_t * time);
} gs_rtc_driver_t;

/**
   Register RTC driver.
   @param[in] driver driver - data/struct must remain valid as long as registered.
   @param[in] driver_data driver specific data, forwarded to driver when set/get is called.
   @return_gs_error_t
*/
gs_error_t gs_rtc_register(const gs_rtc_driver_t * driver, void * driver_data);

/**
   Return GS_OK if RTC is supported.
*/
gs_error_t gs_rtc_supported(void);

/**
   Set RTC.
*/
gs_error_t gs_rtc_get_time(gs_timestamp_t * time);

/**
   Get RTC.
*/
gs_error_t gs_rtc_set_time(const gs_timestamp_t * time);

#ifdef __cplusplus
}
#endif
#endif
