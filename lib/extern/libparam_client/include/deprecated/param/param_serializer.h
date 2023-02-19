#ifndef PARAM_PARAM_SERIALIZER_H
#define PARAM_PARAM_SERIALIZER_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
 * GomSpace Parameter System
 */

#include <param/param_types.h>
#include <gs/param/serialize.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef gs_param_serialize_flags_t param_serializer_flags;
    
static inline int param_betoh(param_type_t type, void * item)
{
    return gs_param_betoh(type, item);
}

static inline int param_htobe(param_type_t type, void * item)
{
    return gs_param_htobe(type, item);
}

static inline int param_serialize_full_table(param_index_t * mem, unsigned int *start, uint8_t * buf, unsigned int maxbuflen, param_serializer_flags flags)
{
    unsigned int buf_pos = 0;
    gs_error_t error = gs_param_serialize_full_table((gs_param_table_instance_t *) mem, start, flags, buf, maxbuflen, &buf_pos);
    return (error == GS_OK) ? (int) buf_pos : -1;
}

static inline gs_error_t param_serialize_item(const param_table_t * param, uint16_t addr, uint8_t * buf, uint16_t * pos, unsigned int maxlen, void * item, param_serializer_flags flags)
{
    unsigned int tmp_pos = *pos;
    gs_error_t error = gs_param_serialize_item((const gs_param_table_row_t*) param, addr, item, flags, buf, maxlen, &tmp_pos);
    *pos = tmp_pos;
    return (error == GS_OK) ? 0 : -1;
}

static inline gs_error_t param_deserialize(param_index_t * mem, uint8_t * buf, int len, param_serializer_flags flags)
{
    return (gs_param_deserialize((gs_param_table_instance_t *) mem, buf, len, flags) == GS_OK) ? 0 : -1;
}

static inline gs_error_t param_deserialize_item(const param_table_t * param, uint16_t addr, param_index_t * mem, void * item, param_serializer_flags flags)
{
    return (gs_param_deserialize_item((gs_param_table_instance_t *) mem, (const gs_param_table_row_t*)param, addr, item, flags) == GS_OK) ? 0 : -1;
}

#ifdef __cplusplus
}
#endif
#endif
