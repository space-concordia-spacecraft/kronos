#ifndef GS_UTIL_LOG_APPENDER_SIMPLE_FILE_H
#define GS_UTIL_LOG_APPENDER_SIMPLE_FILE_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file
   
   Simple log-file appender.
*/
#include <gs/util/log/appender/appender.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Simple File Log Appender driver configuration
*/
typedef struct gs_log_appender_simple_file_config {
    /**
       Name of file to create/write logs to
    */
    const char *filename;
    /**
       Truncate the file, when opening the log file.
    */
    bool truncate;
    /**
       Uee local time stamps when logging to log file, otherwise UTC.
    */
    bool use_local_time;
} gs_log_appender_simple_file_config_t;

/**
   Log appender for file.
*/
extern const gs_log_appender_driver_t gs_log_appender_simple_file_driver;

#ifdef __cplusplus
}
#endif
#endif
