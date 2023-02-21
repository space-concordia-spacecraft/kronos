/* Copyright (c) 2013-2019 GomSpace A/S. All rights reserved. */
#ifndef UTIL_LINUX_DRIVERS_SPI_SPI_H
#define UTIL_LINUX_DRIVERS_SPI_SPI_H
/**
   @file

   Linux SPI driver API. Support for registering linux version of gs_spi_master_transactions

   Linux plugin driver API is used
*/

#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
       Init driver to handle gs_spi_master_transactions on linux

       @param[in] slave_id  slave_id for future reference (integer between 0 and 10).
       @param[in] slave_config Slave configuration.
       @param[in] path Path to the linux bus device (e.g /dev/spidev0.x)

       @return_gs_error_t

    */
    gs_error_t gs_linux_spi_master_configure_slave(uint8_t slave_id,  const gs_spi_master_slave_config_t *slave_config,
                                                   const char *path);

#ifdef __cplusplus
}
#endif
#endif
