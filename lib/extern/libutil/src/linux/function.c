/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/linux/function.h>
#include <gs/util/string.h>

typedef struct {
    const char * short_name;
    const char * long_name;
    gs_function_t function;
} gs_function_register_t;

static gs_function_register_t registry[10];

gs_error_t gs_function_register(const char * short_name, const char * long_name, gs_function_t function)
{
    for (unsigned int i = 0; i < GS_ARRAY_SIZE(registry); ++i) {
        gs_function_register_t * cb = &registry[i];
        if ((cb->short_name == NULL) && (cb->long_name == NULL)) {
            cb->short_name = short_name;
            cb->long_name = long_name;
            cb->function = function;
            return GS_OK;
        }
    }
    return GS_ERROR_FULL;
}

gs_error_t gs_function_invoke(const char * name, void * arg)
{
    for (unsigned int i = 0; i < GS_ARRAY_SIZE(registry); ++i) {
        gs_function_register_t * cb = &registry[i];
        if ((gs_string_empty(cb->short_name) == false) && (strcasecmp(cb->short_name, name) == 0)) {
            return (cb->function)(arg);
        }
        if ((gs_string_empty(cb->long_name) == false) && (strcasecmp(cb->long_name, name) == 0)) {
            return (cb->function)(arg);
        }
    }

    return GS_ERROR_NOT_IMPLEMENTED;
}
