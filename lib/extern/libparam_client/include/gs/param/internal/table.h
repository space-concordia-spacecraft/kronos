#ifndef GS_PARAM_INTERNAL_TABLE_H
#define GS_PARAM_INTERNAL_TABLE_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#if (GS_PARAM_INTERNAL_USE)

#include <gs/param/table.h>

#ifdef __cplusplus
extern "C" {
#endif

gs_error_t gs_param_parse_name_and_array_index(const char * inp, char * name, size_t size_name, uint8_t * array_index, bool * return_is_array);

#ifdef __cplusplus
}
#endif
#endif
#endif
