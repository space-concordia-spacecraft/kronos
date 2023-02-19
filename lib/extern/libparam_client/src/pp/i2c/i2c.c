/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#define GS_PARAM_INTERNAL_USE 1

#include <gs/param/internal/pp/i2c/i2c.h>
#include <gs/util/drivers/i2c/master.h>
#include <gs/util/string.h>
#include <gs/util/check.h>
#include <alloca.h>

static gs_error_t gs_pp_i2c_write(gs_param_i2c_command_t cmd, gs_pp_t * pp, uint8_t table_id, uint16_t addr, void * value, size_t value_size, bool checksum)
{
    GS_CHECK_RANGE(table_id <= 7);
    GS_CHECK_RANGE(addr <= 255);
    GS_CHECK_RANGE((value_size > 0) && (value_size <= 31));

    gs_param_i2c_set_request_t * request;
    const size_t size = (sizeof(*request) + value_size + (checksum ? 1 : 0));
    request = alloca(size);

    request->header.domain_command = GS_I2C_SLAVE_DISPATCH_HEADER(GS_I2C_SLAVE_DISPATCH_DOMAIN_PARAM, cmd);
    request->length_table = GS_PARAM_I2C_LENGTH_TABLE(value_size, table_id);
    request->addr = addr;
    memcpy(request->data, value, value_size);

    if (checksum) {
        request->data[value_size] = gs_pp_checksum8(request, size - 1);
    }
    return gs_i2c_master_transaction(pp->pp.i2c.bus, pp->pp.i2c.addr, request, size, NULL, 0, 1000);

}

static gs_error_t gs_pp_i2c_read(gs_param_i2c_command_t cmd, gs_pp_t * pp, uint8_t table_id, uint16_t addr, void * value, size_t value_size, bool checksum)
{
    GS_CHECK_RANGE(table_id <= 7);
    GS_CHECK_RANGE(addr <= 255);
    GS_CHECK_RANGE((value_size > 0) && (value_size <= 31));

    gs_param_i2c_get_request_t request;
    memset(&request, 0, sizeof(request));
    request.length_table = GS_PARAM_I2C_LENGTH_TABLE(value_size, table_id);
    request.addr = addr;
    size_t request_size;

    uint8_t reply[value_size + sizeof(request.checksum)]; // + for checksum
    memset(reply, 0, sizeof(reply));
    size_t reply_size;

    request.header.domain_command = GS_I2C_SLAVE_DISPATCH_HEADER(GS_I2C_SLAVE_DISPATCH_DOMAIN_PARAM, cmd);

    if (checksum) {
        request.checksum = gs_pp_checksum8(&request, (sizeof(request) - sizeof(request.checksum)));
        request_size = sizeof(request);
        reply_size = sizeof(reply);
    } else {
        request_size = sizeof(request) - sizeof(request.checksum);
        reply_size = sizeof(reply) - sizeof(request.checksum);
    }

    gs_error_t error = gs_i2c_master_transaction(pp->pp.i2c.bus, pp->pp.i2c.addr, &request, request_size, reply, reply_size, 1000);
    if (error == GS_OK) {
        if (checksum) {
            if (gs_pp_checksum8(reply, value_size) != reply[value_size]) {
                return GS_ERROR_DATA;
            }
        }
        memcpy(value, reply, value_size);
    }
    return error;

}

static gs_error_t gs_pp_i2c_get(gs_pp_t * pp, uint8_t table_id, uint16_t addr, void * value, size_t value_size, uint32_t flags)
{
    gs_param_i2c_command_t cmd;
    bool checksum = false;
    if (flags & GS_PP_FLAG_CHECKSUM) {
        checksum = true;
        cmd = GS_PARAM_I2C_COMMAND_GET_WITH_CHECKSUM;
    } else {
        cmd = GS_PARAM_I2C_COMMAND_GET;
    }
    return gs_pp_i2c_read(cmd, pp, table_id, addr, value, value_size, checksum);
}

static gs_error_t gs_pp_i2c_set(gs_pp_t * pp, uint8_t table_id, uint16_t addr, const void * value, size_t value_size, uint32_t flags)
{
    gs_param_i2c_command_t cmd;
    bool checksum = false;
    if (flags & GS_PP_FLAG_CHECKSUM) {
        checksum = true;
        cmd = GS_PARAM_I2C_COMMAND_SET_WITH_CHECKSUM;
    } else {
        cmd = GS_PARAM_I2C_COMMAND_SET;
    }
    return gs_pp_i2c_write(cmd, pp, table_id, addr, (void *)value, value_size, checksum);
}

static gs_error_t gs_pp_i2c_set_table_lock(gs_pp_t * pp, uint8_t table_id, const bool * value, uint32_t flags)
{
    gs_param_i2c_command_t cmd = GS_PARAM_I2C_COMMAND_SET_LOCK_WITH_CHECKSUM;
    bool checksum = true;
    return gs_pp_i2c_write(cmd, pp, table_id, 0, (void *)value, 1, checksum);
}

static gs_error_t gs_pp_i2c_get_table_lock(gs_pp_t * pp, uint8_t table_id, bool * value, uint32_t flags)
{
    gs_param_i2c_command_t cmd = GS_PARAM_I2C_COMMAND_GET_LOCK_WITH_CHECKSUM;
    bool checksum = true;
    return gs_pp_i2c_read(cmd, pp, table_id, 0, value, 1, checksum);
}

gs_error_t gs_pp_i2c_init(gs_pp_t * pp, uint8_t bus, uint8_t addr, bool big_endian)
{
    GS_CHECK_HANDLE(pp != NULL);

    memset(pp, 0, sizeof(*pp));

    pp->get = gs_pp_i2c_get;
    pp->set = gs_pp_i2c_set;
    pp->set_table_lock = gs_pp_i2c_set_table_lock;
    pp->get_table_lock = gs_pp_i2c_get_table_lock;
    pp->big_endian = big_endian;

    pp->pp.i2c.bus = bus;
    pp->pp.i2c.addr = addr;

    return GS_OK;
}
