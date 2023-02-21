/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/rtc.h>

static const gs_rtc_driver_t * rtc_driver;
static void * rtc_driver_data;

gs_error_t gs_rtc_register(const gs_rtc_driver_t * driver, void * driver_data)
{
    rtc_driver = driver;
    rtc_driver_data = driver_data;
    return GS_OK;
}

gs_error_t gs_rtc_supported(void)
{
    return rtc_driver ? GS_OK : GS_ERROR_NOT_SUPPORTED;
}

gs_error_t gs_rtc_get_time(gs_timestamp_t * time)
{
    if (time == NULL) {
        return GS_ERROR_ARG;
    }

    if (rtc_driver && rtc_driver->get_time) {
        return rtc_driver->get_time(rtc_driver_data, time);
    }
    return GS_ERROR_NOT_SUPPORTED;
}

gs_error_t gs_rtc_set_time(const gs_timestamp_t * time)
{
    if (time == NULL) {
        return GS_ERROR_ARG;
    }

    if (rtc_driver && rtc_driver->set_time) {
        return rtc_driver->set_time(rtc_driver_data, time);
    }
    return GS_ERROR_NOT_SUPPORTED;
}
