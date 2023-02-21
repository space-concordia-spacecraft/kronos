/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <stdio.h>
#include <gs/util/check.h>
#include <gs/util/linux/drivers/i2c/i2c.h>

#define MAX_DRIVERS 20
#define HIGHEST_I2C_ADDR 127

typedef struct {
    gs_i2c_master_driver_entry_t entry;
    bool in_use;
} gs_i2c_master_driver_handle_t;

typedef struct {
    gs_i2c_slave_driver_entry_t entry;
    bool in_use;
} gs_i2c_slave_driver_handle_t;

static gs_i2c_master_driver_handle_t master_drivers[MAX_DRIVERS];
static gs_i2c_slave_driver_handle_t slave_drivers[MAX_DRIVERS];

static uint8_t max_index_master_in_use = 0;
static uint8_t max_index_slave_in_use = 0;

gs_error_t gs_i2c_master_transaction(uint8_t device, uint8_t addr, const void * tx,
                                     size_t txlen,
                                     void * rx,
                                     size_t rxlen,
                                     int timeout_ms)
{
    GS_CHECK_RANGE(addr <= HIGHEST_I2C_ADDR);
    gs_i2c_master_driver_handle_t * handle;
    for (int i = max_index_master_in_use; i >= 0; i--) {
        handle = &master_drivers[i];
        if (((device == handle->entry.device) || (handle->entry.device == GS_I2C_ALL_DEVICES))
                        && ((addr == handle->entry.addr) || (handle->entry.addr == GS_I2C_ALL_ADDR))
                        && (handle->in_use == true)) {
            if (handle->entry.driver->master_transaction_handler) {
                return handle->entry.driver->master_transaction_handler(device, addr, tx, txlen, rx, rxlen, timeout_ms, handle->entry.driver_data);
            }
        }
    }
    return GS_ERROR_NOT_FOUND;
}

gs_error_t gs_i2c_master_register_driver(const gs_i2c_master_driver_entry_t * driver_entry)
{
    GS_CHECK_ARG(driver_entry != NULL);
    GS_CHECK_ARG(driver_entry->driver != NULL);

    GS_CHECK_RANGE((driver_entry->addr == GS_I2C_ALL_ADDR) || (driver_entry->addr <= HIGHEST_I2C_ADDR));

    gs_i2c_master_driver_handle_t * handle;
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

static inline gs_i2c_slave_driver_entry_t * gs_slave_find_driver_entry(uint8_t device)
{
    gs_i2c_slave_driver_handle_t * handle;
    for (int i = max_index_slave_in_use; i >= 0; i--) {
        handle = &slave_drivers[i];
        if (((device == handle->entry.device) || (handle->entry.device == GS_I2C_ALL_DEVICES))
                        && (handle->in_use == true)) {
            return &handle->entry;
        }
    }
    return NULL;
}

gs_error_t gs_i2c_slave_start(uint8_t device)
{
    gs_i2c_slave_driver_entry_t * driver_entry = gs_slave_find_driver_entry(device);
    if (driver_entry) {
        if (driver_entry->driver->start_handler) {
            return driver_entry->driver->start_handler(device, driver_entry->driver_data);
        }
    }
    return GS_ERROR_NOT_FOUND;
}

gs_error_t gs_i2c_slave_set_rx(uint8_t device, gs_i2c_slave_receive_t rx)
{
    gs_i2c_slave_driver_entry_t * driver_entry = gs_slave_find_driver_entry(device);
    if (driver_entry) {
        if (driver_entry->driver->set_rx_handler) {
            return driver_entry->driver->set_rx_handler(device, rx, driver_entry->driver_data);
        }
    }
    return GS_ERROR_NOT_FOUND;
}

gs_error_t gs_i2c_slave_set_get_rx_buf(uint8_t device, gs_i2c_slave_get_rx_buf_t get_rx_buf, size_t buf_length)
{
    gs_i2c_slave_driver_entry_t * driver_entry = gs_slave_find_driver_entry(device);
    if (driver_entry) {
        if (driver_entry->driver->set_get_rx_buf_handler) {
            return driver_entry->driver->set_get_rx_buf_handler(device, get_rx_buf, buf_length, driver_entry->driver_data);
        }
    }
    return GS_ERROR_NOT_FOUND;
}

gs_error_t gs_i2c_slave_set_response(uint8_t device, const uint8_t * tx, size_t tx_length)
{
    gs_i2c_slave_driver_entry_t * driver_entry = gs_slave_find_driver_entry(device);
    if (driver_entry) {
        if (driver_entry->driver->set_response_handler) {
            return driver_entry->driver->set_response_handler(device, tx, tx_length, driver_entry->driver_data);
        }
    }
    return GS_ERROR_NOT_FOUND;
}

gs_error_t gs_i2c_slave_register_driver(const gs_i2c_slave_driver_entry_t * driver_entry)
{
    GS_CHECK_ARG(driver_entry != NULL);
    GS_CHECK_ARG(driver_entry->driver != NULL);

    gs_i2c_slave_driver_handle_t * handle;
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
