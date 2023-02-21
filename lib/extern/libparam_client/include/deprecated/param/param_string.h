#ifndef PARAM_PARAM_STRING_H
#define PARAM_PARAM_STRING_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
 * GomSpace Parameter System
 */

#include <param/param_types.h>
#include <gs/param/table.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
    const param_table_t * row3;
    const gs_param_table_row_t * row4;
} gs_param_row_align_t;

static inline const param_table_t * param_find_name(const param_table_t rows[], size_t row_count, const char * name)
{
    gs_param_row_align_t row_in = {.row3 = rows};
    gs_param_row_align_t row_out = {.row4 = gs_param_row_by_name(name, row_in.row4, row_count)};
    return row_out.row3;
}

static inline const param_table_t * param_find_addr(const param_table_t rows[], size_t row_count, uint16_t addr)
{
    gs_param_row_align_t row_in = {.row3 = rows};
    gs_param_row_align_t row_out = {.row4 = gs_param_row_by_address(addr, row_in.row4, row_count)};
    return row_out.row3;
}

static inline void param_list_single(param_table_t * param, param_index_t * mem, int do_read)
{
    gs_param_list_single((gs_param_table_instance_t *) mem, (const gs_param_table_row_t *) param, (do_read != 0));
}

static inline void param_list(param_index_t * mem, int do_read)
{
    gs_param_list((gs_param_table_instance_t *) mem, (do_read != 0));
}

static inline gs_error_t param_from_string(const param_table_t * param , const char * string, void * value)
{
    return gs_param_from_string((const gs_param_table_row_t *)param , string, value);
}

static inline int param_to_string(const param_table_t * param, char * buf, int pos, const void * value, int with_type, int max_size)
{
    unsigned int written = 0;
    gs_param_to_string((const gs_param_table_row_t *)param, value, with_type, buf, max_size, pos, &written);
    return (int) written;
}

static inline const char * param_type_to_string(param_type_t type)
{
    return gs_param_type_to_string(type);
}

static inline uint16_t param_index_chksum(param_index_t * mem)
{
    return gs_param_table_checksum((gs_param_table_instance_t*)mem);
}

static inline uint16_t param_index_chksum2(param_index_t * mem)
{
    return gs_param_table_checksum2((gs_param_table_instance_t*)mem);
}

#ifdef __cplusplus
}
#endif
#endif
