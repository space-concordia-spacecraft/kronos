#ifndef GS_PARAM_CLIENT_SERIALIZE_H
#define GS_PARAM_CLIENT_SERIALIZE_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Serialize API - pending refactoring.
*/

#include <gs/param/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Serialize/deserialize flags
   Flags must be in range: bit 8 - 15, to avoid clash with other parts of the parameter system.
*/
typedef enum {
    GS_PARAM_SF_DRY_RUN           = (1 <<  8), //!< F_DRY_RUN do not write to memory
    GS_PARAM_SF_TO_BIG_ENDIAN     = (1 <<  9), //!< F_TO_BIG_ENDIAN Convert from host to big endian
    GS_PARAM_SF_FROM_BIG_ENDIAN   = (1 << 10), //!< F_FROM_BIG_ENDIAN Confert from big endian to host order
    GS_PARAM_SF_PACKED            = (1 << 11), //!< F_PACKED Do not pack addresses

    F_DRY_RUN           = GS_PARAM_SF_DRY_RUN,
    F_TO_BIG_ENDIAN     = GS_PARAM_SF_TO_BIG_ENDIAN,
    F_FROM_BIG_ENDIAN   = GS_PARAM_SF_FROM_BIG_ENDIAN,
    F_PACKED            = GS_PARAM_SF_PACKED,
} gs_param_serialize_flags_t;

/**
 * In-place conversion of a single parameter from big endian to host byte order
 * @param type param type
 * @param item pointer to parameter memory
 * @return 1 if memory has been swapped, 0 if not
 */
bool gs_param_betoh(gs_param_type_t type, void * item);

/**
 * In-place conversion of a single parameter from host byte order to big endian
 * @param type param type
 * @param item porinter to parameter memory
 * @return 1 if memory has been swapped, 0 if not
 */
bool gs_param_htobe(gs_param_type_t type, void * item);

/**
   Serialize data

   @param[in] tinst table.
   @param[in,out] param_pos parameter iterator.
   @param[in] flags flags.
   @param[out] buf user supplied buffer of \a buf_size.
   @param[in] buf_size of size of \a buf
   @param[in,out] buf_pos index into \a buf
   @return_gs_error_t
*/
gs_error_t gs_param_serialize_full_table(gs_param_table_instance_t * tinst, unsigned int * param_pos, uint32_t flags, uint8_t * buf, unsigned int buf_size, unsigned int * buf_pos);

/**
   Serialize single item

   @param[in] param parameter to serialize
   @param[in] addr Address of item
   @param[in] item item to serialize.
   @param[in] flags flags.
   @param[out] buf user supplied buffer of \a buf_size.
   @param[in] buf_size of size of \a buf
   @param[in,out] buf_pos index into \a buf
   @return_gs_error_t
*/
gs_error_t gs_param_serialize_item(const gs_param_table_row_t * param, uint16_t addr, const void * item, uint32_t flags, uint8_t * buf, unsigned int buf_size, unsigned int * buf_pos);

/**
   Deserialize packed parameters into memory

   @param[in] tinst table.
   @param[in] buf serialized data.
   @param[in] buf_size size \a buf containing serialized data
   @param[in] flags flags.
   @return_gs_error_t
*/
gs_error_t gs_param_deserialize(gs_param_table_instance_t * tinst, const uint8_t * buf, unsigned int buf_size, uint32_t flags);

/**
   Deserialize a sginle item from a string into memory

   @param[in] tinst table.
   @param param Pointer to specific parameter to deserialize
   @param addr Address of parameter
   @param item Pointer to memory area where item should be written
   @param flags Set options using combination of flags
   @return_gs_error_t
*/
gs_error_t gs_param_deserialize_item(gs_param_table_instance_t * tinst, const gs_param_table_row_t * param, uint16_t addr, const void * item, uint32_t flags);

#ifdef __cplusplus
}
#endif
#endif
