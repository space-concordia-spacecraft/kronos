#ifndef PARAM_RPARAM_CLIENT_H
#define PARAM_RPARAM_CLIENT_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
 * GomSpace Parameter System
 */

#include <param/param_types.h>
#include <gs/param/rparam.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Legacy magic checksum.
   @deprecated use #GS_RPARAM_MAGIC_CHECKSUM (if remote products supports it).
*/
#define GS_RPARAM_LEGACY_MAGIC_CHECKSUM 0xb00b
    
static inline int rparam_get_full_table(param_index_t* idx, int node, int port, int index_id, uint32_t timeout_ms)
{
    return (gs_rparam_get_full_table((gs_param_table_instance_t *) idx,
                                     node,
                                     index_id,
                                     GS_RPARAM_LEGACY_MAGIC_CHECKSUM,
                                     timeout_ms) == GS_OK) ? 0 : -1;
}

static inline int rparam_download_table_spec_from_remote_and_save_to_file2(const char* fname, uint8_t node, uint8_t port, param_index_t* idx, uint16_t* return_checksum, uint32_t timeout_ms)
{
    return (gs_rparam_download_table_spec((gs_param_table_instance_t *) idx,
                                          fname,
                                          node,
                                          idx->mem_id,
                                          timeout_ms,
                                          return_checksum) == GS_OK) ? 0 : -1;
}

static inline int rparam_download_table_spec_from_remote_and_save_to_file(const char* fname, uint8_t node, uint8_t port, param_index_t* idx, uint16_t* checksum)
{
    return rparam_download_table_spec_from_remote_and_save_to_file2(fname, node, port, idx, checksum, 10000);
}

static inline int rparam_load_table_spec_from_file(const char* fname, param_index_t* idx, uint16_t* return_checksum)
{
    return (gs_rparam_load_table_spec((gs_param_table_instance_t *) idx, fname, return_checksum) == GS_OK) ? 0 : -1;
}
    
static inline int rparam_get_single(void * out, uint16_t addr, param_type_t type, int size, int mem_id, int node, int port, uint32_t timeout_ms)
{
    return (gs_rparam_get(node, mem_id, addr, type, GS_RPARAM_LEGACY_MAGIC_CHECKSUM, timeout_ms, out, size) == GS_OK) ? size : -1;
}

static inline int rparam_set_single(const void * in, uint16_t addr, param_type_t type, int size, int mem_id, int node, int port, uint32_t timeout_ms)
{
    return (gs_rparam_set(node, mem_id, addr, type, GS_RPARAM_LEGACY_MAGIC_CHECKSUM, timeout_ms, in, size) == GS_OK) ? size : -1;
}

static inline int rparam_copy(uint8_t node, uint8_t port, uint32_t timeout_ms, uint8_t from, uint8_t to)
{
    return (gs_rparam_copy(node, timeout_ms, from, to) == GS_OK) ? 1 : 0;
}

static inline int rparam_save(uint8_t node, uint8_t port, uint32_t timeout_ms, uint8_t from, uint8_t to)
{
    return (gs_rparam_save(node, timeout_ms, from, to) == GS_OK) ? 1 : 0;
}

static inline int rparam_load(uint8_t node, uint8_t port, uint32_t timeout_ms, uint8_t from, uint8_t to)
{
    return (gs_rparam_load(node, timeout_ms, from, to) == GS_OK) ? 1 : 0;
}

/**
 * Remote getters for parameters.
 * @param out pointer to the output buffer/parameter
 * @param outlen length of the supplied out-buffer, only applies to rparam_get_string()
 * @param addr local address of the parameter
 * @param node CSP address of the node to query
 * @param port CSP port of the parameter server
 * @param timeout timeout on remote CSP calls
 * @returns <0 on error in which case the contents of out is invalid, >0 on success
 */
static inline int rparam_get_string(char *out, int outlen, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_get_single(out, addr, PARAM_STRING, outlen, index_id, node, port, timeout_ms);
}

/**
 * Remote setters for parameters.
 * @param in pointer to the buffer/parameter value to set
 * @param inlen length of the supplied in-buffer, only applies to rparam_set_string()
 * @param addr local address of the parameter
 * @param node CSP address of the node to query
 * @param port CSP port of the parameter server
 * @param timeout timeout on remote CSP calls
 * @returns <0 on error in which case the contents of out is invalid, >0 on success
 */
static inline int rparam_set_string(const char *in, int inlen, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_set_single(in, addr, PARAM_STRING, inlen, index_id, node, port, timeout_ms);
}

static inline int rparam_get_uint8(uint8_t * out, uint16_t addr, int tinst_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_get_single(out, addr, PARAM_UINT8, sizeof(uint8_t), tinst_id, node, port, timeout_ms);
}

static inline int rparam_set_uint8(const uint8_t * in, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_set_single(in, addr, PARAM_UINT8, sizeof(uint8_t), index_id, node, port, timeout_ms);
}

static inline int rparam_get_uint16(uint16_t * out, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_get_single(out, addr, PARAM_UINT16, sizeof(uint16_t), index_id, node, port, timeout_ms);
}

static inline int rparam_set_uint16(const uint16_t * in, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_set_single(in, addr, PARAM_UINT16, sizeof(uint16_t), index_id, node, port, timeout_ms);
}

static inline int rparam_get_uint32(uint32_t * out, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_get_single(out, addr, PARAM_UINT32, sizeof(uint32_t), index_id, node, port, timeout_ms);
}

static inline int rparam_set_uint32(const uint32_t * in, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_set_single(in, addr, PARAM_UINT32, sizeof(uint32_t), index_id, node, port, timeout_ms);
}

static inline int rparam_get_uint64(uint64_t * out, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_get_single(out, addr, PARAM_UINT64, sizeof(uint64_t), index_id, node, port, timeout_ms);
}

static inline int rparam_set_uint64(const uint64_t * in, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_set_single(in, addr, PARAM_UINT64, sizeof(uint64_t), index_id, node, port, timeout_ms);
}

static inline int rparam_get_int8(int8_t * out, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_get_single(out, addr, PARAM_INT8, sizeof(int8_t), index_id, node, port, timeout_ms);
}

static inline int rparam_set_int8(const int8_t * in, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_set_single(in, addr, PARAM_INT8, sizeof(int8_t), index_id, node, port, timeout_ms);
}

static inline int rparam_get_int16(int16_t * out, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_get_single(out, addr, PARAM_INT16, sizeof(int16_t), index_id, node, port, timeout_ms);
}

static inline int rparam_set_int16(const int16_t * in, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_set_single(in, addr, PARAM_INT16, sizeof(int16_t), index_id, node, port, timeout_ms);
}

static inline int rparam_get_int32(int32_t * out, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_get_single(out, addr, PARAM_INT32, sizeof(int32_t), index_id, node, port, timeout_ms);
}

static inline int rparam_set_int32(const int32_t * in, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_set_single(in, addr, PARAM_INT32, sizeof(int32_t), index_id, node, port, timeout_ms);
}

static inline int rparam_get_int64(int64_t * out, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_get_single(out, addr, PARAM_INT64, sizeof(int64_t), index_id, node, port, timeout_ms);
}

static inline int rparam_set_int64(const int64_t * in, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_set_single(in, addr, PARAM_INT64, sizeof(int64_t), index_id, node, port, timeout_ms);
}

static inline int rparam_get_float(float * out, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_get_single(out, addr, PARAM_FLOAT, sizeof(float), index_id, node, port, timeout_ms);
}

static inline int rparam_set_float(const float * in, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_set_single(in, addr, PARAM_FLOAT, sizeof(float), index_id, node, port, timeout_ms);
}

static inline int rparam_get_double(double * out, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_get_single(out, addr, PARAM_DOUBLE, sizeof(double), index_id, node, port, timeout_ms);
}

static inline int rparam_set_double(const double * in, uint16_t addr, int index_id, int node, int port, uint32_t timeout_ms)
{
    return rparam_set_single(in, addr, PARAM_DOUBLE, sizeof(double), index_id, node, port, timeout_ms);
}

#ifdef __cplusplus
}
#endif
#endif
