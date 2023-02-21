/* Copyright (c) 2013-2019 GomSpace A/S. All rights reserved. */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <gs/util/check.h>
#include <gs/util/linux/drivers/spi/spi.h>
#include <gs/util/mutex.h>
#include <gs/util/log.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <linux/spi/spidev.h>


#define GS_MAX_SPI_SLAVES 10
typedef struct {
    gs_spi_master_driver_entry_t master_entry;
    bool in_use;
    gs_mutex_t lock;
    int handle;
}gs_linux_spi_slave_t;

gs_linux_spi_slave_t gs_linux_spi_slaves[GS_MAX_SPI_SLAVES];

static gs_error_t gs_linux_spi_master_transactions(uint8_t slave, gs_spi_master_trans_t *trans, size_t count,
                                            int timeout_ms, void * driver_data)
{
    gs_linux_spi_slave_t *dev = (gs_linux_spi_slave_t*)driver_data;
    GS_CHECK_ARG(slave < GS_MAX_SPI_SLAVES);
    if (!dev->in_use) {
        return GS_ERROR_ARG;
    }
    gs_error_t err = gs_mutex_lock(dev->lock);
    if (err != GS_OK) {
        return err;
    }
    struct spi_ioc_transfer transfer[count];
    memset(transfer, 0, sizeof(struct spi_ioc_transfer)*count);
    for (unsigned int i = 0; i < count; i++) {
        transfer[i].tx_buf   = (unsigned long)trans[i].tx;
        transfer[i].rx_buf   = (unsigned long)trans[i].rx;
        transfer[i].len      = trans[i].size;
    }
    if (ioctl(dev->handle, SPI_IOC_MESSAGE(count), &transfer[0]) < 0) {
        log_error("SPI message transfer failed %s", strerror(errno));
        gs_mutex_unlock(dev->lock);
        return GS_ERROR_IO;
    }
    gs_mutex_unlock(dev->lock);
    return GS_OK;
}

static const gs_spi_master_driver_t master_driver = {.master_transactions_handler = gs_linux_spi_master_transactions};

gs_error_t gs_linux_spi_master_configure_slave(uint8_t slave_id, const gs_spi_master_slave_config_t *slave_config,
                                               const char *path)
{
    GS_CHECK_ARG(slave_id < GS_MAX_SPI_SLAVES);
    gs_linux_spi_slave_t *dev = &gs_linux_spi_slaves[slave_id];
    if (dev->in_use) {
        return GS_ERROR_IN_USE;
    }
    gs_error_t err = gs_mutex_create(&dev->lock);
    if (err != GS_OK) {
        return err;
    }

    dev->in_use = true;

    dev->handle = open(path, O_RDWR);
    if (dev->handle < 0) {
        log_error("Failed to open device %s code %s", path, strerror(errno));
        gs_mutex_destroy(dev->lock);
        dev->in_use = false;
        return GS_ERROR_IO;
    }

    dev->master_entry.slave = slave_id;
    dev->master_entry.driver = &master_driver;
    dev->master_entry.driver_data = dev;

    if (ioctl(dev->handle, SPI_IOC_WR_MODE, &slave_config->mode) < 0){
        log_error("Failed to set spi mode");
    }
    bool lsb_first = !slave_config->data_order_msb;
    if (ioctl(dev->handle, SPI_IOC_WR_LSB_FIRST, &lsb_first) < 0){
        log_error("Failed to set data order");
    }

    if (ioctl(dev->handle, SPI_IOC_WR_BITS_PER_WORD, &slave_config->bits) < 0){
        log_error("Failed to set Bits per word");
    }
    if (ioctl(dev->handle, SPI_IOC_WR_MAX_SPEED_HZ, &slave_config->bps) < 0){
        log_error("Failed to set max speed\r\n");
    }

    err = gs_spi_master_register_driver(&dev->master_entry);
    return err;
}
