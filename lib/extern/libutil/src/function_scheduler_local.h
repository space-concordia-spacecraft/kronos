/* Copyright (c) 2013-2019 GomSpace A/S. All rights reserved. */

#include <gs/util/function_scheduler.h>

typedef struct {
    // name of entry
    char name[10];
    // function to call
    gs_function_scheduler_function_t function;
    // function's user data
    void * user_data;
    // timeout in mS
    uint32_t timeout_ms;
    // last execution time in mS
    uint32_t last_exec_ms;
} gs_function_scheduler_entry_t;

struct gs_function_scheduler {
    // Max timeout in mS
    uint32_t max_timeout_ms;
    // allocated entries
    unsigned int max_entries;
    // entries
    gs_function_scheduler_entry_t entries[0];
};
