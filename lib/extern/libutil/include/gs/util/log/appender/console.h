#ifndef GS_UTIL_LOG_APPENDER_CONSOLE_H
#define GS_UTIL_LOG_APPENDER_CONSOLE_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Console log appender - logs to stdout.
*/

#include <gs/util/log/appender/appender.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Log appender for console

   This log appender is the standard appender which is always available
   on any system. The appender should be registered to the root group,
   in order to get console/stdio logs.
*/
extern gs_log_appender_t gs_log_appender_console;

/**
   Log appender for console callback type

   This callback function can be used for registering a user defined logger function if
   the default can not be used for the given system.

   @param[in] appender pointer to the console appender.
   @param[in] level log level for log message
   @param[in] group log group for log message
   @param[in] ts timestamp for log message
   @param[in] format format of message in printf style
   @param[in] va variable argument list in printf style

   @return void
*/
typedef void (*gs_log_appender_console_cb_t)(gs_log_appender_t *appender, gs_log_level_t level, const gs_log_group_t *group, const gs_timestamp_t * ts, const char * format, va_list va);

/**
   Set Log appender for console callback

   When set, the given callback is called instead of the default console log function.
   To revert back to the default console log function, call this function with NULL as parameter.

   @param[in] cb callback to use for console logging.

   @return gs_error_t
*/
gs_error_t gs_log_appender_console_set_cb(gs_log_appender_console_cb_t cb);

#ifdef __cplusplus
}
#endif
#endif
