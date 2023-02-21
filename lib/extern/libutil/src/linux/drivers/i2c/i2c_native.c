/* Copyright (c) 2013-2019 GomSpace A/S. All rights reserved. */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <gs/util/check.h>
#include <gs/util/linux/drivers/i2c/i2c.h>
#include <gs/util/mutex.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#define GS_MAX_I2C_DEVICES 4
static struct {
    bool in_use;
    int handle;
    gs_mutex_t lock;
} gs_i2c_devices[GS_MAX_I2C_DEVICES];

static gs_error_t gs_linux_i2c_master_transaction(uint8_t device, uint8_t addr, const void * tx, size_t txlen, void * rx, size_t rxlen,
                                           int timeout_ms, void* driver_data)
{
    GS_CHECK_ARG(device < GS_MAX_I2C_DEVICES);
    if (!gs_i2c_devices[device].in_use) {
        return GS_ERROR_ARG;
    }
    gs_error_t err = gs_mutex_lock(gs_i2c_devices[device].lock);
    if (err != GS_OK) {
        return err;
    }
    uint8_t msgs = 0;
    struct i2c_msg msg[2];
    if (tx) {
        msg[msgs].addr = addr;
        msg[msgs].flags = 0;
        msg[msgs].len = txlen;
        msg[msgs].buf = (void*)tx;
        msgs++;
    }
    if (rx) {
        msg[msgs].addr = addr;
        msg[msgs].flags = I2C_M_RD;
        msg[msgs].len = rxlen;
        msg[msgs].buf = rx;
        msgs++;
    }
    struct i2c_rdwr_ioctl_data trans = {&msg[0], msgs};
    ioctl(gs_i2c_devices[device].handle, I2C_TIMEOUT, timeout_ms/10);
    if (ioctl(gs_i2c_devices[device].handle, I2C_RDWR, &trans) < 0) {
        gs_mutex_unlock(gs_i2c_devices[device].lock);
        return GS_ERROR_IO;
    }
    gs_mutex_unlock(gs_i2c_devices[device].lock);
    return GS_OK;
}

gs_error_t gs_linux_i2c_init(uint8_t device, const char * path)
{
    static bool driver_registered = false;
    GS_CHECK_ARG(device < GS_MAX_I2C_DEVICES);
    if (gs_i2c_devices[device].in_use) {
        return GS_ERROR_IN_USE;
    }
    gs_error_t err = gs_mutex_create(&gs_i2c_devices[device].lock);
    if (err != GS_OK) {
        return err;
    }
    gs_i2c_devices[device].in_use = true;
    gs_i2c_devices[device].handle = open(path, O_RDWR);
    if (gs_i2c_devices[device].handle < 0) {
        gs_mutex_destroy(gs_i2c_devices[device].lock);
        gs_i2c_devices[device].in_use = false;
        return GS_ERROR_IO;
    }
    ioctl(gs_i2c_devices[device].handle, I2C_RETRIES, 0);

    if (!driver_registered) {
        static const gs_i2c_master_driver_t driver = {.master_transaction_handler = gs_linux_i2c_master_transaction};
        static const gs_i2c_master_driver_entry_t master_entry = {.device = GS_I2C_ALL_DEVICES, .addr = GS_I2C_ALL_ADDR,
                                                                  .driver = &driver, .driver_data = NULL};
        driver_registered = true;

        err = gs_i2c_master_register_driver(&master_entry);
    }
    return err;
}
