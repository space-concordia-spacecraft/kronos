/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <stdio.h>
#include <gs/util/check.h>
#include <gs/util/linux/drivers/spi/spi.h>

#define MAX_DRIVERS 20

typedef struct {
    gs_spi_master_driver_entry_t entry;
    bool in_use;
} gs_spi_master_driver_handle_t;

typedef struct {
    gs_spi_slave_driver_entry_t entry;
    bool in_use;
} gs_spi_slave_driver_handle_t;

static gs_spi_master_driver_handle_t master_drivers[MAX_DRIVERS];
static gs_spi_slave_driver_handle_t slave_drivers[MAX_DRIVERS];

static uint8_t max_index_master_in_use = 0;
static uint8_t max_index_slave_in_use = 0;

static inline gs_spi_master_driver_entry_t * gs_master_find_driver_entry(uint8_t slave)
{
    gs_spi_master_driver_handle_t * handle;
    for (int i = max_index_master_in_use; i >= 0; i--) {
        handle = &master_drivers[i];
        if (((slave == handle->entry.slave) || (handle->entry.slave == GS_SPI_ALL_SLAVES)) && (handle->in_use == true)) {
            return &handle->entry;
        }
    }
    return NULL;
}

gs_error_t gs_spi_master_transaction(uint8_t slave, const void * tx, void * rx, size_t size, int timeout_ms)
{
    gs_spi_master_trans_t trans = {.tx = tx, .rx = rx, .size = size};
    return gs_spi_master_transactions(slave, &trans, 1, timeout_ms);
}

gs_error_t gs_spi_master_transactions(uint8_t slave, gs_spi_master_trans_t *trans, size_t count, int timeout_ms)
{
    gs_spi_master_driver_entry_t * driver_entry = gs_master_find_driver_entry(slave);
    if (driver_entry) {
        if (driver_entry->driver->master_transactions_handler) {
            return driver_entry->driver->master_transactions_handler(slave, trans, count, timeout_ms, driver_entry->driver_data);
        }
    }
    return GS_ERROR_NOT_FOUND;
}

gs_error_t gs_spi_master_register_driver(const gs_spi_master_driver_entry_t * driver_entry)
{
    GS_CHECK_ARG(driver_entry != NULL);
    GS_CHECK_ARG(driver_entry->driver != NULL);

    gs_spi_master_driver_handle_t * handle;
    for (uint8_t i = 0; i < MAX_DRIVERS; i++) {
        handle = &master_drivers[i];
        if (handle->in_use == false) {
            handle->entry = *driver_entry;
            handle->in_use = true;
            max_index_master_in_use = i;
            return GS_OK;
        }
    }

    /* Not enough space in buffer */
    return GS_ERROR_FULL;
}

static inline gs_spi_slave_driver_entry_t * gs_slave_find_driver_entry(uint8_t device)
{
    gs_spi_slave_driver_handle_t * handle;
    for (int i = max_index_slave_in_use; i >= 0; i--) {
        handle = &slave_drivers[i];
        if (((device == handle->entry.device) || (handle->entry.device == GS_SPI_ALL_DEVICES))
                        && (handle->in_use == true)) {
            return &handle->entry;
        }
    }
    return NULL;
}

gs_error_t gs_spi_slave_start(uint8_t device)
{
    gs_spi_slave_driver_entry_t * driver_entry = gs_slave_find_driver_entry(device);
    if (driver_entry) {
        if (driver_entry->driver->start_handler) {
            return driver_entry->driver->start_handler(device, driver_entry->driver_data);
        }
    }
    return GS_ERROR_NOT_FOUND;
}

gs_error_t gs_spi_slave_set_rx(uint8_t device, gs_spi_slave_receive_t rx)
{
    gs_spi_slave_driver_entry_t * driver_entry = gs_slave_find_driver_entry(device);
    if (driver_entry) {
        if (driver_entry->driver->set_rx_handler) {
            return driver_entry->driver->set_rx_handler(device, rx, driver_entry->driver_data);
        }
    }
    return GS_ERROR_NOT_FOUND;
}

gs_error_t gs_spi_slave_set_response(uint8_t device, size_t offset, const uint8_t * tx, size_t size)
{
    gs_spi_slave_driver_entry_t * driver_entry = gs_slave_find_driver_entry(device);
    if (driver_entry) {
        if (driver_entry->driver->set_response_handler) {
            return driver_entry->driver->set_response_handler(device, offset, tx, size, driver_entry->driver_data);
        }
    }
    return GS_ERROR_NOT_FOUND;
}

gs_error_t gs_spi_slave_register_driver(const gs_spi_slave_driver_entry_t * driver_entry)
{
    GS_CHECK_ARG(driver_entry != NULL);
    GS_CHECK_ARG(driver_entry->driver != NULL);

    gs_spi_slave_driver_handle_t * handle;
    for (uint8_t i = 0; i < MAX_DRIVERS; i++) {
        handle = &slave_drivers[i];
        if (handle->in_use == false) {
            handle->entry = *driver_entry;
            handle->in_use = true;
            max_index_slave_in_use = i;
            return GS_OK;
        }
    }
    /* Not enough space in buffer */
    return GS_ERROR_FULL;
}
