/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#define GS_PARAM_INTERNAL_USE 1

#include <gs/param/internal/pp/spi/spi.h>
#include <gs/util/drivers/spi/master.h>
#include <gs/util/string.h>
#include <gs/util/check.h>
#include <alloca.h>

static gs_error_t gs_pp_spi_get(gs_pp_t * pp, uint8_t table_id, uint16_t addr, void * value, size_t value_size, uint32_t flags)
{
    GS_CHECK_RANGE(table_id <= 7);
    GS_CHECK_RANGE(addr <= 255);

    if (flags & GS_PP_FLAG_CHECKSUM) {
        gs_param_spi_get_with_checksum_t * request;
        const size_t size = (sizeof(*request) + value_size + 1); // +1 for CHECKSUM in returned data
        request = alloca(size);
        memset(request, 0, size);
        request->header.domain_command = GS_SPI_SLAVE_DISPATCH_HEADER(GS_SPI_SLAVE_DISPATCH_DOMAIN_PARAM, GS_PARAM_SPI_COMMAND_GET_WITH_CHECKSUM);
        request->length_table = GS_PARAM_SPI_LENGTH_TABLE(value_size, table_id);
        request->addr = addr;
        request->checksum = gs_pp_checksum8(request, (sizeof(*request) - sizeof(request->filler)));

        gs_error_t error = gs_spi_master_transaction(pp->pp.spi.slave, request, request, size, 1000);
        if (error == GS_OK) {
            if (gs_pp_checksum8(request->data, value_size) != request->data[value_size]) {
                return GS_ERROR_DATA;
            }
            memcpy(value, request->data, value_size);
        }
        return error;

    } else {
        gs_param_spi_get_t * request;
        const size_t size = (sizeof(*request) + value_size);
        request = alloca(size);
        memset(request, 0, size);
        request->header.domain_command = GS_SPI_SLAVE_DISPATCH_HEADER(GS_SPI_SLAVE_DISPATCH_DOMAIN_PARAM, GS_PARAM_SPI_COMMAND_GET);
        request->length_table = GS_PARAM_SPI_LENGTH_TABLE(value_size, table_id);
        request->addr = addr;

        gs_error_t error = gs_spi_master_transaction(pp->pp.spi.slave, request, request, size, 1000);
        if (error == GS_OK) {
            memcpy(value, request->data, value_size);
        }
        return error;
    }
}

static gs_error_t gs_pp_spi_set(gs_pp_t * pp, uint8_t table_id, uint16_t addr, const void * value, size_t value_size, uint32_t flags)
{
    GS_CHECK_RANGE(table_id <= 7);
    GS_CHECK_RANGE(addr <= 255);

    gs_param_spi_set_t * request;
    const size_t size = (sizeof(*request) + value_size + ((flags & GS_PP_FLAG_CHECKSUM) ? 1 : 0));
    request = alloca(size);
    if (flags & GS_PP_FLAG_CHECKSUM) {
        request->header.domain_command = GS_SPI_SLAVE_DISPATCH_HEADER(GS_SPI_SLAVE_DISPATCH_DOMAIN_PARAM, GS_PARAM_SPI_COMMAND_SET_WITH_CHECKSUM);
    } else {
        request->header.domain_command = GS_SPI_SLAVE_DISPATCH_HEADER(GS_SPI_SLAVE_DISPATCH_DOMAIN_PARAM, GS_PARAM_SPI_COMMAND_SET);
    }
    request->length_table = GS_PARAM_SPI_LENGTH_TABLE(value_size, table_id);
    request->addr = addr;
    memcpy(request->data, value, value_size);

    if (flags & GS_PP_FLAG_CHECKSUM) {
        request->data[value_size] = gs_pp_checksum8(request, size - 1);
    }

    return gs_spi_master_transaction(pp->pp.spi.slave, request, NULL, size, 1000);
}

gs_error_t gs_pp_spi_init(gs_pp_t * pp, uint8_t slave, bool big_endian)
{
    GS_CHECK_HANDLE(pp != NULL);

    memset(pp, 0, sizeof(*pp));

    pp->get = gs_pp_spi_get;
    pp->set = gs_pp_spi_set;
    pp->set_table_lock = NULL; // Not implemented
    pp->get_table_lock = NULL; // Not implemented
    pp->big_endian = big_endian;

    pp->pp.spi.slave = slave;

    return GS_OK;
}
