/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include "function_scheduler_local.h"

#include <gs/util/check.h>
#include <gs/util/time.h>
#include <gs/util/minmax.h>
#include <gs/util/check.h>
#include <gs/util/string.h>
#include <stdlib.h>

GS_CHECK_STATIC_ASSERT(sizeof(int) >= 2, int_must_be_at_least_16bit);

gs_error_t gs_function_scheduler_create(uint32_t max_timeout_ms, unsigned int max_entries, gs_function_scheduler_t ** return_scheduler)
{
    GS_CHECK_ARG(max_timeout_ms <= INT_MAX);
    GS_CHECK_ARG(max_entries > 0);
    GS_CHECK_ARG(return_scheduler != NULL);

    gs_function_scheduler_t * scheduler = calloc(1, (sizeof(*scheduler) + (max_entries * sizeof(scheduler->entries[0]))));
    if (scheduler == NULL) {
        return GS_ERROR_ALLOC;
    }

    scheduler->max_timeout_ms = max_timeout_ms;
    scheduler->max_entries = max_entries;

    *return_scheduler = scheduler;

    return GS_OK;
}

gs_error_t gs_function_scheduler_destroy(gs_function_scheduler_t * scheduler)
{
    GS_CHECK_HANDLE(scheduler);
    free(scheduler);
    return GS_OK;
}

gs_error_t gs_function_scheduler_register(gs_function_scheduler_t * scheduler,
                                          uint32_t first_timeout_ms, gs_function_scheduler_function_t func, void * user_data)
{
    GS_CHECK_HANDLE(scheduler != NULL);
    GS_CHECK_ARG(func != NULL);

    for (unsigned int i = 0; i < scheduler->max_entries; ++i) { 
        gs_function_scheduler_entry_t * entry = &scheduler->entries[i];
        if (entry->function == NULL) {
            entry->function = func;
            entry->user_data = user_data;
            entry->timeout_ms = first_timeout_ms;
            entry->last_exec_ms = gs_time_rel_ms();
            return GS_OK;
        }
    }

    return GS_ERROR_FULL;
}

gs_error_t gs_function_scheduler_modify_by_name(gs_function_scheduler_t * scheduler, const char * name,
                                                uint32_t interval_ms, gs_function_scheduler_function_t func, void * user_data)
{
    GS_CHECK_HANDLE(scheduler != NULL);
    GS_CHECK_ARG(name && name[0]);

    const bool add = ((interval_ms > 0) && (func != NULL));
    if (add == false) {
        func = NULL;
        user_data = NULL;
        interval_ms = 0;
    }

    gs_function_scheduler_entry_t * free_entry = NULL;
    for (unsigned int i = 0; i < scheduler->max_entries; ++i) {
        gs_function_scheduler_entry_t * entry = &scheduler->entries[i];
        if (add && (entry->function == NULL) && (free_entry == NULL)) {
            free_entry = entry;
        } else if (strcasecmp(entry->name, name) == 0) {
            // Found existing entry
            free_entry = entry;
            break;
        }
    }

    if (free_entry) {
        if (add) {
            GS_STRNCPY(free_entry->name, name);
        } else {
            free_entry->name[0] = 0;
        }
        free_entry->function = func;
        free_entry->user_data = user_data;
        free_entry->timeout_ms = interval_ms;
        free_entry->last_exec_ms = gs_time_rel_ms();
        return GS_OK;
    }

    return add ? GS_ERROR_FULL : GS_ERROR_NOT_FOUND;
}

int gs_function_scheduler_execute(gs_function_scheduler_t * scheduler)
{
    uint32_t timeout_ms = 5000; // max timeout to ensure gs_time_rel_ms() works correctly (wrapping more than once is bad)

    if (scheduler) {
        timeout_ms = scheduler->max_timeout_ms;
        uint32_t now_ms = gs_time_rel_ms();

        for (unsigned int i = 0; i < scheduler->max_entries; ++i) {
            gs_function_scheduler_entry_t * entry = &scheduler->entries[i];
            if (entry->function) {
                uint32_t elapsed = gs_time_diff_ms(entry->last_exec_ms, now_ms);
                if (elapsed >= entry->timeout_ms) {
                    uint32_t new_timeout_ms = (entry->function)(entry->user_data);
                    entry->last_exec_ms = now_ms = gs_time_rel_ms();
                    if (new_timeout_ms != GS_FUNCTION_SCHEDULER_REPEAT_INTERVAL) {
                        entry->timeout_ms = new_timeout_ms;
                    }
                    elapsed = 0;
                }
                timeout_ms = gs_min(timeout_ms, (entry->timeout_ms - elapsed));
            }
        }
    }

    return (int)((timeout_ms < INT_MAX) ? timeout_ms : INT_MAX);
}
