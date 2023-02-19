/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/linux/rtc.h>

#include <time.h>
#include <linux/rtc.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>

static gs_error_t gs_rtc_get(void * driver_data, gs_timestamp_t * return_time)
{
    if (return_time == NULL) {
        return GS_ERROR_ARG;
    }
    
    return_time->tv_sec = 0;
    return_time->tv_nsec = 0;

    int fd = open("/dev/rtc", O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
        return gs_error(errno);
    }

    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    int res = ioctl(fd, RTC_RD_TIME, &tm);
    close(fd);
    if (res < 0) {
        return gs_error(errno);
    }

    time_t time = mktime(&tm);
    if (time < 0) {
        return GS_ERROR_DATA;
    }

    return_time->tv_sec = (uint32_t) time;

    return GS_OK;
}

static gs_error_t gs_rtc_set(void * driver_data, const gs_timestamp_t * set_time)
{
    if (set_time == NULL) {
        return GS_ERROR_ARG;
    }

    int fd = open("/dev/rtc", O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
        return gs_error(errno);
    }

    const time_t now = set_time->tv_sec;
    struct tm tm;
    gmtime_r(&now, &tm);
    int res = ioctl(fd, RTC_SET_TIME, &tm);
    close(fd);
    if (res < 0) {
        return gs_error(errno);
    }

    return GS_OK;
}

gs_error_t gs_rtc_register_linux(bool get, bool set)
{
    static gs_rtc_driver_t rtc_driver;
    if (get) {
        rtc_driver.get_time = gs_rtc_get;
    }
    if (set) {
        rtc_driver.set_time = gs_rtc_set;
    }
    return gs_rtc_register(&rtc_driver, NULL);
}
