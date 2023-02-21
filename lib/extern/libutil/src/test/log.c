/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/test/log.h>
#include <gs/util/test/cmocka.h>
#include <gs/util/log/log.h>
#include <gs/util/log/appender/appender.h>
#include <gs/util/mutex.h>
#include <gs/util/bytebuffer.h>
#include <fnmatch.h>
#include <stdio.h>
#include <memory.h>
#include <fnmatch.h>

// cmocka
void cm_print_error(const char * const format, ...) CMOCKA_PRINTF_ATTRIBUTE(1, 2);

static gs_mutex_t g_lock;

#define GS_TEST_LOG_MAX_LEVELS       6
#define GS_TEST_LOG_MAX_LOG_MESSAGES 200

typedef struct {
    // Overall count per log level.
    unsigned int count[GS_TEST_LOG_MAX_LEVELS];
    // Log messages.
    struct {
        // Level.
        gs_log_level_t level;
        // Format log message.
        char msg[150];
    } logs[GS_TEST_LOG_MAX_LOG_MESSAGES];
    // Index of next entry in \a logs.
    unsigned int next_log;
} gs_test_log_stats_t;

static gs_test_log_stats_t gs_test_log_stats;

static void log_callback(gs_log_appender_t *appender, gs_log_level_t level, const gs_log_group_t * group, const gs_timestamp_t * ts, const char * format, va_list va)
{
    gs_mutex_lock(g_lock);
    gs_test_log_stats.count[level]++;

    {
        gs_bytebuffer_t bb;
        gs_bytebuffer_init(&bb,
                           gs_test_log_stats.logs[gs_test_log_stats.next_log].msg,
                           sizeof(gs_test_log_stats.logs[gs_test_log_stats.next_log].msg));

        va_list my_va;
        va_copy(my_va, va);
        gs_bytebuffer_printf(&bb, "%s: ", group->name);
        gs_bytebuffer_vprintf(&bb, format, my_va);
        va_end(my_va);
        gs_bytebuffer_get_as_string(&bb, NULL); // ensure NUL termination
        gs_test_log_stats.logs[gs_test_log_stats.next_log].level = level;

        ++gs_test_log_stats.next_log;
        if (gs_test_log_stats.next_log >= GS_TEST_LOG_MAX_LOG_MESSAGES) {
            gs_test_log_stats.next_log = 0;
        }
        gs_test_log_stats.logs[gs_test_log_stats.next_log].msg[0] = 0; // clear next entry
        gs_test_log_stats.logs[gs_test_log_stats.next_log].level = GS_TEST_LOG_MAX_LEVELS;
    }

    gs_mutex_unlock(g_lock);
}

static gs_log_appender_driver_t g_log_test_appender_driver = {
    .init = 0,
    .append = log_callback,
    .append_isr = log_callback,
};

static gs_log_appender_t g_log_test_appender = {
    .name = "test_appender",
    .drv = &g_log_test_appender_driver,
    .drv_config = NULL,
    .drv_data = NULL,
    .mask = LOG_ALL_MASK,
};

void gs_test_log_init(bool verbose)
{
    gs_log_init(true);

    if (g_lock == NULL) {
        GS_ASSERT_ERROR_EQUAL(gs_mutex_create(&g_lock), GS_OK);

        /* Add/Register appender - Only first time that init is called. */
        gs_log_appender_add(&g_log_test_appender, 1);
        gs_log_group_register_appender("root", "test_appender");
    }
    if (verbose) {
        gs_log_appender_set_level_mask("console", LOG_ALL_MASK);
    } else {
        gs_log_appender_set_level_mask("console", 0);
    }

    gs_test_log_clear();
}
    
void gs_test_log_clear(void)
{
    gs_mutex_lock(g_lock);
    memset(&gs_test_log_stats, 0, sizeof(gs_test_log_stats));
    gs_mutex_unlock(g_lock);
}

void gs_assert_log_count(int level, unsigned int count, const char * file, int line)
{
    if (level < 0) {
        unsigned int tot = 0;
        for (int i = 0; i < GS_TEST_LOG_MAX_LEVELS; ++i) {
            tot += gs_test_log_stats.count[i];
        }
        if (tot != count) {
            cm_print_error("Unexpected total log count: %u != %u\n", tot, count);
            _fail(file, line);
        }
    } else if (level >= GS_TEST_LOG_MAX_LEVELS) {
        cm_print_error("Unknown log level: %d - valid levels 0 - %d\n", level, GS_TEST_LOG_MAX_LEVELS - 1);
        _fail(file, line);
    } else if (gs_test_log_stats.count[level] != count) {
        cm_print_error("Unexpected log count: %u != %u\n", gs_test_log_stats.count[level], count);
        _fail(file, line);
    }
}

void gs_assert_log(unsigned int stack_index, unsigned int count, gs_log_level_t level, const char * pattern, const char * file, int line)
{
    if (stack_index == UINT32_MAX) {
        // loop through all logs
        unsigned int next = gs_test_log_stats.next_log;
        unsigned int hits = 0;
        for (unsigned int i = next - 1; i != next; --i) {
            if (i >= GS_TEST_LOG_MAX_LOG_MESSAGES) {
                i = (GS_TEST_LOG_MAX_LOG_MESSAGES - 1);
            }
            if ((gs_test_log_stats.logs[i].level == level) && (gs_test_log_stats.logs[i].msg[0])) {
                if (fnmatch(pattern, gs_test_log_stats.logs[i].msg, 0) == 0) {
                    ++hits;
                }
            }
        }
        if (hits != count) {
            cm_print_error("Unexpected log count: %u != %u\n", hits, count);
            _fail(file, line);
        }
    } else if (stack_index >= GS_TEST_LOG_MAX_LOG_MESSAGES) {
        cm_print_error("Invalid stack_index: %u - valid 0 - %d\n", stack_index, GS_TEST_LOG_MAX_LOG_MESSAGES - 1);
        _fail(file, line);
    } else {
        unsigned int i = (((gs_test_log_stats.next_log + GS_TEST_LOG_MAX_LOG_MESSAGES) - 1 - stack_index) % GS_TEST_LOG_MAX_LOG_MESSAGES);
        if ((gs_test_log_stats.logs[i].level == level) &&
            (gs_test_log_stats.logs[i].msg[0]) &&
            (fnmatch(pattern, gs_test_log_stats.logs[i].msg, 0) == 0)) {
            // match
        } else {
            cm_print_error("[%c][%s] != [%c][%s]\n",
                           gs_log_level_to_char(gs_test_log_stats.logs[i].level), gs_test_log_stats.logs[i].msg,
                           gs_log_level_to_char(level), pattern);
            _fail(file, line);
        }
    }
}
