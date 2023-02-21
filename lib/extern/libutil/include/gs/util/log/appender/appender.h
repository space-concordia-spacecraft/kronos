#ifndef GS_UTIL_LOG_APPENDER_APPENDER_H
#define GS_UTIL_LOG_APPENDER_APPENDER_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Log Appender interface.

   The log appender interface supports logging to different "stores".
   Logging is done through groups, which can be registered to different log appenders.
   Each log appender has it's own filter (level mask).
   Examples of log appenders could be: console, file, vmem, ...
*/

#include <gs/util/log/log.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
   Log appender (forward declaration)
   All log groups log to one or more appenders. The Log appender is responsible
   for putting the actual log data to a store/console or some other log medium.
*/
typedef struct gs_log_appender gs_log_appender_t;

/**
   Log appender record iterator callback function

   @param[in] ctx context data for iterator.
   @param[in] level log level of record being iterated
   @param[in] ts timestamp of record being iterated
   @param[in] group group string (zero terminated) of record being iterated
   @param[in] msg message string (zero terminated) of record being iterated
   @return true/false: Return false to discontinue iteration.
*/
typedef bool (*gs_log_record_iterator_t)(void *ctx, gs_log_level_t level, const gs_timestamp_t *ts, const char *group, const char *msg);

/**
   Log appender driver interface
*/
typedef struct {
    /** appender init function */
    gs_error_t (*init)(gs_log_appender_t *appender);
    /** appender function */
    void (*append)(gs_log_appender_t *appender, gs_log_level_t level, const gs_log_group_t *group, const gs_timestamp_t * ts, const char * format, va_list va);
    /** appender function for isr context */
    void (*append_isr)(gs_log_appender_t *appender, gs_log_level_t level, const gs_log_group_t *group, const gs_timestamp_t * ts, const char * format, va_list va);
    /** appender function for getting appender details string */
    gs_error_t (*info)(gs_log_appender_t *appender, char * info_str, uint8_t str_size);
    /** appender function for iterating stored appenders log history */
    gs_error_t (*hist)(gs_log_appender_t *appender, void * ctx, gs_log_record_iterator_t iter);
    /** appender function for clearing it's log history */
    gs_error_t (*clear)(gs_log_appender_t *appender);
    /** appender function for flushing cached log entries to it's store.
        This is only relevant for appenders implementing a log cache. */
    gs_error_t (*flush)(gs_log_appender_t *appender);
} gs_log_appender_driver_t;

/**
   Log appender
   All log groups log to one or more appenders. The Log appender is responsible
   for putting the actual log data to a store/console or some other log medium.
*/
struct gs_log_appender {
    /** Name of the appender */
    const char * name;
    /** appender driver interface */
    const gs_log_appender_driver_t * drv;
    /** appender driver configuration data */
    const void * drv_config;
    /** appender driver data - dynamic/internal data */
    void * drv_data;
    /** appender level mask */
    uint8_t mask;
};

/**
   Register an appender for the given log group.
   All logging, where the mask matches the groups \a level_mask, will be forwarded to this appender.

   @param[in] group_name Name of the group.
   @param[in] appender_name Name of appender to register for this group.
   @return gs_error_t
*/
gs_error_t gs_log_group_register_appender(const char * group_name, const char * appender_name);

/**
   Log appender iterator callback function

   @param[in] ctx context data for iterator.
   @param[in] appender log appender being iterated

   @return true/false: Return false to discontinue iteration.
*/
typedef bool (*gs_log_appender_iterator_t)(void *ctx, gs_log_appender_t * appender);

/**
   Iterate all or specific log appender(s).

   @param[in] name name of log appender, or NULL/\"all\" for all groups.
   @param[in] ctx user context data.
   @param[in] iter iterator, return \a true to continue, \a false to break iteration.
   @return_gs_error_t
*/
gs_error_t gs_log_appender_iterate(const char * name, void * ctx, gs_log_appender_iterator_t iter);

/**
   Iterate registered appenders for a specific group.

   @param[in] group log group to iterate appenders on.
   @param[in] ctx user context data.
   @param[in] iter appender iterator, return \a true to continue, \a false to break iteration.
   @return gs_error_t
*/
gs_error_t gs_log_group_appender_iterate(gs_log_group_t * group, void * ctx, gs_log_appender_iterator_t iter);

/**
   Register log appender.

   The log appender will be registered and initialized (if the appender has en init function, see #gs_log_appender_driver_t)

   The appender will not be attached to any log groups. For registering an appender to a group, use gs_log_group_register_appender()

   @param[in] appender appender - must stay in memory during the life-time of the application
   @return_gs_error_t
*/
gs_error_t gs_log_appender_register(gs_log_appender_t *appender);

/**
   Add log appender(s).

   The log appender will be registered and initialized (if the appender has en init function, see #gs_log_appender_driver_t)

   The appender will not be attached to any log groups. For registering an appender to a group, use gs_log_group_register_appender()

   @deprecated impossible to determine which appender fails, use gs_log_appender_register()
   @param[in] appenders array of appender(s) - must stay in memory during the life-time of the application
   @param[in] count array count - number of appenders.
   @return_gs_error_t
*/
gs_error_t gs_log_appender_add(gs_log_appender_t *appenders, uint16_t count);
    
/**
   Set log appender level mask.

   @param[in] appender_name log appender name
   @param[in] mask level mask to set.
   @return_gs_error_t
*/
gs_error_t gs_log_appender_set_level_mask(const char * appender_name, uint8_t mask);

/**
   Get log appender level mask.

   @param[in] appender_name log appender name
   @param[out] mask returned current level mask.
   @return_gs_error_t
*/
gs_error_t gs_log_appender_get_level_mask(const char * appender_name, uint8_t *mask);

/**
   Iterate log history for all or specific log appender.

   @param[in] name name of log appender, or NULL/\"all\" for all appenders.
   @param[in] ctx user context data for iterator.
   @param[in] iter iterator, return \a true to continue, \a false to break iteration.
   @return gs_error_t
*/
gs_error_t gs_log_appender_history_iterate(const char * name, void * ctx, gs_log_record_iterator_t iter);

/**
   Flush all log appenders data to storage.

   This will call the flush API (if implemented) for all log appenders
   available on the system. This should be called on regular basis from
   a system thread to ensure all cached data is correctly flushed to their
   stores.

   @return gs_error_t
*/
gs_error_t gs_log_appender_flush_all();

#ifdef __cplusplus
}
#endif
#endif
