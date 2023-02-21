/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/param/pp/pp.h>
#include <gs/util/string.h>
#include <gs/util/check.h>
#include <gs/util/byteorder.h>
#include <gs/util/endian.h>

static inline bool gs_pp_endian_convert(gs_pp_t * pp)
{
    return (pp && (pp->big_endian != gs_endian_big()));
}

static gs_error_t gs_pp_get(gs_pp_t * pp, uint8_t table_id, uint16_t addr, void * value, size_t value_size, uint32_t flags)
{
    if (pp == NULL) {
        return GS_ERROR_HANDLE;
    }
    if (pp->get == NULL) {
        return GS_ERROR_NOT_IMPLEMENTED;
    }
    return pp->get(pp, table_id, addr, value, value_size, flags);
}

static gs_error_t gs_pp_set(gs_pp_t * pp, uint8_t table_id, uint16_t addr, const void * value, size_t value_size, uint32_t flags)
{
    if (pp == NULL) {
        return GS_ERROR_HANDLE;
    }
    if (pp->set == NULL) {
        return GS_ERROR_NOT_IMPLEMENTED;
    }
    return pp->set(pp, table_id, addr, value, value_size, flags);
}

uint8_t gs_pp_checksum8(const void * data_in, size_t length)
{
    const uint8_t * data = data_in;
    unsigned int checksum = 0;
    for (unsigned int i = 0; i < length; ++i) {
        checksum += *data++;
    }
    checksum &= 0xff;
    return checksum ? checksum : 1;
}

gs_error_t gs_pp_get_table_lock(gs_pp_t * pp, uint8_t table_id, bool * value, uint32_t flags)
{
    GS_CHECK_ARG(value != NULL);
    GS_CHECK_HANDLE(pp != NULL);
    if (pp->get_table_lock == NULL) {
        return GS_ERROR_NOT_IMPLEMENTED;
    }
    return pp->get_table_lock(pp, table_id, value, flags);
}

gs_error_t gs_pp_set_table_lock(gs_pp_t * pp, uint8_t table_id, const bool * value, uint32_t flags)
{
    GS_CHECK_ARG(value != NULL);
    GS_CHECK_HANDLE(pp != NULL);
    if (pp->set_table_lock == NULL) {
        return GS_ERROR_NOT_IMPLEMENTED;
    }
    return pp->set_table_lock(pp, table_id, value, flags);
}

// int8_t
gs_error_t gs_pp_get_int8(gs_pp_t * pp, uint8_t table_id, uint8_t addr, int8_t * value, size_t count, uint32_t flags)
{
    return gs_pp_get_uint8(pp, table_id, addr, (uint8_t *) value, count, flags);
}

gs_error_t gs_pp_set_int8(gs_pp_t * pp, uint8_t table_id, uint8_t addr, const int8_t * value, size_t count, uint32_t flags)
{
    return gs_pp_set_uint8(pp, table_id, addr, (const uint8_t *) value, count, flags);
}

// uint8_t

gs_error_t gs_pp_get_uint8(gs_pp_t * pp, uint8_t table_id, uint8_t addr, uint8_t * value, size_t count, uint32_t flags)
{
    GS_CHECK_ARG(value != NULL);
    GS_CHECK_ARG(count >  0);
    return gs_pp_get(pp, table_id, addr, value, (sizeof(*value) * count), flags);
}

gs_error_t gs_pp_set_uint8(gs_pp_t * pp, uint8_t table_id, uint8_t addr, const uint8_t * value, size_t count, uint32_t flags)
{
    GS_CHECK_ARG(value != NULL);
    GS_CHECK_ARG(count >  0);
    return gs_pp_set(pp, table_id, addr, value, (sizeof(*value) * count), flags);
}

// int16_t

gs_error_t gs_pp_get_int16(gs_pp_t * pp, uint8_t table_id, uint8_t addr, int16_t * value, size_t count, uint32_t flags)
{
    return gs_pp_get_uint16(pp, table_id, addr, (uint16_t *) value, count, flags);
}

gs_error_t gs_pp_set_int16(gs_pp_t * pp, uint8_t table_id, uint8_t addr, const int16_t * value, size_t count, uint32_t flags)
{
    return gs_pp_set_uint16(pp, table_id, addr, (const uint16_t *) value, count, flags);
}

// uint16_t

gs_error_t gs_pp_get_uint16(gs_pp_t * pp, uint8_t table_id, uint8_t addr, uint16_t * value, size_t count, uint32_t flags)
{
    GS_CHECK_ARG(value != NULL);
    GS_CHECK_ARG(count >  0);
    gs_error_t error = gs_pp_get(pp, table_id, addr, value, (sizeof(*value) * count), flags);
    if (gs_pp_endian_convert(pp)) {
        gs_bswap_16_array(value, value, count);
    }
    return error;
}

gs_error_t gs_pp_set_uint16(gs_pp_t * pp, uint8_t table_id, uint8_t addr, const uint16_t * value, size_t count, uint32_t flags)
{
    GS_CHECK_ARG(value != NULL);
    GS_CHECK_ARG(count >  0);
    uint16_t _converted[count];
    if (gs_pp_endian_convert(pp)) {
        gs_bswap_16_array(value, _converted, count);
        value = _converted;
    }
    return gs_pp_set(pp, table_id, addr, value, (sizeof(*value) * count), flags);
}

// int32_t

gs_error_t gs_pp_get_int32(gs_pp_t * pp, uint8_t table_id, uint8_t addr, int32_t * value, size_t count, uint32_t flags)
{
    return gs_pp_get_uint32(pp, table_id, addr, (uint32_t *) value, count, flags);
}

gs_error_t gs_pp_set_int32(gs_pp_t * pp, uint8_t table_id, uint8_t addr, const int32_t * value, size_t count, uint32_t flags)
{
    return gs_pp_set_uint32(pp, table_id, addr, (const uint32_t *) value, count, flags);
}

// uint32_t

gs_error_t gs_pp_get_uint32(gs_pp_t * pp, uint8_t table_id, uint8_t addr, uint32_t * value, size_t count, uint32_t flags)
{
    GS_CHECK_ARG(value != NULL);
    GS_CHECK_ARG(count >  0);
    gs_error_t error = gs_pp_get(pp, table_id, addr, value, (sizeof(*value) * count), flags);
    if (gs_pp_endian_convert(pp)) {
        gs_bswap_32_array(value, value, count);
    }
    return error;
}

gs_error_t gs_pp_set_uint32(gs_pp_t * pp, uint8_t table_id, uint8_t addr, const uint32_t * value, size_t count, uint32_t flags)
{
    GS_CHECK_ARG(value != NULL);
    GS_CHECK_ARG(count >  0);
    uint32_t _converted[count];
    if (gs_pp_endian_convert(pp)) {
        gs_bswap_32_array(value, _converted, count);
        value = _converted;
    }
    return gs_pp_set(pp, table_id, addr, value, (sizeof(*value) * count), flags);
}

gs_error_t gs_pp_get_float(gs_pp_t * pp, uint8_t table_id, uint8_t addr, float * value, size_t count, uint32_t flags)
{
    GS_CHECK_ARG(value != NULL);
    GS_CHECK_ARG(count >  0);
    gs_error_t error = gs_pp_get(pp, table_id, addr, value, (sizeof(*value) * count), flags);
    if (gs_pp_endian_convert(pp)) {
        gs_bswap_float_array(value, value, count);
    }
    return error;
}

gs_error_t gs_pp_set_float(gs_pp_t * pp, uint8_t table_id, uint8_t addr, const float * value, size_t count, uint32_t flags)
{
    GS_CHECK_ARG(value != NULL);
    GS_CHECK_ARG(count >  0);
    float _converted[count];
    if (gs_pp_endian_convert(pp)) {
        gs_bswap_float_array(value, _converted, count);
        value = _converted;
    }
    return gs_pp_set(pp, table_id, addr, value, (sizeof(*value) * count), flags);
}
