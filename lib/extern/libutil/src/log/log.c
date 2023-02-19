/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include "local.h"
#include <gs/util/string.h>
#include <gs/util/clock.h>
#include <gs/util/mutex.h>
#include <gs/util/stdio.h>
#include <gs/util/error.h>
#include <gs/util/log/log.h>
#include <gs/util/log/appender/console.h>
#include <gs/util/check.h>
#include <gs/util/lock.h>
#include <stdlib.h>

#define MASK_SET    0
#define MASK_AND    1
#define MASK_OR     2

// use color in log print
static bool g_print_no_color;

// group level-mask configuration
static const char * g_level_mask_configuration;

// Log Group list
GS_STATIC gs_log_list_t g_log_groups = { .data = { .group = 0} };

// Log Appender list
GS_STATIC gs_log_list_t g_log_appenders = { .data = { .appender = 0} };

// Root Log Appenders - used for holding a appender list
GS_STATIC gs_log_group_t g_log_group_root = {.name = GS_LOG_GROUP_ROOT};

// Default log group - always present.
GS_LOG_GROUP(LOG_DEFAULT, "default", GS_LOG_CAT_DEFAULT, LOG_ERROR_MASK | LOG_WARNING_MASK | LOG_NOTICE_MASK | LOG_INFO_MASK);

/**
   All functions must call this initialization function, to ensure log is initialized.
*/
static gs_error_t gs_log_init_internal(void)
{
    if (g_log_groups.data.group == NULL) {
        return gs_log_init(true);
    }
    return GS_OK;
}

void gs_log_set_level_mask_configuration(const char * config)
{
    gs_log_init_internal();
    g_level_mask_configuration = config;
    gs_log_set_level_mask(g_level_mask_configuration, NULL);
}

bool gs_log_is_group_all(const char * name)
{
    return (name && ((strcasecmp(name, "*") == 0) || (strcasecmp(name, "all") == 0)));
}

static bool iter_set_level_mask(void * ctx_in, gs_log_group_t * group)
{
    uint8_t * level_mask = ctx_in;
    group->mask = *level_mask;
    return true;
}

gs_error_t gs_log_group_set_level_mask(const char * group_name, uint8_t mask)
{
    if (gs_string_empty(group_name)) {
        return GS_ERROR_HANDLE;
    }

    return gs_log_group_iterate(group_name, &mask, iter_set_level_mask);
}

gs_error_t gs_log_set_level_mask(const char * level_mask, const char * name_only)
{
    // format: <group|appender>=<level>[,<group|appender>=<level>]...

    for (const char * next = level_mask; next && *next;) {

        for (;*next == ','; ++next);

        if (*next == 0) {
            return GS_OK;
        }

        char entry[50];
        GS_STRNCPY(entry, next);

        char * name = entry;
        char * level = strchr(name, '=');
        if (level) {
            *level = 0;
            ++level;

            char * end = strchr(level, ',');
            if (end) {
                *end = 0;
            }

            gs_string_trim(name, sizeof(entry));
            gs_string_trim(level, sizeof(entry));

            if (strcasecmp(name, "color") == 0) {
                bool set;
                if (gs_string_to_bool(level, &set) == GS_OK) {
                    gs_log_set_print_color(set);
                }
            } else if (gs_string_empty(name_only) || gs_string_match(name, name_only)) {
                uint8_t mask = 0;
                if (gs_log_group_get_level_mask(name, &mask) == GS_OK) {
                    if (gs_log_string_to_mask(level, mask, &mask) == GS_OK) {
                        gs_log_group_set_level_mask(name, mask);
                    }
                } else if (gs_log_appender_get_level_mask(name, &mask) == GS_OK) {
                    if (gs_log_string_to_mask(level, mask, &mask) == GS_OK) {
                        gs_log_appender_set_level_mask(name, mask);
                    }
                }
            }
        }

        next = strchr(next, ',');
    }

    return GS_OK;
}

static bool iter_get_level_mask(void * ctx_in, gs_log_group_t * group)
{
    uint8_t * level_mask = ctx_in;
    *level_mask = group->mask;
    return true;
}

gs_error_t gs_log_group_get_level_mask(const char * group_name, uint8_t *mask)
{
    if (gs_string_empty(group_name)) {
        return GS_ERROR_HANDLE;
    }

    gs_error_t error = gs_log_group_iterate(group_name, mask, iter_get_level_mask);
    return error;
}

gs_error_t gs_log_string_to_level(const char * str, gs_log_level_t * return_level)
{
    if (gs_string_empty(str)) {
        return GS_ERROR_ARG;
    }

    const size_t len = strlen(str);
    gs_log_level_t level;

    if (strncasecmp(str, "trace", len) == 0) {
        level = LOG_TRACE;
    } else if (strncasecmp(str, "debug", len) == 0) {
        level = LOG_DEBUG;
    } else if (strncasecmp(str, "informational", len) == 0) {
        level = LOG_INFO;
    } else if (strncasecmp(str, "notice", len) == 0) {
        level = LOG_NOTICE;
    } else if (strncasecmp(str, "warning", len) == 0) {
        level = LOG_WARNING;
    } else if (strncasecmp(str, "error", len) == 0) {
        level = LOG_ERROR;
    } else {
        return GS_ERROR_DATA;
    }

    if (return_level) {
        *return_level = level;
    }

    return GS_OK;
}

char gs_log_level_to_char(gs_log_level_t level)
{
    switch (level) {
        case LOG_TRACE:   return 'T';
        case LOG_DEBUG:   return 'D';
        case LOG_INFO:    return 'I';
        case LOG_NOTICE:  return 'N';
        case LOG_WARNING: return 'W';
        case LOG_ERROR:   return 'E';
        default:          return '?';
    }
}

gs_error_t gs_log_string_to_mask(const char *str, uint8_t old, uint8_t * return_mask)
{
    GS_CHECK_ARG(gs_string_empty(str) == false);

    const char * next = str;
    while (*next) {

        for (;(*next == ','); ++next);

        // check for +xxx (add), -xxxx (remove), xxxx (set)
        int op = MASK_SET;
        if (*next == '+') {
            op = MASK_OR;
            ++next;
        } else if (*next == '-') {
            op = MASK_AND;
            ++next;
        }

        char token[30];
        GS_STRNCPY(token, next);
        unsigned int token_length = 0;
        for (; *next && (*next != ',') && (*next != '-') && (*next != '+'); ++next, ++token_length);
        if ((token_length < 1) || (token_length >= sizeof(token))) {
            return GS_ERROR_DATA;
        }
        token[token_length] = 0;

        /* Check mask */
        uint8_t mask;
        gs_log_level_t level;
        if (gs_log_string_to_level(token, &level) == GS_OK) {
            // actual level
            if (op == MASK_SET) {
                // set all level bits equal or lover
                mask = LOG_ALL_MASK & ~((1 << level) - 1);
            } else {
                mask = (1 << level);
            }
        } else if (!strncasecmp(token, "default", token_length)) { // legacy - conflicts with 'de(bug)'
            mask = LOG_DEFAULT_MASK;
            op = MASK_SET;
        } else if (!strncasecmp(token, "standard", token_length)) {
            mask = LOG_DEFAULT_MASK;
            op = MASK_SET;
        } else if (!strncasecmp(token, "all", token_length)) {
            mask = LOG_ALL_MASK;
            op = MASK_SET;
        } else if (!strncasecmp(token, "off", token_length)) {
            mask = 0;
            op = MASK_SET;
        } else if (!strncasecmp(token, "none", token_length)) { // legacy - conflicts with 'no(tice)'
            mask = 0;
            op = MASK_SET;
        } else if (gs_string_to_uint8(token, &mask) == GS_OK) {
            op = MASK_SET;
        } else {
            return GS_ERROR_DATA;
        }

        /* Apply operation */
        if (op == MASK_OR) {
            old |= mask;
        } else if (op == MASK_AND) {
            old &= ~mask;
        } else if (op == MASK_SET) {
            old = mask;
        }
    }

    if (return_mask) {
        *return_mask = old;
    }

    return GS_OK;
}

gs_error_t gs_log_group_iterate(const char * group_name, void * ctx, gs_log_group_iterator_t iter)
{
    const bool all = (gs_string_empty(group_name) || gs_log_is_group_all(group_name));
    bool found = false;

    for (gs_log_list_t *node = &g_log_groups; node; node = node->next) {
        if (node->data.group) {
            if (all || gs_string_match(group_name, node->data.group->name)) {
                found = true;
                bool cont = iter(ctx, node->data.group);
                if (cont == false) {
                    return GS_OK;
                }
            }
        }
    }

    return found ? GS_OK : GS_ERROR_NOT_FOUND;
}

static gs_error_t gs_log_group_register_internal(gs_log_group_t *group)
{
    // check if appender is already in the list and find last node
    gs_log_list_t * parent = &g_log_groups; // there will always be at least 1 group -> default
    for (; parent; parent = parent->next) {
        if ((parent->data.group == group) || (strcasecmp(group->name, parent->data.group->name) == 0)) {
            return GS_ERROR_EXIST;
        }
        if (parent->next == NULL) {
            break;
        }
    }

    gs_log_list_t * new_group_node = calloc(1, sizeof(*new_group_node));
    if (new_group_node == NULL) {
        return GS_ERROR_ALLOC;
    }

    new_group_node->data.group = group;

    // add to list - must be done last, iterating list can be done without locking
    parent->next = new_group_node;

    return GS_OK;
}

gs_error_t gs_log_group_register(gs_log_group_t *group)
{
    GS_CHECK_ARG(group != NULL);
    GS_CHECK_ARG(gs_string_empty(group->name) == false);

    gs_log_init_internal();

    gs_lock_lock();
    gs_error_t error = gs_log_group_register_internal(group);
    gs_lock_unlock();

    if (error == GS_OK) {
        gs_log_set_level_mask(g_level_mask_configuration, group->name);
    }

    return error;
}


bool gs_log_group_is_level_enabled(gs_log_group_t *group, gs_log_level_t level)
{
    return ((group->mask & level) > 0);
}

gs_error_t gs_log_appender_iterate(const char * name, void * ctx, gs_log_appender_iterator_t iter)
{
    const bool all = (gs_string_empty(name) || gs_log_is_group_all(name));
    bool found = false;

    /* Iterate the dynamically registered log appenders: */
    for (gs_log_list_t *node = &g_log_appenders; node; node = node->next) {
        if (node->data.appender) {
            if (all || gs_string_match(name, node->data.appender->name)) {
                found = true;
                bool cont = iter(ctx, node->data.appender);
                if (cont == false) {
                    return GS_OK;
                }
            }
        }
    }

    return found ? GS_OK : GS_ERROR_NOT_FOUND;
}

struct gs_log_history_ctx {
    gs_log_record_iterator_t iter;
    void *ctx;
};

static bool gs_log_history_iterator(void* ctx, gs_log_appender_t *appender)
{
    struct gs_log_history_ctx *hist_ctx = ctx;
    if (appender->drv->hist) {
        appender->drv->hist(appender, hist_ctx->ctx, hist_ctx->iter);
    }
    return true;
}

gs_error_t gs_log_appender_history_iterate(const char * name, void * ctx, gs_log_record_iterator_t iter)
{
    struct gs_log_history_ctx hist_ctx = {.iter=iter, .ctx = ctx};

    return gs_log_appender_iterate(name, &hist_ctx, gs_log_history_iterator);
}

static gs_error_t gs_log_appender_register_internal(gs_log_appender_t *appender)
{
    if (g_log_appenders.data.appender == NULL) {
        // first appender
        g_log_appenders.data.appender = appender;

        if (appender->drv->init) {
            gs_error_t error = appender->drv->init(appender);
            if (error) {
                g_log_appenders.data.appender = NULL;
                return error;
            }
        }

        return GS_OK;
    }

    // check if appender is already in the list and find last node
    gs_log_list_t * parent = &g_log_appenders;
    for (; parent; parent = parent->next) {
        if ((parent->data.appender == appender) || (strcasecmp(parent->data.appender->name, appender->name) == 0)) {
            return GS_ERROR_EXIST;
        }
        if (parent->next == NULL) {
            break;
        }
    }

    gs_log_list_t *new_appender = calloc(1, sizeof(*new_appender));
    if (new_appender == NULL) {
        return GS_ERROR_ALLOC;
    }

    new_appender->data.appender = appender;

    if (appender->drv->init) {
        gs_error_t error = appender->drv->init(appender);
        if (error) {
            free(new_appender);
            return error;
        }
    }

    // add to list - must be done last, iterating list can be done without locking
    parent->next = new_appender;

    return GS_OK;
}

gs_error_t gs_log_appender_register(gs_log_appender_t *appender)
{
    GS_CHECK_ARG(appender != NULL);
    GS_CHECK_ARG(gs_string_empty(appender->name) == false);
    GS_CHECK_ARG(appender->drv != NULL);
    GS_CHECK_ARG(appender->drv->append != NULL);

    gs_log_init_internal();

    gs_lock_lock();
    gs_error_t error = gs_log_appender_register_internal(appender);
    gs_lock_unlock();

    if (error == GS_OK) {
        gs_log_set_level_mask(g_level_mask_configuration, appender->name);
    }

    return error;
}

gs_error_t gs_log_appender_add(gs_log_appender_t *appender, uint16_t count)
{
    GS_CHECK_ARG(appender != NULL);
    GS_CHECK_ARG(count != 0);

    gs_error_t error = GS_OK;
    for (uint16_t i = 0; i < count; i++) {
        gs_error_t tmp_error = gs_log_appender_register(&appender[i]);
        if ((error == GS_OK) && tmp_error) {
            error = tmp_error;
        }
    }

    return error;
}

static bool iter_set_appender_level_mask(void * ctx_in, gs_log_appender_t * appender)
{
    uint8_t * level_mask = ctx_in;
    appender->mask = *level_mask;
    return true;
}

gs_error_t gs_log_appender_set_level_mask(const char * appender_name, uint8_t mask)
{
    if (gs_string_empty(appender_name)) {
        return GS_ERROR_HANDLE;
    }

    return gs_log_appender_iterate(appender_name, &mask, iter_set_appender_level_mask);
}

static bool iter_get_appender_level_mask(void * ctx_in, gs_log_appender_t * appender)
{
    uint8_t * level_mask = ctx_in;
    *level_mask = appender->mask;
    return true;
}

gs_error_t gs_log_appender_get_level_mask(const char * appender_name, uint8_t *mask)
{
    if (gs_string_empty(appender_name)) {
        return GS_ERROR_HANDLE;
    }

    return gs_log_appender_iterate(appender_name, mask, iter_get_appender_level_mask);
}

// Appender register/de-register iterator context
typedef struct {
    gs_log_appender_t *appender;
    gs_error_t ret;
} iter_group_appender_t;

static bool iter_log_appender_add(void *ctx, gs_log_group_t *group)
{
    iter_group_appender_t* in = ctx;

    gs_log_list_t * last_elem = group->appenders;
    for (gs_log_list_t *elem = group->appenders; elem; elem = elem->next) {
        last_elem = elem;
        if (elem->data.appender == in->appender) {
            in->ret = GS_ERROR_EXIST;
            return true;
        }
    }

    in->ret = GS_ERROR_ALLOC;
    gs_log_list_t *new_appender = calloc(1, sizeof(*new_appender));
    if (new_appender) {
        new_appender->data.appender = in->appender;
        new_appender->next = 0;
        if (last_elem != NULL) {
            last_elem->next = new_appender;
        } else {
            group->appenders = new_appender;
        }
        in->ret = GS_OK;
    }

    return true;
}

gs_error_t gs_log_group_register_appender(const char * group_name, const char * appender_name)
{
    gs_log_appender_t *appender = NULL;
    for (gs_log_list_t *elem = &g_log_appenders; elem; elem = elem->next) {
        if (elem->data.appender) {
            if (strcasecmp(elem->data.appender->name, appender_name) == 0) {
                appender = elem->data.appender;
                break;
            }
        }
    }
    if (NULL == appender) {
        return GS_ERROR_NOT_FOUND;
    }

    iter_group_appender_t ctx = {.appender = appender, .ret = GS_OK};

    gs_error_t ret = GS_OK;
    if (strcasecmp(group_name, GS_LOG_GROUP_ROOT) == 0) {
        iter_log_appender_add(&ctx, &g_log_group_root);
    } else {
        ret = gs_log_group_iterate(group_name, &ctx, iter_log_appender_add);
    }

    if (ret == GS_OK) {
        ret = ctx.ret;
    }

    return ret;
}

static bool iter_log_appender_remove(void *ctx, gs_log_group_t *group)
{
    iter_group_appender_t* in = ctx;
    in->ret = GS_ERROR_NOT_FOUND;

    gs_log_list_t * last_elem = group->appenders;
    for (gs_log_list_t *elem = group->appenders; elem; elem = elem->next) {
        if (elem->data.appender == in->appender) {
            if (elem == group->appenders) {
                group->appenders = elem->next;
            }
            last_elem->next = elem->next;
            free(elem);
            in->ret = GS_OK;
            break;
        }
        last_elem = elem;
    }

    return true;
}

gs_error_t gs_log_group_deregister_appender(const char * group_name, const char * appender_name)
{
    gs_log_appender_t *appender = NULL;
    for (gs_log_list_t *elem = &g_log_appenders; elem; elem = elem->next) {
        if (elem->data.appender) {
            if (strcasecmp(elem->data.appender->name, appender_name) == 0) {
                appender = elem->data.appender;
                break;
            }
        }
    }
    if (NULL == appender) {
        return GS_ERROR_NOT_FOUND;
    }

    iter_group_appender_t ctx = {.appender = appender, .ret = GS_OK};

    gs_error_t ret;
    if (strcasecmp(group_name, GS_LOG_GROUP_ROOT) == 0) {
        ret = iter_log_appender_remove(&ctx, &g_log_group_root);
    } else {
        ret = gs_log_group_iterate(group_name, &ctx, iter_log_appender_remove);
    }

    if (ret == GS_OK) {
        ret = ctx.ret;
    }

    return ret;
}

gs_error_t gs_log_group_appender_iterate(gs_log_group_t * group, void * ctx, gs_log_appender_iterator_t iter)
{
    GS_CHECK_ARG(group != NULL);

    bool found = false;
    for (gs_log_list_t *elem = group->appenders; elem; elem = elem->next) {
        found = true;
        iter(ctx, elem->data.appender);
    }

    /* Iterate root appenders */
    for (gs_log_list_t *elem = g_log_group_root.appenders; elem; elem = elem->next) {
        found = true;
        iter(ctx, elem->data.appender);
    }

    return found ? GS_OK : GS_ERROR_NOT_FOUND;
}

static inline void gs_log_process_appenders(const gs_log_list_t * it, gs_log_level_t level,
                                            const gs_log_group_t * group, gs_timestamp_t* ts, bool from_isr, const char * format, va_list va)
{
    for (; it; it = it->next) {
        gs_log_appender_t* appender = it->data.appender;

        if ((appender->mask & (1 << level)) == 0) {
            continue;
        }

        if (from_isr == false) {
            // log from none ISR context
            appender->drv->append(appender, level, group, ts, format, va);

        } else if (appender->drv->append_isr) {
            // log from ISR (Interrupt Service Routine) context
            appender->drv->append_isr(appender, level, group, ts, format, va);
        }
    }
}

static inline void gs_log_common_va(gs_log_level_t level, gs_log_group_t * group, bool from_isr, const char * format, va_list va)
{
    // get time as soon as possible
    gs_timestamp_t ts;
    if (from_isr) {
        // we need a ISR safe clock get
        ts.tv_sec = ts.tv_nsec = 0;
    } else {
        gs_clock_get_time(&ts);
    }

    // only needed if someone call function directly - otherwise the log macro has set it to a valid group
    if (group == NULL) {
        group = LOG_DEFAULT;
    }

    // check level mask for current group (this will nearly always be true, because the log macro has done the checking
    if (group->mask & (1 << level)) {

        // legacy - if log hasn't been initialized, this will initialize with console output enabled.
        gs_log_init_internal();

        if (group->appenders) {
            gs_log_process_appenders(group->appenders, level, group, &ts, from_isr, format, va);
        }

        if (group->additivity) {
            /* Call root appenders */
            gs_log_process_appenders(g_log_group_root.appenders, level, group, &ts, from_isr, format, va);
        }
    }
}

void gs_log(gs_log_level_t level, gs_log_group_t * group, const char * format, ...)
{
    va_list va_args;
    va_start(va_args, format);
    gs_log_common_va(level, group, false, format, va_args);
    va_end(va_args);
}

void gs_log_isr(gs_log_level_t level, gs_log_group_t * group, const char * format, ...)
{
    va_list va_args;
    va_start(va_args, format);
    gs_log_common_va(level, group, true, format, va_args);
    va_end(va_args);
}

void gs_log_va(gs_log_level_t level, gs_log_group_t * group, const char * format, va_list args)
{
    gs_log_common_va(level, group, false, format, args);
}

void gs_log_set_print_color(bool color)
{
    g_print_no_color = (color == false);
}

const char * gs_log_level_to_color_begin(gs_log_level_t level)
{
    if (g_print_no_color) {
        return "";
    }

    switch (level) {
        case LOG_ERROR:   return "\E[1;31m"; // Red
        case LOG_WARNING: return "\E[0;33m"; // Yellow
        case LOG_NOTICE:
        case LOG_INFO:    return "\E[0;32m"; // Green
        case LOG_DEBUG:   return "\E[0;34m"; // Blue
        default:
        case LOG_TRACE:   return "\E[0;35m"; // Magenta
    }
}

const char * gs_log_level_to_color_end(void)
{
    if (g_print_no_color) {
        return "";
    }

    return "\E[0m";
}

uint8_t gs_log_level_to_mask(gs_log_level_t level)
{
    /* Enable all levels with priority above the set level */
    uint8_t level_mask = (0xFF << level) & LOG_ALL_MASK;
    return level_mask;
}

static bool iter_flush_appender(void * ctx_in, gs_log_appender_t * appender)
{
    if (appender->drv->flush) {
        appender->drv->flush(appender);
    }
    return true;
}

gs_error_t gs_log_appender_flush_all()
{
    return gs_log_appender_iterate("", NULL, iter_flush_appender);
}

gs_error_t gs_log_init(bool with_console_appender)
{
    gs_error_t error = GS_OK;

    gs_lock_init(); // ignore result, this is the log system

    if (g_log_groups.data.group == NULL) {

        // default log group -> mark log as initialized
        g_log_groups.data.group = LOG_DEFAULT;

        // register console log appender
        if (with_console_appender) {
            error = gs_log_appender_register(&gs_log_appender_console);
            if (error == GS_OK) {
                error = gs_log_group_register_appender(GS_LOG_GROUP_ROOT, gs_log_appender_console.name);
            }
        }
    }

    return error;
}
