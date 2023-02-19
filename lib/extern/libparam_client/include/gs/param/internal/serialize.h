#ifndef GS_PARAM_INTERNAL_SERIALIZE_H
#define GS_PARAM_INTERNAL_SERIALIZE_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#if (GS_PARAM_INTERNAL_USE)

#include <gs/param/serialize.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Serialize data
 * @param mem Pointer to indexed parameter table
 * @param addr Array of addresses to serialize
 * @param addr_count number of items in addr array
 * @param[in|out] param_offset table parameter offset
 * @param flags Set options using combination of flags
 * @param buf Pointer to output
 * @param buf_size Size of \a buf.
 */
gs_error_t gs_param_serialize_list(gs_param_table_instance_t * tinst, const uint16_t addr[], unsigned int addr_count, unsigned int * param_pos, uint32_t flags, uint8_t * buf, unsigned int buf_size, unsigned int * buf_pos);

#ifdef __cplusplus
}
#endif
#endif
#endif
