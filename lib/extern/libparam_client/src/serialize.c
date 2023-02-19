/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#define GS_PARAM_INTERNAL_USE 1

#include "serialize_local.h"
#include <gs/param/table.h>
#include <gs/param/internal/types.h>
#include <gs/util/byteorder.h>
#include <gs/util/endian.h>
#include <alloca.h>
#include <string.h>
#include <stdio.h>

#include <param/param_types.h> // need PARM_X??? definitions

bool gs_param_betoh(gs_param_type_t type, void * item)
{
    if (item) {
        switch (type) {
            case GS_PARAM_UINT16:
            case GS_PARAM_INT16:
            case PARAM_X16:
                {
                    *(uint16_t *) item = util_betoh16(*(uint16_t *) item);
                    return true;
                }
            case GS_PARAM_UINT32:
            case GS_PARAM_INT32:
            case PARAM_X32:
                {
                    *(uint32_t *) item = util_betoh32(*(uint32_t *) item);
                    return true;
                }
            case GS_PARAM_UINT64:
            case GS_PARAM_INT64:
            case PARAM_X64:
                {
                    *(uint64_t *) item = util_betoh64(*(uint64_t *) item);
                    return true;
                }
            case GS_PARAM_FLOAT:
                {
                    *(float *) item = util_ntohflt(*(float *) item);
                    return true;
                }
            case GS_PARAM_DOUBLE:
                {
                    *(double *) item = util_ntohdbl(*(double *) item);
                    return true;
                }

            case GS_PARAM_UINT8:
            case GS_PARAM_INT8:
            case PARAM_X8:
            case GS_PARAM_STRING:
            case GS_PARAM_DATA:
            case GS_PARAM_BOOL:
                // no swap
                break;
        }
    }
    return false;
}

bool gs_param_htobe(gs_param_type_t type, void * item)
{
    if (item) {
        switch (type) {
            case PARAM_UINT16:
            case PARAM_INT16:
            case PARAM_X16:
                {
                    *(uint16_t *) item = util_htobe16(*(uint16_t *) item);
                    return true;
                }
            case PARAM_UINT32:
            case PARAM_INT32:
            case PARAM_X32:
                {
                    *(uint32_t *) item = util_htobe32(*(uint32_t *) item);
                    return true;
                }
            case PARAM_UINT64:
            case PARAM_INT64:
            case PARAM_X64:
                {
                    *(uint64_t *) item = util_htobe64(*(uint64_t *) item);
                    return true;
                }
            case PARAM_FLOAT:
                {
                    *(float *) item = util_htonflt(*(float *) item);
                    return true;
                }
            case PARAM_DOUBLE:
                {
                    *(double *) item = util_htondbl(*(double *) item);
                    return true;
                }

            case PARAM_UINT8:
            case PARAM_INT8:
            case PARAM_X8:
            case PARAM_STRING:
            case PARAM_DATA:
            case PARAM_BOOL:
                // no swap
                break;
        }
    }
    return false;
}

static bool gs_param_require_endian_swap(gs_param_type_t type)
{
    switch (type) {
        case PARAM_UINT16:
        case PARAM_INT16:
        case PARAM_X16:
        case PARAM_UINT32:
        case PARAM_INT32:
        case PARAM_X32:
        case PARAM_UINT64:
        case PARAM_INT64:
        case PARAM_X64:
        case PARAM_FLOAT:
        case PARAM_DOUBLE:
            // swap
            break;

        case PARAM_UINT8:
        case PARAM_INT8:
        case PARAM_X8:
        case PARAM_STRING:
        case PARAM_DATA:
        case PARAM_BOOL:
            // no swap
            return false;

        default:
            break;
    }
    return true;
}

static gs_error_t gs_param_serialize_array(gs_param_table_instance_t * tinst, const gs_param_table_row_t * param, uint32_t flags, uint8_t * buf, unsigned int buf_size, unsigned int * buf_pos)
{
    const unsigned int param_size = GS_PARAM_SIZE(param);
    const unsigned int param_array_size = GS_PARAM_ARRAY_SIZE(param);
    const gs_param_type_t param_type = GS_PARAM_TYPE(param);

    /* Calculate total parameter size (full array) */
    {
        unsigned int size = param_size * param_array_size;
        if ((flags & F_PACKED) == 0) {
            size += param_array_size * sizeof(uint16_t); // address
        }

        /* Return if parameter array would exceed maxbuf */
        if (*buf_pos + size > buf_size) {
            return GS_ERROR_OVERFLOW;
        }
    }

    uint8_t value[param_size];
    gs_error_t error = GS_OK;
    for (unsigned int j = 0; (j < param_array_size) && (error == GS_OK); j++) {
        const uint16_t addr = GS_PARAM_ADDR(param) + (param_size * j);
        error = gs_param_get(tinst, addr, param_type, value, param_size, 0);
        if (error == GS_OK) {
            error = gs_param_serialize_item(param, addr, value, flags, buf, buf_size, buf_pos);
        }
    }

    return error;
}

gs_error_t gs_param_serialize_item_direct(gs_param_type_t param_type, unsigned int param_size, uint16_t addr, const void * item, uint32_t flags, uint8_t * buf, unsigned int buf_size, unsigned int * buf_pos)
{
    /* Check length */
    if ((((flags & F_PACKED) ? 0 : sizeof(addr)) + param_size + *buf_pos) > buf_size) {
        return GS_ERROR_OVERFLOW;
    }

    /* Include address if not packed */
    if ((flags & F_PACKED) == 0) {

        if (flags & F_TO_BIG_ENDIAN) {
            addr = util_htobe16(addr);
        }

        if ((flags & F_DRY_RUN) == 0) {
            memcpy(&buf[*buf_pos], &addr, sizeof(addr));
        }

        *buf_pos += sizeof(addr);
    }

    if ((flags & F_DRY_RUN) == 0) {
        if (flags & F_TO_BIG_ENDIAN) {
            void * tmp = alloca(param_size); // this must be aligned
            memcpy(tmp, item, param_size);
            gs_param_htobe(param_type, tmp);
            item = tmp;
        }
        memcpy(&buf[*buf_pos], item, param_size);
    }

    *buf_pos += param_size;

    return GS_OK;
}

gs_error_t gs_param_serialize_item(const gs_param_table_row_t * param, uint16_t addr, const void * item, uint32_t flags, uint8_t * buf, unsigned int buf_size, unsigned int * buf_pos)
{
    const gs_param_type_t param_type = GS_PARAM_TYPE(param);
    const unsigned int param_size = GS_PARAM_SIZE(param);
    return gs_param_serialize_item_direct(param_type, param_size, addr, item, flags, buf, buf_size, buf_pos);
}

gs_error_t gs_param_serialize_full_table(gs_param_table_instance_t * tinst, unsigned int * param_pos, uint32_t flags, uint8_t * buf, unsigned int buf_size, unsigned int * buf_pos)
{
    if (flags & GS_PARAM_SF_DRY_RUN) {
        buf = NULL;
        buf_size = -1; // Max size
    }
    gs_error_t error = GS_OK;
    unsigned int i = *param_pos;
    for (; i < tinst->row_count; i++) {
        const gs_param_table_row_t * param = &tinst->rows[i];
        error = gs_param_serialize_array(tinst, param, flags, buf, buf_size, buf_pos);
        if (error) {
            break;
        }
    }
    *param_pos = i;
    return error;
}

gs_error_t gs_param_serialize_list(gs_param_table_instance_t * tinst,
                                   const uint16_t addr[], unsigned int addr_count,
                                   unsigned int * param_pos, uint32_t flags,
                                   uint8_t * buf, unsigned int buf_size, unsigned int * buf_pos)
{
    if (tinst == NULL) {
        return GS_ERROR_HANDLE;
    }

    gs_error_t error = GS_OK;
    unsigned int i = *param_pos;
    for (; i < addr_count; ++i) {
        const gs_param_table_row_t * param = gs_param_row_by_address(addr[i], tinst->rows, tinst->row_count);
        if (param == NULL) {
            continue;
        }

        const gs_param_type_t param_type = GS_PARAM_TYPE(param);
        const unsigned int param_size = GS_PARAM_SIZE(param);
        uint8_t value[param_size];
        error = gs_param_get(tinst, addr[i], param_type, value, param_size, 0);
        if (error) {
            break;
        }
        error = gs_param_serialize_item(param, addr[i], value, flags, buf, buf_size, buf_pos);
        if (error) {
            break;
        }
    }
    *param_pos = i;

    return error;
}

gs_error_t gs_param_deserialize_item(gs_param_table_instance_t * tinst,
                                     const gs_param_table_row_t * param,
                                     uint16_t addr,
                                     const void * item,
                                     uint32_t flags)
{
    const gs_param_type_t param_type = GS_PARAM_TYPE(param);
    const unsigned int param_size = GS_PARAM_SIZE(param);

    if (flags & F_FROM_BIG_ENDIAN) {
        if (gs_param_require_endian_swap(param_type)) {

            // Copy to temporary storage, so we don't mess with input memory
            void * tmp = alloca(param_size);
            memcpy(tmp, item, param_size);

            gs_param_betoh(param_type, tmp);

            // Replace input pointer
            item = tmp;
        }
    }

    gs_error_t error = GS_OK;
    if ((flags & F_DRY_RUN) == 0) {
        error = gs_param_set(tinst, addr, param_type, item, param_size, GS_PARAM_FLAGS(param));
    }

    return error;
}

gs_error_t gs_param_deserialize(gs_param_table_instance_t * tinst, const uint8_t * buf, unsigned int buf_size, uint32_t flags)
{
    unsigned int pos = 0;
    unsigned int count = 0;
    gs_error_t error = GS_OK;
    while ((pos < buf_size) && (error == GS_OK)) {

        if (flags & F_PACKED) {
            /** PACKED */

            /* Find in table */
            const gs_param_table_row_t * param = &tinst->rows[count];
            const unsigned int param_array_size = GS_PARAM_ARRAY_SIZE(param);
            const unsigned int param_size = GS_PARAM_SIZE(param);

            /* For each item in array */
            for (unsigned int j = 0; (j < param_array_size) && (error == GS_OK); j++) {
                uint16_t addr = GS_PARAM_ADDR(param) + (param_size * j);
                error = gs_param_deserialize_item(tinst, param, addr, &buf[pos], flags);
                pos += param_size;
            }

        } else {
            /** NOT PACKED */

            /* Read address from data */
            uint16_t addr;
            memcpy(&addr, &buf[pos], sizeof(addr));
            if (flags & F_FROM_BIG_ENDIAN) {
                addr = util_betoh16(addr);
            }
            pos += sizeof(addr);

            /* Find in table */
            const gs_param_table_row_t * param = gs_param_row_by_address(addr, tinst->rows, tinst->row_count);
            if (param == NULL) {
                return GS_ERROR_NOT_FOUND;
            }

            /* Copy value */
            error = gs_param_deserialize_item(tinst, param, addr, &buf[pos], flags);
            pos += GS_PARAM_SIZE(param);
        }

        count++;
    }

    return error;
}
