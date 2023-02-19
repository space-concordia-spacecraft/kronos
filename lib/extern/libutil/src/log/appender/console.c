/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/log/appender/console.h>
#include <gs/util/mutex.h>
#include <stdio.h>
// generated by waf configure -> GS_LOG_ENABLE_ISR_LOGS
#include <conf_util.h>

static gs_log_appender_console_cb_t g_console_log_cb = NULL;
static gs_mutex_t g_log_console_mutex = NULL;

gs_error_t gs_log_console_append_init(gs_log_appender_t *appender)
{
    gs_error_t ret = GS_OK;
    if (g_log_console_mutex == NULL) {
        ret = gs_mutex_create(&g_log_console_mutex);
        if (ret != GS_OK) {
            g_log_console_mutex = NULL;
        }
    }
    return ret;
}

static void gs_log_console_append_common(gs_log_appender_t *appender, gs_log_level_t level, const gs_log_group_t *group, const gs_timestamp_t * ts, const char * format, va_list va, FILE * output_stream)
{
    va_list my_va;
    va_copy(my_va, va);

    const char * color = gs_log_level_to_color_begin(level);
    const char * end_color = gs_log_level_to_color_end();
    const char clevel = gs_log_level_to_char(level);

#if (GS_LOG_ENABLE_THREAD_ID && __linux__)
    fprintf(output_stream, "%s%04"PRIu32".%06"PRIu32" %c %s (%lu): ", color, ts->tv_sec, ts->tv_nsec / 1000, clevel, group->name, (unsigned long int) pthread_self());
#else
    fprintf(output_stream, "%s%04"PRIu32".%06"PRIu32" %c %s: ", color, ts->tv_sec, ts->tv_nsec / 1000, clevel, group->name);
#endif
    GS_PGM_VFPRINTF(output_stream, format, my_va);
    fprintf(output_stream, "%s\r\n", end_color);
    fflush(output_stream);

    va_end(my_va);
}

#if (GS_LOG_ENABLE_ISR_LOGS)
static void gs_log_console_append_isr(gs_log_appender_t *appender, gs_log_level_t level, const gs_log_group_t *group, const gs_timestamp_t * ts, const char * format, va_list va)
{
    /* Logs from ISR goes to stderr */
    gs_log_console_append_common(appender, level, group, ts, format, va, stderr);
}
#endif

static void gs_log_console_append(gs_log_appender_t *appender, gs_log_level_t level, const gs_log_group_t *group, const gs_timestamp_t * ts, const char * format, va_list va)
{
    if (g_console_log_cb)
        return g_console_log_cb(appender, level, group, ts, format, va);

    if (g_log_console_mutex) {
        gs_mutex_lock(g_log_console_mutex);
    }

    gs_log_console_append_common(appender, level, group, ts, format, va, stdout);

    if (g_log_console_mutex) {
        gs_mutex_unlock(g_log_console_mutex);
    }
}

static gs_error_t gs_log_console_append_get_info(gs_log_appender_t *appender, char *info_str, uint8_t str_size)
{
    if (!info_str) {
        return GS_ERROR_ARG;
    }

    snprintf(info_str, str_size, "Prints on stdout");
    return GS_OK;
}

gs_error_t gs_log_appender_console_set_cb(gs_log_appender_console_cb_t cb)
{
    g_console_log_cb = cb;
    return GS_OK;
}

static const gs_log_appender_driver_t console_appender_driver = {
    .init = gs_log_console_append_init,
    .append = gs_log_console_append,
#if (GS_LOG_ENABLE_ISR_LOGS)
    .append_isr = gs_log_console_append_isr,
#endif
    .info = gs_log_console_append_get_info,
};

gs_log_appender_t gs_log_appender_console = {
    .name = "console",
    .drv = &console_appender_driver,
    .drv_config = 0,
    .mask = LOG_ALL_MASK,
};
