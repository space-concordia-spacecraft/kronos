#ifndef GS_UTIL_LOG_LOG_H
#define GS_UTIL_LOG_LOG_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Log interface.

   Logging is done through groups (domains), where the level mask can be changed runtime.
*/

#include <gs/util/error.h>
#include <gs/util/timestamp.h>
#include <gs/util/pgm.h>
#include <inttypes.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Name of the root log group
*/
#define GS_LOG_GROUP_ROOT   "root"

/**
   Log levels.

   The levels can easily be mapped to standard syslog severity levels (https://en.wikipedia.org/wiki/Syslog).
*/
typedef enum {
    /**
       Trace (more detailed than \a debug).

       syslog: maps to \a debug (or \a trace if supported).
    */
    GS_LOG_TRACE = 0,
    /**
       Debug.

       syslog: maps to \a debug.
    */
    GS_LOG_DEBUG = 1,
    /**
       Informational.

       syslog: maps to \a informational.
    */
    GS_LOG_INFO = 2,
    /**
       Normal but significant conditions.

       syslog: maps to \a notice.
    */
    GS_LOG_NOTICE = 3,
    /**
       Warning.

       syslog: maps to \a warning.
    */
    GS_LOG_WARNING = 4,
    /**
       Error.

       syslog: maps to \a error.
    */
    GS_LOG_ERROR = 5,

    /**
       Trace (more detailed than \a debug).
       @deprecated use #GS_LOG_TRACE
    */
    LOG_TRACE = GS_LOG_TRACE,
    /**
       Debug.
       @deprecated use #GS_LOG_DEBUG
    */
    LOG_DEBUG = GS_LOG_DEBUG,
    /**
       Informational.
       @deprecated use #GS_LOG_INFO
    */
    LOG_INFO = GS_LOG_INFO,
    /**
       Normal but significant conditions.
       @deprecated use #GS_LOG_NOTICE
    */
    LOG_NOTICE = GS_LOG_NOTICE,
    /**
       Warning.
       @deprecated use #GS_LOG_WARNING
    */
    LOG_WARNING = GS_LOG_WARNING,
    /**
       Error.
       @deprecated use #GS_LOG_ERROR
    */
    LOG_ERROR = GS_LOG_ERROR,
} gs_log_level_t;

/**
   Log categories.

   The category is a way of grouping information about which sub-systems have logged. It is primarily used in the \a 
   telemetry table, to indicate what sub-systems have logged an \a error or \a warning - indicating a possible problem.

   Up to 32 categories are supported (stored in a uint32).

   Categories should be unique within a single node. However, nothing happens if categories clashes - it will only be more difficult to determine what part of the system logged.

   Standard categories are defined from #GS_LOG_CAT_1 and up. Products or mission specific software should start from #GS_LOG_CAT_32 and down.
*/
typedef enum {
    //! Standard, used for #GS_LOG_CAT_DEFAULT
    GS_LOG_CAT_1  = 1 << 0,
    //! Standard, used for #GS_LOG_CAT_DRIVER
    GS_LOG_CAT_2  = 1 << 1,
    //! Standard, used for #GS_LOG_CAT_CSP
    GS_LOG_CAT_3  = 1 << 2,
    //! Standard, used for #GS_LOG_CAT_PARAM
    GS_LOG_CAT_4  = 1 << 3,
    //! Standard, used for #GS_LOG_CAT_FILE_SYSTEM
    GS_LOG_CAT_5  = 1 << 4,
    //! Standard, used for #GS_LOG_CAT_COMMAND
    GS_LOG_CAT_6  = 1 << 5,
    //! Standard, used for #GS_LOG_CAT_HK
    GS_LOG_CAT_7  = 1 << 6,
    //! Standard, used for #GS_LOG_CAT_FP
    GS_LOG_CAT_8  = 1 << 7,
    //! Standard, used for #GS_LOG_CAT_ADCS
    GS_LOG_CAT_9  = 1 << 8,
    GS_LOG_CAT_10 = 1 << 9,
    GS_LOG_CAT_11 = 1 << 10,
    GS_LOG_CAT_12 = 1 << 11,
    GS_LOG_CAT_13 = 1 << 12,
    GS_LOG_CAT_14 = 1 << 13,
    GS_LOG_CAT_15 = 1 << 14,
    GS_LOG_CAT_16 = 1 << 15,
#if (__AVR__ == 0)
    GS_LOG_CAT_17 = 1 << 16,
    GS_LOG_CAT_18 = 1 << 17,
    GS_LOG_CAT_19 = 1 << 18,
    GS_LOG_CAT_20 = 1 << 19,
    GS_LOG_CAT_21 = 1 << 20,
    GS_LOG_CAT_22 = 1 << 21,
    GS_LOG_CAT_23 = 1 << 22,
    GS_LOG_CAT_24 = 1 << 23,
    GS_LOG_CAT_25 = 1 << 24,
    GS_LOG_CAT_26 = 1 << 25,
    GS_LOG_CAT_27 = 1 << 26,
    GS_LOG_CAT_28 = 1 << 27,
    GS_LOG_CAT_29 = 1 << 28,
    GS_LOG_CAT_30 = 1 << 29,
    GS_LOG_CAT_31 = 1 << 30,
    //! Product or mission specific - start here and down
    GS_LOG_CAT_32 = 1 << 31,
#endif
} gs_log_category_t;

/**
   @defgroup reserved_log_categories Reserved/assigned log categories.
   These categories are assigned/reserved for certain sub-systems.
   @{
*/
    /**
       Default, used if nothing else fits.
    */
#define GS_LOG_CAT_DEFAULT     GS_LOG_CAT_1
    /**
       Driver layer.
    */
#define GS_LOG_CAT_DRIVER      GS_LOG_CAT_2
    /**
       CSP.
    */
#define GS_LOG_CAT_CSP         GS_LOG_CAT_3
    /**
       Parameter system.
    */
#define GS_LOG_CAT_PARAM       GS_LOG_CAT_4
    /**
       File system.
    */
#define GS_LOG_CAT_FILE_SYSTEM GS_LOG_CAT_5
    /**
       Command framework and execution.
    */
#define GS_LOG_CAT_COMMAND     GS_LOG_CAT_6
    /**
       Housekeeping System.
    */
#define GS_LOG_CAT_HK          GS_LOG_CAT_7
    /**
       Flight Planner.
    */
#define GS_LOG_CAT_FP          GS_LOG_CAT_8
    /**
       ADCS
    */
#define GS_LOG_CAT_ADCS        GS_LOG_CAT_9
/** @} */

struct gs_log_list; /* forward declared private log list struct */
/**
   Log list type (private)

   Private gs_log_list type.
*/
typedef struct gs_log_list gs_log_list_t;

/**
   Log group.
   All logs are logged to a \a group. The group contains the current log level mask,
   which controls whether the log is carried through or not.
*/
typedef struct {
    /**
       Name of log group.
    */
    const char * name;
    /**
       Category, see #gs_log_category_t.
    */
    uint32_t category;
    /**
       Current level mask, see #gs_log_level_t.
    */
    uint8_t mask;
    /**
       Is group additive, if \a true (default) logging will be done on both root appenders and this groups appenders - if \a false, logging will only be done to this groups appenders.
    */
    bool additivity;
    /**
       Private list of appenders.
    */
    gs_log_list_t * appenders;
#if (__AVR__)
    uint16_t dummy_align;
#endif
} gs_log_group_t;

/**
   Log masks (levels converted to mask).
   @{
*/
/**
   Trace level enabled.
*/
#define GS_LOG_TRACE_MASK   (1 << GS_LOG_TRACE)
/**
   Debug level enabled.
*/
#define GS_LOG_DEBUG_MASK   (1 << GS_LOG_DEBUG)
/**
   Info level enabled.
*/
#define GS_LOG_INFO_MASK    (1 << GS_LOG_INFO)
/**
   Notice level enabled.
*/
#define GS_LOG_NOTICE_MASK  (1 << GS_LOG_NOTICE)
/**
   Warning level enabled.
*/
#define GS_LOG_WARNING_MASK (1 << GS_LOG_WARNING)
/**
   Error level enabled.
*/
#define GS_LOG_ERROR_MASK   (1 << GS_LOG_ERROR)
/**
   All levels enabled.
*/
#define GS_LOG_ALL_MASK     (GS_LOG_TRACE_MASK | GS_LOG_DEBUG_MASK | GS_LOG_INFO_MASK | GS_LOG_NOTICE_MASK | GS_LOG_WARNING_MASK | GS_LOG_ERROR_MASK)
/**
   Default levels enabled - #GS_LOG_ERROR, #GS_LOG_WARNING and #GS_LOG_NOTICE.
*/
#define GS_LOG_DEFAULT_MASK (GS_LOG_ERROR_MASK | GS_LOG_WARNING_MASK | GS_LOG_NOTICE_MASK)
/**
   Trace level enabled.
   @deprecated use #GS_LOG_TRACE_MASK
*/
#define LOG_TRACE_MASK      GS_LOG_TRACE_MASK
/**
   Debug level enabled.
   @deprecated use #GS_LOG_DEBUG_MASK
*/
#define LOG_DEBUG_MASK      GS_LOG_DEBUG_MASK
/**
   Info level enabled.
   @deprecated use #GS_LOG_INFO_MASK
*/
#define LOG_INFO_MASK       GS_LOG_INFO_MASK
/**
   Notice level enabled.
   @deprecated use #GS_LOG_NOTICE_MASK
*/
#define LOG_NOTICE_MASK     GS_LOG_NOTICE_MASK
/**
   Warning level enabled.
   @deprecated use #GS_LOG_WARNING_MASK
*/
#define LOG_WARNING_MASK    GS_LOG_WARNING_MASK
/**
   Error level enabled.
   @deprecated use #GS_LOG_ERROR_MASK
*/
#define LOG_ERROR_MASK      GS_LOG_ERROR_MASK
/**
   All levels enabled.
   @deprecated use #GS_LOG_ALL_MASK
*/
#define LOG_ALL_MASK        GS_LOG_ALL_MASK
/**
   Default levels enabled - #GS_LOG_ERROR, #GS_LOG_WARNING and #GS_LOG_NOTICE.
   @deprecated use #GS_LOG_DEFAULT_MASK
*/
#define LOG_DEFAULT_MASK    GS_LOG_DEFAULT_MASK
/**@}*/

/**
   Define/Create a log group.

   @note name clash: This defines a variable, which potentially is \a global, meaning possibility of name clashes. Therefore log group should always
   be prefixed with something that makes it fairly unique, i.e. component name. Example: gs_a3200dock_log - log group used by liba3200dock library.

   @param[in] group name of variables created. See note above about name clash.
   @param[in] name_in display name
   @param[in] cat_in log group category
   @param[in] level_mask log level mask.
*/
#define GS_LOG_GROUP(group, name_in, cat_in, level_mask)                        \
    gs_log_group_t group##_s = {.name = name_in, .category = cat_in,            \
                                .mask = level_mask, .additivity = true,         \
                                .appenders = NULL};                             \
    gs_log_group_t * group = &group##_s

/**
   Define log group with initial mask for \a print and \a store.

   @note name clash: This defines a variable, which potentially is \a global, meaning possibility of name clashes. Therefore log group should always
   be prefixed with something that makes it fairly unique, i.e. component name. Example: gs_a3200dock_log - log group used by liba3200dock library.

   @deprecated This MACRO is no longer supported, use #GS_LOG_GROUP(...) instead.

   @param[in] group name of variables created. See note above about name clash.
   @param[in] name_in display name
   @param[in] print_mask enable mask for \a print.
   @param[in] store_mask enable mask for \a store.
*/
#define LOG_GROUP_MASKED(group, name_in, print_mask, store_mask)   GS_LOG_GROUP(group, name_in, GS_LOG_CAT_DEFAULT, (print_mask | store_mask))

/**
   Declare log group as external (defined else where).

   @param[in] group the log group variable defined elsewhere.
*/
#define GS_LOG_GROUP_EXTERN(group)  extern gs_log_group_t * group

/**
   Define log group - levels are #GS_LOG_DEFAULT_MASK

   @deprecated This MACRO is no longer supported, use #GS_LOG_GROUP(..) instead.
*/
#define LOG_GROUP(group, name_in)         GS_LOG_GROUP(group, name_in, GS_LOG_CAT_DEFAULT, LOG_DEFAULT_MASK)

/**
   Define verbose log group - all levels are enabled (#GS_LOG_ALL_MASK)

   @deprecated This MACRO is no longer supported, use #GS_LOG_GROUP(..) instead.
*/
#define LOG_GROUP_VERBOSE(group, name_in) GS_LOG_GROUP(group, name_in, GS_LOG_CAT_DEFAULT, LOG_ALL_MASK)

/**
   Define silent log group - all levels are disabled.

   @deprecated This MACRO is no longer supported, use #GS_LOG_GROUP(..) instead.
*/
#define LOG_GROUP_SILENT(group, name_in)  GS_LOG_GROUP(group, name_in, GS_LOG_CAT_DEFAULT, 0)

/**
   Declare log group as external (defined else where).

   @deprecated use #GS_LOG_GROUP_EXTERN(...) instead.
*/
#define LOG_GROUP_EXTERN(group) GS_LOG_GROUP_EXTERN(group)

/**
   Default log group.
   This can be overridden by a define
*/
extern gs_log_group_t * LOG_DEFAULT;

/**
   Initializes the log system.

   @param[in] with_console_appender Enable/Disable console log appender
   @return_gs_error_t
*/
gs_error_t gs_log_init(bool with_console_appender);

/**
   Set log group level mask.

   @param[in] group_name log group name
   @param[in] mask level mask to set.
   @return_gs_error_t
*/
gs_error_t gs_log_group_set_level_mask(const char * group_name, uint8_t mask);

/**
   Set (multiple) log group/appender level mask.

   @version 3.7.2
   @param[in] name_level_mask name and level mask, format: \<name\>=\<level\>[,\<name\>=\<level\>]
   @param[in] name_only if set, only set group/appender matching the name
   @return_gs_error_t
   @see gs_log_set_level_mask_configuration()
*/
gs_error_t gs_log_set_level_mask(const char * name_level_mask, const char * name_only);

/**
   Get log group level mask.

   @param[in] group_name log group name
   @param[out] mask returned current level mask.
   @return_gs_error_t
*/
gs_error_t gs_log_group_get_level_mask(const char * group_name, uint8_t *mask);

/**
   Set initial group/appender level mask configuration.

   When a log group/appender is registered, gs_log_set_level_mask() is called with the configuration and the 
   name of the registered group/appender.

   @version 3.7.2
   @param[in] config level mask configuration. The string (pointer) must remain valid as long as it is set.
*/
void gs_log_set_level_mask_configuration(const char * config);

/**
   Log group iterator callback function

   @param[in] ctx context data for iterator.
   @param[in] group log group being iterated.

   @return true/false: Return false to discontinue iteration.
*/
typedef bool (*gs_log_group_iterator_t)(void *ctx, gs_log_group_t * group);

/**
   Iterate all or specific log group(s).

   @param[in] group_name name of log group, or NULL/\"all\" for all groups.
   @param[in] ctx user context data.
   @param[in] iter iterator, return \a true to continue, \a false to break iteration.
   @return_gs_error_t
*/
gs_error_t gs_log_group_iterate(const char * group_name, void * ctx, gs_log_group_iterator_t iter);

/**
   Register a log group in the log system.

   The log group will be added to a system wide list of log groups, enabling list and set of level.

   @note The group must remain valid during the life-time of the application.

   @param[in] group The log group to be added to the system.
   @return_gs_error_t
*/
gs_error_t gs_log_group_register(gs_log_group_t *group);

/**
   Register a log group in the log system.

   @note The group must stay in memory during the life-time of the application
   @see gs_log_group_register()
   @param[in] group The log group to be added to the system.
   @return_gs_error_t
*/
static inline gs_error_t gs_log_group_add(gs_log_group_t *group)
{
    return gs_log_group_register(group);
}
    
/**
   Checks if a level is enabled on a log group

   @param[in] group The log group to check.
   @param[in] level The log level to check if it's set on the group.
   @return bool (true if enabled / false if not enabled)
*/
bool gs_log_group_is_level_enabled(gs_log_group_t *group, gs_log_level_t level);

/**
   Convert string to log level.

   @param[in] str log level.
   @param[out] return_level converted log level.
   @return_gs_error_t
*/
gs_error_t gs_log_string_to_level(const char * str, gs_log_level_t * return_level);

/**
   Convert level to single character.

   @param[in] level log level
   @return single character representing the \a level.
*/
char gs_log_level_to_char(gs_log_level_t level);


/**
   Register Log commands.
   @return_gs_error_t
*/
gs_error_t gs_log_register_commands(void);

/**
   Generic log.
   @note This function should not be called directly, use log macros.

   @param level log level
   @param group log group. If NULL, the \a default log group will be used.
   @param format Format string (printf style).
*/
void gs_log(gs_log_level_t level, gs_log_group_t * group, const char * format, ...) __attribute__ ((format (__printf__, 3, 4)));

/**
   Generic log from ISR.
   @note This function should not be called directly, use log macros.

   @param level log level
   @param group log group. If NULL, the \a default log group will be used.
   @param format Format string (printf style).
*/
void gs_log_isr(gs_log_level_t level, gs_log_group_t * group, const char * format, ...) __attribute__ ((format (__printf__, 3, 4)));

/**
   Generic log (va_list).
   @note This function should not be called directly, use log macros.

   @param level log level
   @param group log group. If NULL, the \a default log group will be used.
   @param format Format string (printf style).
   @param args arguments for \a format.
*/
void gs_log_va(gs_log_level_t level, gs_log_group_t * group, const char * format, va_list args);

/**
   Enable/disable color in \a print logs.
   Default is \a enabled/true.

   @param[in] color \a true to enable color, \a false disable color.
*/
void gs_log_set_print_color(bool color);

/**
   Level to color (begin).

   @param[in] level log level.
   @return color string.
*/
const char * gs_log_level_to_color_begin(gs_log_level_t level);

/**
   Level to color (end).

   @return color string.
*/
const char * gs_log_level_to_color_end(void);

/**
   Take a level as input an create a level mask enabling all
   levels with priority >= level.

   If level is e.g. LOG_INFO, the mask will enable Error, Warn & Info.

   @param level the log level.
   @return level mask
*/
uint8_t gs_log_level_to_mask(gs_log_level_t level);

/**
   Convert string to log mask.

   Format: [+-]level[[+-]level]

   + add level, - remove level.

   @param[in] str log mask
   @param[in] current_mask current mask, used when input format contains + or -.
   @param[out] return_mask converted log mask.
   @return_gs_error_t
*/
gs_error_t gs_log_string_to_mask(const char *str, uint8_t current_mask, uint8_t * return_mask);

#if !(__DOXYGEN__)
/**
   Internal macro with check of log-level, before making log.
*/
#define __gs_log(level, group, format, ...)                             \
    if (group->mask & (1 << level)) {                                   \
        gs_log(level, group, GS_PGM_STR(format), ##__VA_ARGS__);        \
    }

/**
   Internal macro with check of log-level, before making log.
*/
#define __gs_log_isr(level, group, format, ...)                         \
    if (group->mask & (1 << level)) {                                   \
        gs_log_isr(level, group, GS_PGM_STR(format), ##__VA_ARGS__);    \
    }

/**
   Internal macro used for performing a log only once.
   @note This creates a \a static \a variable.
*/
#define __gs_log_once(level, group, format, ...)                        \
    ({                                                                  \
        static bool print_once;                                         \
        if (!print_once) {                                              \
            print_once = true;                                          \
            __gs_log(level, group, format, ##__VA_ARGS__);              \
        }                                                               \
    })
#endif // __DOXYGEN__
    
/**
   Default compile-time enabling/disabling of all levels
   Unless levels are individually defined, this will be the default value.
*/
#if !defined(GS_LOG_DISABLE_ALL)
#define GS_LOG_DISABLE_ALL     0
#endif

/**
   Disable \a error level compile-time by defining a value > 0
*/
#if !defined(GS_LOG_DISABLE_ERROR)
#define GS_LOG_DISABLE_ERROR   GS_LOG_DISABLE_ALL
#endif

/**
   Disable \a warning level compile-time by defining a value > 0
*/
#if !defined(GS_LOG_DISABLE_WARNING)
#define GS_LOG_DISABLE_WARNING GS_LOG_DISABLE_ALL
#endif

/**
   Disable \a notice level compile-time by defining a value > 0
*/
#if !defined(GS_LOG_DISABLE_NOTICE)
#define GS_LOG_DISABLE_NOTICE  GS_LOG_DISABLE_ALL
#endif

/**
   Disable \a info level compile-time by defining a value > 0
*/
#if !defined(GS_LOG_DISABLE_INFO)
#define GS_LOG_DISABLE_INFO    GS_LOG_DISABLE_ALL
#endif
    
/**
   Disable \a debug level compile-time by defining a value > 0
*/
#if !defined(GS_LOG_DISABLE_DEBUG)
#define GS_LOG_DISABLE_DEBUG   GS_LOG_DISABLE_ALL
#endif

/**
   Disable \a trace level compile-time by defining a value > 0
*/
#if !defined(GS_LOG_DISABLE_TRACE)
#define GS_LOG_DISABLE_TRACE   GS_LOG_DISABLE_ALL
#endif

/**
   Log \a error to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_error(format, ...)                       { if (!GS_LOG_DISABLE_ERROR) __gs_log(LOG_ERROR, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a error from ISR to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_error_isr(format, ...)                   { if (!GS_LOG_DISABLE_ERROR) __gs_log_isr(LOG_ERROR, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a error to group.
   @param[in] group log group (gs_log_group_t *).
   @param[in] format Format string (printf style).
*/
#define log_error_group(group, format, ...)          { if (!GS_LOG_DISABLE_ERROR) __gs_log(LOG_ERROR, (group), format, ##__VA_ARGS__); }

/**
   Log \a error only once to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_error_once(format, ...)                  { if (!GS_LOG_DISABLE_ERROR) __gs_log_once(LOG_ERROR, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a error only once to group.
   @param[in] group log group (gs_log_group_t *).
   @param[in] format Format string (printf style).
*/
#define log_error_once_group(group, format, ...)     { if (!GS_LOG_DISABLE_ERROR) __gs_log_once(LOG_ERROR, (group), format, ##__VA_ARGS__); }

/**
   Log \a warning to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_warning(format, ...)                     { if (!GS_LOG_DISABLE_WARNING) __gs_log(LOG_WARNING, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a warning from ISR to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_warning_isr(format, ...)                 { if (!GS_LOG_DISABLE_WARNING) __gs_log_isr(LOG_WARNING, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a warning to group.
   @param[in] group log group (gs_log_group_t *).
   @param[in] format Format string (printf style).
*/
#define log_warning_group(group, format, ...)        { if (!GS_LOG_DISABLE_WARNING) __gs_log(LOG_WARNING, (group), format, ##__VA_ARGS__); }

/**
   Log \a warning only once to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_warning_once(format, ...)                { if (!GS_LOG_DISABLE_WARNING) __gs_log_once(LOG_WARNING, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a warning only once to group.
   @param[in] group log group (gs_log_group_t *).
   @param[in] format Format string (printf style).
*/
#define log_warning_once_group(group, format, ...)   { if (!GS_LOG_DISABLE_WARNING) __gs_log_once(LOG_WARNING, (group), format, ##__VA_ARGS__); }

/**
   Log \a notice to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_notice(format, ...)                      { if (!GS_LOG_DISABLE_NOTICE) __gs_log(LOG_NOTICE, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a notice from ISR to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_notice_isr(format, ...)                  { if (!GS_LOG_DISABLE_NOTICE) __gs_log_isr(LOG_NOTICE, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a notice to group.
   @param[in] group log group (gs_log_group_t *).
   @param[in] format Format string (printf style).
*/
#define log_notice_group(group, format, ...)         { if (!GS_LOG_DISABLE_NOTICE) __gs_log(LOG_NOTICE, (group), format, ##__VA_ARGS__); }

/**
   Log \a notice only once to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_notice_once(format, ...)                 { if (!GS_LOG_DISABLE_NOTICE) __gs_log_once(LOG_NOTICE, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a notice only once to group.
   @param[in] group log group (gs_log_group_t *).
   @param[in] format Format string (printf style).
*/
#define log_notice_once_group(group, format, ...)    { if (!GS_LOG_DISABLE_NOTICE) __gs_log_once(LOG_NOTICE, (group), format, ##__VA_ARGS__); }
    
/**
   Log \a info to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_info(format, ...)                        { if (!GS_LOG_DISABLE_INFO) __gs_log(LOG_INFO, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a info from ISR to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_info_isr(format, ...)                    { if (!GS_LOG_DISABLE_INFO) __gs_log_isr(LOG_INFO, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a info to group.
   @param[in] group log group (gs_log_group_t *).
   @param[in] format Format string (printf style).
*/
#define log_info_group(group, format, ...)           { if (!GS_LOG_DISABLE_INFO) __gs_log(LOG_INFO, (group), format, ##__VA_ARGS__); }

/**
   Log \a info only once to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_info_once(format, ...)                   { if (!GS_LOG_DISABLE_INFO) __gs_log_once(LOG_INFO, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a info only once to group.
   @param[in] group log group (gs_log_group_t *).
   @param[in] format Format string (printf style).
*/
#define log_info_once_group(group, format, ...)      { if (!GS_LOG_DISABLE_INFO) __gs_log_once(LOG_INFO, (group), format, ##__VA_ARGS__); }

/**
   Log \a debug to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_debug(format, ...)                       { if (!GS_LOG_DISABLE_DEBUG) __gs_log(LOG_DEBUG, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a debug from ISR to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_debug_isr(format, ...)                   { if (!GS_LOG_DISABLE_DEBUG) __gs_log_isr(LOG_DEBUG, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a debug to group.
   @param[in] group log group (gs_log_group_t *).
   @param[in] format Format string (printf style).
*/
#define log_debug_group(group, format, ...)          { if (!GS_LOG_DISABLE_DEBUG) __gs_log(LOG_DEBUG, (group), format, ##__VA_ARGS__); }

/**
   Log \a debug only once to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_debug_once(format, ...)                  { if (!GS_LOG_DISABLE_DEBUG) __gs_log_once(LOG_DEBUG, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a debug only once to group.
   @param[in] group log group (gs_log_group_t *).
   @param[in] format Format string (printf style).
*/
#define log_debug_once_group(group, format, ...)     { if (!GS_LOG_DISABLE_DEBUG) __gs_log_once(LOG_DEBUG, (group), format, ##__VA_ARGS__); }

/**
   Log \a trace to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_trace(format, ...)                       { if (!GS_LOG_DISABLE_TRACE) __gs_log(LOG_TRACE, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a trace from ISR to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_trace_isr(format, ...)                   { if (!GS_LOG_DISABLE_TRACE) __gs_log_isr(LOG_TRACE, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a trace to group.
   @param[in] group log group (gs_log_group_t *).
   @param[in] format Format string (printf style).
*/
#define log_trace_group(group, format, ...)          { if (!GS_LOG_DISABLE_TRACE) __gs_log(LOG_TRACE, (group), format, ##__VA_ARGS__); }

/**
   Log \a trace only once to default group (LOG_DEFAULT).
   @param[in] format Format string (printf style).
*/
#define log_trace_once(format, ...)                  { if (!GS_LOG_DISABLE_TRACE) __gs_log_once(LOG_TRACE, LOG_DEFAULT, format, ##__VA_ARGS__); }

/**
   Log \a trace only once to group.
   @param[in] group log group (gs_log_group_t *).
   @param[in] format Format string (printf style).
*/
#define log_trace_once_group(group, format, ...)     { if (!GS_LOG_DISABLE_TRACE) __gs_log_once(LOG_TRACE, (group), format, ##__VA_ARGS__); }

#ifdef __cplusplus
}
#endif
#endif
