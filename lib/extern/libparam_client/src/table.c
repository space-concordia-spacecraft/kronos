/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#define GS_PARAM_INTERNAL_USE 1

#include <gs/param/internal/types.h>
#include <gs/param/internal/table.h>
#include <gs/param/serialize.h>
#include <gs/util/check.h>
#include <gs/util/byteorder.h>
#include <gs/util/endian.h>
#include <gs/util/fletcher.h>

#include <alloca.h>
#include <stdlib.h>

static void _copy_data(gs_param_type_t type, void * dst, const void * src, size_t size)
{
#if (GS_PARAM_ATOMIC_ACCESS)
    switch (type) {
        case GS_PARAM_UINT8:
        case GS_PARAM_INT8:
        case PARAM_X8:
        case GS_PARAM_STRING:
        case GS_PARAM_DATA:
        case GS_PARAM_BOOL:
            break;

        case GS_PARAM_UINT16:
        case GS_PARAM_INT16:
        case PARAM_X16:
            if (!((intptr_t)src & 1) && !((intptr_t)dst & 1)) {
                // (u)int16 aligned correctly
                const uint16_t * s = src;
                uint16_t * d = dst;
                const unsigned int count = (size / sizeof(*d));
                for (unsigned int i = 0; i < count; ++i, ++d, ++s) {
                    *d = *s;
                }
                //printf("%s:int16\r\n", __FUNCTION__);
                return;
            }
            break;

        case GS_PARAM_UINT32:
        case GS_PARAM_INT32:
        case PARAM_X32:
        case GS_PARAM_FLOAT:
            if (!((intptr_t)src & 3) && !((intptr_t)dst & 3)) {
                // (u)int32 aligned correctly
                const uint32_t * s = src;
                uint32_t * d = dst;
                const unsigned int count = (size / sizeof(*d));
                for (unsigned int i = 0; i < count; ++i, ++d, ++s) {
                    *d = *s;
                }
                //printf("%s:int32\r\n", __FUNCTION__);
                return;
            }
            break;

        case GS_PARAM_UINT64:
        case GS_PARAM_INT64:
        case PARAM_X64:
        case GS_PARAM_DOUBLE:
            break;
    }
#endif

    // fallback - do byte copy
    memcpy(dst, src, size);
}

gs_error_t gs_param_table_lock(gs_param_table_instance_t * tinst)
{
    if (tinst->lock) {
        return gs_mutex_lock(tinst->lock);
    }
    return GS_OK;
}

gs_error_t gs_param_table_unlock(gs_param_table_instance_t * tinst)
{
    if (tinst->lock) {
        return gs_mutex_unlock(tinst->lock);
    }
    return GS_OK;
}

static gs_error_t gs_param_table_lock_free(gs_param_table_instance_t * tinst)
{
    if (tinst && tinst->lock) {
        gs_mutex_destroy(tinst->lock);
        tinst->lock = NULL;
    }
    return GS_OK;
}

gs_error_t gs_param_table_free(gs_param_table_instance_t * tinst)
{
    if (tinst) {
        if (tinst->flags & GS_PARAM_TABLE_F_ALLOC_MEMORY) {
            free(tinst->memory);
        }
        if (tinst->flags & GS_PARAM_TABLE_F_ALLOC_ROWS) {
            free((void*)tinst->rows);
        }
        if (tinst->lock) {
            gs_param_table_lock_free(tinst);
        }
        memset(tinst, 0, sizeof(*tinst));
    }
    return GS_OK;
}

static void checksum_update(gs_param_table_instance_t * tinst)
{
    const uint16_t no_swap = gs_fletcher16(tinst->rows, (sizeof(*tinst->rows) * tinst->row_count));

    // fletcher16 with swapped fields > 1 byte
    gs_fletcher16_t f16;
    gs_fletcher16_init(&f16);
    for (unsigned int i = 0; i < tinst->row_count; ++i) {
        gs_param_table_row_t row = tinst->rows[i];
        row.addr = gs_bswap_16(row.addr);
        gs_fletcher16_update(&f16, &row, sizeof(row));
    }
    const uint16_t swap = gs_fletcher16_finalize(&f16);

    if (gs_endian_big()) {
        tinst->checksum_be = no_swap;
        tinst->checksum_le = swap;
    } else {
        tinst->checksum_be = swap;
        tinst->checksum_le = no_swap;
    }
}

uint16_t gs_param_table_checksum_be(gs_param_table_instance_t * tinst)
{
    if (tinst && tinst->rows && tinst->row_count) {
        if (tinst->checksum_be == 0) {
            checksum_update(tinst);
        }
        return tinst->checksum_be;
    }
    return 0;
}

uint16_t gs_param_table_checksum_le(gs_param_table_instance_t * tinst)
{
    if (tinst && tinst->rows && tinst->row_count) {
        if (tinst->checksum_le == 0) {
            checksum_update(tinst);
        }
        return tinst->checksum_le;
    }
    return 0;
}

uint16_t gs_param_table_checksum(gs_param_table_instance_t * tinst)
{
    if (gs_endian_big()) {
        return gs_param_table_checksum_be(tinst);
    } else {
        return gs_param_table_checksum_le(tinst);
    }
}

gs_error_t gs_param_get(gs_param_table_instance_t * tinst, uint16_t addr, gs_param_type_t type, void * value, size_t value_size, uint32_t flags)
{
    if (tinst == NULL) {
        return GS_ERROR_HANDLE;
    }

    if ((addr + value_size) > tinst->memory_size) {
        return GS_ERROR_RANGE;
    }

    gs_error_t error = GS_ERROR_NOT_SUPPORTED;
    if (tinst->function_interface.get) {
        error = (tinst->function_interface.get)(tinst->function_interface.context, tinst, addr, type, value, value_size, flags);
    } else if (tinst->memory) {
        gs_param_table_lock(tinst);
        _copy_data(type, value, ((uint8_t*)(tinst->memory)) + addr, value_size);
        gs_param_table_unlock(tinst);
        error = GS_OK;
    }

    if ((error == GS_OK) && (flags & GS_PARAM_SF_TO_BIG_ENDIAN) && !gs_endian_big()) {
        gs_param_htobe(type, value);
    }

    return error;
}

gs_error_t gs_param_set(gs_param_table_instance_t * tinst, uint16_t addr, gs_param_type_t type, const void * value, size_t value_size, uint32_t flags)
{
    if (tinst == NULL) {
        return GS_ERROR_HANDLE;
    }

    if ((addr + value_size) > tinst->memory_size) {
        return GS_ERROR_RANGE;
    }

    if ((flags & GS_PARAM_SF_FROM_BIG_ENDIAN) && !gs_endian_big()) {
        void * tmp = alloca(value_size); // this must be aligned
        memcpy(tmp, value, value_size);
        value = tmp;
        gs_param_betoh(type, tmp);
    }

    gs_error_t error = GS_ERROR_NOT_SUPPORTED;
    if (tinst->function_interface.set) {
        error = (tinst->function_interface.set)(tinst->function_interface.context, tinst, addr, type, value, value_size, flags);
    } else if (tinst->memory) {
        gs_param_table_lock(tinst);
        _copy_data(type, ((uint8_t*)(tinst->memory)) + addr, value, value_size);
        gs_param_table_unlock(tinst);

        if (tinst->auto_persist.set && (flags & GS_PARAM_F_AUTO_PERSIST)) {
            (tinst->auto_persist.set)(tinst, addr, type, value, value_size, flags);
        }
        error = GS_OK;
    }

    // Callbacks
    if ((error == GS_OK) && tinst->callback && ((flags & GS_PARAM_F_NO_CALLBACK) == 0)) {
        (tinst->callback)(addr, tinst);
    }

    return error;
}

uint8_t gs_param_type_size(gs_param_type_t type)
{
    switch (type) {
        case GS_PARAM_UINT8:
        case GS_PARAM_INT8:
        case PARAM_X8:
        case GS_PARAM_STRING:
        case GS_PARAM_DATA:
            return sizeof(int8_t);
        case GS_PARAM_INT16:
        case GS_PARAM_UINT16:
        case PARAM_X16:
            return sizeof(int16_t);
        case GS_PARAM_INT32:
        case GS_PARAM_UINT32:
        case PARAM_X32:
            return sizeof(int32_t);
        case GS_PARAM_INT64:
        case GS_PARAM_UINT64:
        case PARAM_X64:
            return sizeof(int64_t);
        case GS_PARAM_DOUBLE:
            return sizeof(double);
        case GS_PARAM_FLOAT:
            return sizeof(float);
        case GS_PARAM_BOOL:
            return sizeof(bool);
    }
    return 0;
}

void * gs_param_table_get_memory(gs_param_table_instance_t * tinst, size_t * return_size)
{
    if (tinst && tinst->memory) {
        if (return_size) {
            *return_size = tinst->memory_size;
        }
        return tinst->memory;
    }
    return NULL;
}

const gs_param_table_row_t * gs_param_table_get_rows(gs_param_table_instance_t * tinst, size_t * return_count)
{
    if (tinst && tinst->rows && tinst->row_count) {
        if (return_count) {
            *return_count = tinst->row_count;
        }
        return tinst->rows;
    }
    return NULL;
}

size_t gs_param_table_instance_size(void)
{
    return sizeof(gs_param_table_instance_t);
}

gs_param_table_instance_t * gs_param_table_instance_clear(void * var, size_t var_size)
{
    gs_param_table_instance_t * tinst = NULL;
    if (var && (var_size >= sizeof(*tinst))) {
        tinst = (gs_param_table_instance_t *) var;
        memset(tinst, 0, sizeof(*tinst));
    }
    return tinst;
}

gs_param_table_instance_t * gs_param_table_instance_alloc(void)
{
    return calloc(1, sizeof(gs_param_table_instance_t));
}

gs_error_t gs_param_get_string(gs_param_table_instance_t * tinst, uint16_t addr, char * buf, size_t buf_size, uint32_t flags)
{
    if (tinst == NULL) {
        return GS_ERROR_HANDLE;
    }
    if (buf == NULL) {
        return GS_ERROR_ARG;
    }

    const gs_param_table_row_t * param = gs_param_row_by_address(addr, tinst->rows, tinst->row_count);
    if (param == NULL) {
        return GS_ERROR_RANGE;
    }

    if (buf_size <= param->size) {
        return GS_ERROR_OVERFLOW;
    }

    gs_error_t error = gs_param_get(tinst, addr, param->type, buf, param->size, flags);
    buf[param->size] = 0;
    return error;
}

gs_error_t gs_param_set_string(gs_param_table_instance_t * tinst, uint16_t addr, const char * value, uint32_t flags)
{
    if (tinst == NULL) {
        return GS_ERROR_HANDLE;
    }
    if (value == NULL) {
        return GS_ERROR_ARG;
    }

    const gs_param_table_row_t * param = gs_param_row_by_address(addr, tinst->rows, tinst->row_count);
    if (param == NULL) {
        return GS_ERROR_RANGE;
    }

    const size_t len = strlen(value) + 1;
    if (len > GS_PARAM_SIZE(param)) {
        return GS_ERROR_OVERFLOW;
    }

    return gs_param_set(tinst, addr, param->type, value, len, flags); // flags have full control
}

gs_error_t gs_param_get_data(gs_param_table_instance_t * tinst, uint16_t addr, void * buf, size_t buf_size, uint32_t flags)
{
    if (tinst == NULL) {
        return GS_ERROR_HANDLE;
    }
    if (buf == NULL) {
        return GS_ERROR_ARG;
    }

    const gs_param_table_row_t * param = gs_param_row_by_address(addr, tinst->rows, tinst->row_count);
    if (param == NULL) {
        return GS_ERROR_RANGE;
    }

    if (buf_size < param->size) {
        return GS_ERROR_OVERFLOW;
    }

    return gs_param_get(tinst, addr, param->type, buf, param->size, flags);
}

gs_error_t gs_param_set_data(gs_param_table_instance_t * tinst, uint16_t addr, const void * value, size_t value_size, uint32_t flags)
{
    if (tinst == NULL) {
        return GS_ERROR_HANDLE;
    }
    if (value == NULL) {
        return GS_ERROR_ARG;
    }

    const gs_param_table_row_t * param = gs_param_row_by_address(addr, tinst->rows, tinst->row_count);
    if (param == NULL) {
        return GS_ERROR_RANGE;
    }

    if (value_size > GS_PARAM_SIZE(param)) {
        return GS_ERROR_OVERFLOW;
    }

    return gs_param_set(tinst, addr, param->type, value, value_size, flags); // flags have full control
}
