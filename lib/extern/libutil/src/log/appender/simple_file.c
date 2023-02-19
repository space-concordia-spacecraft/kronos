/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#ifndef __AVR__
#include <gs/util/log/appender/simple_file.h>
#include <gs/util/string.h>
#include <gs/util/mutex.h>

#include <stdlib.h>
#include <time.h>

typedef struct simple_file_drv_data {
    FILE *fp;
    gs_mutex_t mutex;
} simple_file_drv_data_t;

static gs_error_t gs_log_simple_file_init(gs_log_appender_t *appender)
{
    const gs_log_appender_simple_file_config_t *config = appender->drv_config;

    if (config == NULL || gs_string_empty(config->filename)) {
        return GS_ERROR_ARG;
    }

    simple_file_drv_data_t *drv_data = appender->drv_data;
    if (drv_data == NULL) {
        drv_data = calloc(1, sizeof(*drv_data));
        if (drv_data == NULL) {
            return GS_ERROR_ALLOC;
        }
    }

    /* If file is already open - Close it first */
    if (drv_data->fp) {
        gs_mutex_lock(drv_data->mutex);
        fclose(drv_data->fp);
        drv_data->fp = NULL;
        gs_mutex_unlock(drv_data->mutex);
        gs_mutex_destroy(drv_data->mutex);
    }

    const char * mode = config->truncate ? "w" : "a";

    drv_data->fp = fopen(config->filename, mode);
    if (drv_data->fp == NULL) {
        log_error("%s: failed to open log-file: [%s], mode: %s", __FUNCTION__, config->filename, mode);
        free(drv_data);
        drv_data = 0;
        return GS_ERROR_IO;
    }

    gs_mutex_create(&drv_data->mutex);
    appender->drv_data = drv_data;  /* Set driver data on appender */
    return GS_OK;
}

static void gs_log_simple_file_append(gs_log_appender_t *appender, gs_log_level_t level, const gs_log_group_t *group, const gs_timestamp_t * ts, const char * format, va_list va)
{
    va_list my_va;
    va_copy(my_va, va);

    const gs_log_appender_simple_file_config_t *config = appender->drv_config;
    simple_file_drv_data_t *drv_data = appender->drv_data;
    if (drv_data == 0) {
        va_end(my_va);
        return;
    }

    const char clevel = gs_log_level_to_char(level);

    const time_t t = ts->tv_sec;
    struct tm result;
    const char * tzone;
    if (config->use_local_time) {
        localtime_r(&t, &result);
        tzone = "";
    } else {
        gmtime_r(&t, &result);
        tzone = "Z";
    }

    if (drv_data->mutex) {
        gs_mutex_lock(drv_data->mutex);
    }
    {
        fprintf(drv_data->fp, "%04d-%02d-%02d %02d:%02d:%02d.%06"PRIu32"%s %c %s: ",
                result.tm_year + 1900, result.tm_mon + 1, result.tm_mday,
                result.tm_hour, result.tm_min, result.tm_sec,
                ts->tv_nsec / 1000, tzone, clevel, group->name);
        vfprintf(drv_data->fp, format, my_va);
        fprintf(drv_data->fp, "\r\n");
        fflush(drv_data->fp);
    }
    if (drv_data->mutex) {
        gs_mutex_unlock(drv_data->mutex);
    }

    va_end(my_va);
}

static gs_error_t gs_log_simple_file_append_info(gs_log_appender_t *appender, char *info_str, uint8_t str_size)
{
    if (!info_str) {
        return GS_ERROR_ARG;
    }

    const gs_log_appender_simple_file_config_t *config = appender->drv_config;
    snprintf(info_str, str_size, "Writes to file \"%s\"", config->filename);
    return GS_OK;
}

const gs_log_appender_driver_t gs_log_appender_simple_file_driver = {
    .init = gs_log_simple_file_init,
    .append = gs_log_simple_file_append,
    .append_isr = 0,
    .info = gs_log_simple_file_append_info,
};

#endif
