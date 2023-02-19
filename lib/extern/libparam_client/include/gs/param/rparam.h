#ifndef GS_PARAM_REMOTE_H
#define GS_PARAM_REMOTE_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Remote parameter API - pending refactoring.
*/

#include <gs/param/types.h>
#include <gs/util/string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Magic checksum.
   If specifying a magic checksum, the rparam server will ignore/skip checksum validation.
*/
#define GS_RPARAM_MAGIC_CHECKSUM        0x0bb0

/**
   Register rparam commands.
   @return_gs_error_t
*/
gs_error_t gs_rparam_register_commands(void);

/**
   Download all (data) values from a remote table to a local instance.

   @param[in] tinst local table instance.
   @param[in] node CSP address.
   @param[in] table_id remote table id to download.
   @param[in] checksum table checksum.
   @param[in] timeout_ms timeout.
   @return_gs_error_t
*/
gs_error_t gs_rparam_get_full_table(gs_param_table_instance_t * tinst,
                                    uint8_t node,
                                    gs_param_table_id_t table_id,
                                    uint16_t checksum,
                                    uint32_t timeout_ms);

/**
   Download a table specification from a remote node, store it in memory and save it to local file system.

   @note Will free existing rows - do not use this on table instances with static assigned rows.

   Table memory will be (re)allocated to match specification.

   @param[in] tinst local table instance.
   @param[in] fname name of the file to store the table specification in. If NULL, no file will be stored.
   @param[in] node CSP address
   @param[in] table_id remote table id to download.
   @param[in] timeout_ms timeout.
   @param[out] return_checksum fletcher16 checksum of downloaded specification "as is" - before network/host swapping.
   @return_gs_error_t
   @see gs_param_table_free()
*/
gs_error_t gs_rparam_download_table_spec(gs_param_table_instance_t * tinst,
                                         const char * fname,
                                         uint8_t node,
                                         gs_param_table_id_t table_id,
                                         uint32_t timeout_ms,
                                         uint16_t * return_checksum);

/**
   Load a table specification from a local file and store it in memory.

   @note Will free existing rows - do not use this on table instances with static assigned rows.

   Table memory will be (re)allocated to match specification.

   @param[in] tinst local table instance.
   @param[in] fname name of the file to load the table specification from.
   @param[out] return_checksum fletcher16 checksum stored in file.
   @return_gs_error_t
   @see gs_param_table_free()
*/
gs_error_t gs_rparam_load_table_spec(gs_param_table_instance_t * tinst, const char* fname, uint16_t * return_checksum);

/**
   Copy from one table to another table.

   @deprecated Not supported by a param 4 backend and future versions.

   @param[in] node CSP address
   @param[in] timeout_ms timeout on remote CSP calls
   @param[in] from from table-id.
   @param[in] to to table-id.
   @return_gs_error_t
*/
gs_error_t gs_rparam_copy(uint8_t node, uint32_t timeout_ms, uint8_t from, uint8_t to);

/**
   Save table.

   @note On a param 4 backend, the table will always be saved to it's primary store.

   @param[in] node CSP address
   @param[in] timeout_ms timeout on remote CSP calls
   @param[in] id table to save.
   @param[in] to to file slot - ignored on param 4 backends.
   @return_gs_error_t
*/
gs_error_t gs_rparam_save(uint8_t node, uint32_t timeout_ms, gs_param_table_id_t id, uint8_t to);

/**
   Save table.

   @version 4
   @param[in] node CSP address
   @param[in] timeout_ms timeout on remote CSP calls
   @param[in] table_id remote table id.
   @param[in] store store name.
   @param[in] slot slot within store.
   @return_gs_error_t
*/
gs_error_t gs_rparam_save_to_store(uint8_t node, uint32_t timeout_ms, uint8_t table_id,
                                   const char * store, const char * slot);

/**
   Load table from store.

   @note On a param 4 backend, the specified table will be loadded from it's primary store.

   @param[in] node CSP address
   @param[in] timeout_ms timeout on remote CSP calls
   @param[in] from from file slot - ignored on param 4 backends.
   @param[in] id table to load.
   @return_gs_error_t
*/
gs_error_t gs_rparam_load(uint8_t node, uint32_t timeout_ms, uint8_t from, gs_param_table_id_t id);

/**
   Load table from store.

   @version 4
   @param[in] node CSP address
   @param[in] timeout_ms timeout on remote CSP calls
   @param[in] table_id remote table id.
   @param[in] store store name.
   @param[in] slot slot within store.
   @return_gs_error_t
*/
gs_error_t gs_rparam_load_from_store(uint8_t node, uint32_t timeout_ms, uint8_t table_id,
                                     const char * store, const char * slot);

/**
   Get parameter.

   @param[in] node CSP address
   @param[in] table_id remote table id.
   @param[in] addr parameter address (remote table).
   @param[in] type parameter type.
   @param[in] checksum checksum
   @param[in] timeout_ms timeout
   @param[out] value returned value (user allocated)
   @param[in] value_element_size size of \a value element, i.e. size of parameter type in bytes.
   @param[in] array_size number of elements in \a value buffer.
   @return_gs_error_t
*/
gs_error_t gs_rparam_get_array(uint8_t node,
                               gs_param_table_id_t table_id,
                               uint16_t addr,
                               gs_param_type_t type,
                               uint16_t checksum,
                               uint32_t timeout_ms,
                               void * value,
                               size_t value_element_size,
                               size_t array_size);

/**
   Get parameter.

   @param[in] node CSP address
   @param[in] table_id remote table id.
   @param[in] addr parameter address (remote table).
   @param[in] type parameter type.
   @param[in] checksum checksum
   @param[in] timeout_ms timeout
   @param[out] value returned value (user allocated)
   @param[in] value_size size of \a value, i.e. size of parameter type in bytes.
   @return_gs_error_t
*/
gs_error_t gs_rparam_get(uint8_t node,
                         gs_param_table_id_t table_id,
                         uint16_t addr,
                         gs_param_type_t type,
                         uint16_t checksum,
                         uint32_t timeout_ms,
                         void * value,
                         size_t value_size);

/**
   Set parameter.

   @param[in] node CSP address
   @param[in] table_id remote table id.
   @param[in] addr parameter address (remote table).
   @param[in] type parameter type.
   @param[in] checksum checksum
   @param[in] timeout_ms timeout
   @param[in] value value to set
   @param[in] value_size size of \a value.
   @return_gs_error_t
*/
gs_error_t gs_rparam_set(uint8_t node,
                         gs_param_table_id_t table_id,
                         uint16_t addr,
                         gs_param_type_t type,
                         uint16_t checksum,
                         uint32_t timeout_ms,
                         const void * value,
                         size_t value_size);

/**
   Get string.
   @note If the returned string is max length, the value buffer will not be 0 terminated.
   @param[in] node CSP address
   @param[in] table_id remote table id.
   @param[in] addr parameter address (remote table).
   @param[in] checksum checksum
   @param[in] timeout_ms timeout
   @param[out] value returned value (user allocated)
   @param[in] value_size size of \a value, i.e. size of parameter type in bytes.
   @return_gs_error_t
*/
static inline gs_error_t gs_rparam_get_string(uint8_t node, gs_param_table_id_t table_id, uint16_t addr,
                                              uint16_t checksum, uint32_t timeout_ms, char * value, size_t value_size)
{
    return gs_rparam_get(node, table_id, addr, GS_PARAM_STRING, checksum, timeout_ms, value, value_size);
}

/**
   Set string.
*/
static inline gs_error_t gs_rparam_set_string(uint8_t node, gs_param_table_id_t table_id,  uint16_t addr,
                                              uint16_t checksum, uint32_t timeout_ms, const char * value, size_t value_size)
{
    return gs_rparam_set(node, table_id, addr, GS_PARAM_STRING, checksum, timeout_ms, value, (value_size == 0) ? (strlen(value) + 1) : value_size);
}

/**
   Get int8.
*/
static inline gs_error_t gs_rparam_get_int8(uint8_t node, gs_param_table_id_t table_id, uint16_t addr,
                                            uint16_t checksum, uint32_t timeout_ms, int8_t * value)
{
    return gs_rparam_get(node, table_id, addr, GS_PARAM_INT8, checksum, timeout_ms, value, sizeof(*value));
}

/**
   Set int8.
*/
static inline gs_error_t gs_rparam_set_int8(uint8_t node, gs_param_table_id_t table_id,  uint16_t addr,
                                            uint16_t checksum, uint32_t timeout_ms, int8_t value)
{
    return gs_rparam_set(node, table_id, addr, GS_PARAM_INT8, checksum, timeout_ms, &value, sizeof(value));
}

/**
   Get uint8.
*/
static inline gs_error_t gs_rparam_get_uint8(uint8_t node, gs_param_table_id_t table_id, uint16_t addr,
                                             uint16_t checksum, uint32_t timeout_ms, uint8_t * value)
{
    return gs_rparam_get(node, table_id, addr, GS_PARAM_UINT8, checksum, timeout_ms, value, sizeof(*value));
}

/**
   Set uint8.
*/
static inline gs_error_t gs_rparam_set_uint8(uint8_t node, gs_param_table_id_t table_id,  uint16_t addr,
                                             uint16_t checksum, uint32_t timeout_ms, uint8_t value)
{
    return gs_rparam_set(node, table_id, addr, GS_PARAM_UINT8, checksum, timeout_ms, &value, sizeof(value));
}

/**
   Get int16.
*/
static inline gs_error_t gs_rparam_get_int16(uint8_t node, gs_param_table_id_t table_id, uint16_t addr,
                                             uint16_t checksum, uint32_t timeout_ms, int16_t * value)
{
    return gs_rparam_get(node, table_id, addr, GS_PARAM_INT16, checksum, timeout_ms, value, sizeof(*value));
}

/**
   Set int16.
*/
static inline gs_error_t gs_rparam_set_int16(uint8_t node, gs_param_table_id_t table_id,  uint16_t addr,
                                             uint16_t checksum, uint32_t timeout_ms, int16_t value)
{
    return gs_rparam_set(node, table_id, addr, GS_PARAM_INT16, checksum, timeout_ms, &value, sizeof(value));
}

/**
   Get uint16.
*/
static inline gs_error_t gs_rparam_get_uint16(uint8_t node, gs_param_table_id_t table_id, uint16_t addr,
                                              uint16_t checksum, uint32_t timeout_ms, uint16_t * value)
{
    return gs_rparam_get(node, table_id, addr, GS_PARAM_UINT16, checksum, timeout_ms, value, sizeof(*value));
}

/**
   Set uint16.
*/
static inline gs_error_t gs_rparam_set_uint16(uint8_t node, gs_param_table_id_t table_id,  uint16_t addr,
                                              uint16_t checksum, uint32_t timeout_ms, uint16_t value)
{
    return gs_rparam_set(node, table_id, addr, GS_PARAM_UINT16, checksum, timeout_ms, &value, sizeof(value));
}

/**
   Get int32.
*/
static inline gs_error_t gs_rparam_get_int32(uint8_t node, gs_param_table_id_t table_id, uint16_t addr,
                                             uint16_t checksum, uint32_t timeout_ms, int32_t * value)
{
    return gs_rparam_get(node, table_id, addr, GS_PARAM_INT32, checksum, timeout_ms, value, sizeof(*value));
}

/**
   Set int32.
*/
static inline gs_error_t gs_rparam_set_int32(uint8_t node, gs_param_table_id_t table_id,  uint16_t addr,
                                             uint16_t checksum, uint32_t timeout_ms, int32_t value)
{
    return gs_rparam_set(node, table_id, addr, GS_PARAM_INT32, checksum, timeout_ms, &value, sizeof(value));
}

/**
   Get uint32.
*/
static inline gs_error_t gs_rparam_get_uint32(uint8_t node, gs_param_table_id_t table_id, uint16_t addr,
                                              uint16_t checksum, uint32_t timeout_ms, uint32_t * value)
{
    return gs_rparam_get(node, table_id, addr, GS_PARAM_UINT32, checksum, timeout_ms, value, sizeof(*value));
}

/**
   Set uint32.
*/
static inline gs_error_t gs_rparam_set_uint32(uint8_t node, gs_param_table_id_t table_id,  uint16_t addr,
                                              uint16_t checksum, uint32_t timeout_ms, uint32_t value)
{
    return gs_rparam_set(node, table_id, addr, GS_PARAM_UINT32, checksum, timeout_ms, &value, sizeof(value));
}

/**
   Get int64.
*/
static inline gs_error_t gs_rparam_get_int64(uint8_t node, gs_param_table_id_t table_id, uint16_t addr,
                                             uint16_t checksum, uint32_t timeout_ms, int64_t * value)
{
    return gs_rparam_get(node, table_id, addr, GS_PARAM_INT64, checksum, timeout_ms, value, sizeof(*value));
}

/**
   Set int64.
*/
static inline gs_error_t gs_rparam_set_int64(uint8_t node, gs_param_table_id_t table_id,  uint16_t addr,
                                             uint16_t checksum, uint32_t timeout_ms, int64_t value)
{
    return gs_rparam_set(node, table_id, addr, GS_PARAM_INT64, checksum, timeout_ms, &value, sizeof(value));
}

/**
   Get uint64.
*/
static inline gs_error_t gs_rparam_get_uint64(uint8_t node, gs_param_table_id_t table_id, uint16_t addr,
                                              uint16_t checksum, uint32_t timeout_ms, uint64_t * value)
{
    return gs_rparam_get(node, table_id, addr, GS_PARAM_UINT64, checksum, timeout_ms, value, sizeof(*value));
}

/**
   Set uint64.
*/
static inline gs_error_t gs_rparam_set_uint64(uint8_t node, gs_param_table_id_t table_id,  uint16_t addr,
                                              uint16_t checksum, uint32_t timeout_ms, uint64_t value)
{
    return gs_rparam_set(node, table_id, addr, GS_PARAM_UINT64, checksum, timeout_ms, &value, sizeof(value));
}

/**
   Get float.
*/
static inline gs_error_t gs_rparam_get_float(uint8_t node, gs_param_table_id_t table_id, uint16_t addr,
                                             uint16_t checksum, uint32_t timeout_ms, float * value)
{
    return gs_rparam_get(node, table_id, addr, GS_PARAM_FLOAT, checksum, timeout_ms, value, sizeof(*value));
}

/**
   Set float.
*/
static inline gs_error_t gs_rparam_set_float(uint8_t node, gs_param_table_id_t table_id,  uint16_t addr,
                                             uint16_t checksum, uint32_t timeout_ms, float value)
{
    return gs_rparam_set(node, table_id, addr, GS_PARAM_FLOAT, checksum, timeout_ms, &value, sizeof(value));
}

/**
   Get double.
*/
static inline gs_error_t gs_rparam_get_double(uint8_t node, gs_param_table_id_t table_id, uint16_t addr,
                                              uint16_t checksum, uint32_t timeout_ms, double * value)
{
    return gs_rparam_get(node, table_id, addr, GS_PARAM_DOUBLE, checksum, timeout_ms, value, sizeof(*value));
}

/**
   Set double.
*/
static inline gs_error_t gs_rparam_set_double(uint8_t node, gs_param_table_id_t table_id,  uint16_t addr,
                                              uint16_t checksum, uint32_t timeout_ms, double value)
{
    return gs_rparam_set(node, table_id, addr, GS_PARAM_DOUBLE, checksum, timeout_ms, &value, sizeof(value));
}

#ifdef __cplusplus
}
#endif
#endif
