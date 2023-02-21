#ifndef LIB_LIBUTIL_INCLUDE_GS_UTIL_LINUX_DRIVERS_SPI_SPI_H_
#define LIB_LIBUTIL_INCLUDE_GS_UTIL_LINUX_DRIVERS_SPI_SPI_H_
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Linux SPI plugin driver
*/

#include <gs/util/drivers/spi/master.h>
#include <gs/util/drivers/spi/slave.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Linux driver SPI master transactions.

   @see 'gs/util/drivers/spi/master.h'

   @param[in] slave SPI slave
   @param[in] trans Pointer to transactions
   @param[in] count Number of transactions (rx and/or tx) to complete
   @param[in] timeout_ms timeout in milliseconds, primarily for locking the SPI device.
   @param[in] driver_data data to specific driver
   @return_gs_error_t
 */
typedef gs_error_t (*gs_spi_master_transactions_t)(uint8_t slave, gs_spi_master_trans_t *trans, size_t count,
                                                   int timeout_ms, void * driver_data);

/**
   Linux driver SPI slave start.

   @see 'gs/util/drivers/spi/slave.h'

   @param[in] device SPI device (handle)
   @param[in] driver_data data to specific driver
   @return_gs_error_t
 */
typedef gs_error_t (* gs_spi_slave_start_t)(uint8_t device, void * driver_data);

/**
   Linux driver SPI set rx callback

   @see 'gs/util/drivers/spi/slave.h'

   @param[in] device SPI device (handle).
   @param[in] rx Rx callback.
   @param[in] driver_data data to specific driver
   @return_gs_error_t
 */
typedef gs_error_t (* gs_spi_slave_set_rx_t)(uint8_t device, gs_spi_slave_receive_t rx, void * driver_data);

/**
   Linux driver SPI slave set slave response.

   @see 'gs/util/drivers/spi/slave.h'

   @param[in] device SPI device (handle).
   @param[in] offset offset (in bytes) for the response, counted from start of request, i.e. offset of 2 means data will be sent as the 3rd byte.
   @param[in] tx pointer to data. NOTE: data is not copied due to performance, so data must stay valid until the response has been sent.
   @param[in] size size of data.
   @param[in] driver_data data to specific driver
   @return_gs_error_t
 */
typedef gs_error_t (* gs_spi_slave_set_response_t)(uint8_t device, size_t offset, const uint8_t * tx, size_t size, void * driver_data);

/**
   Every SPI slave ([0 : 254]).
 */
#define GS_SPI_ALL_SLAVES    255

/**
   Every SPI device (0 : 254]).
 */
#define GS_SPI_ALL_DEVICES   255


/**
   SPI master driver.
 */
typedef struct {
    /**
       Function for handling master transactions.
     */
    gs_spi_master_transactions_t master_transactions_handler;
} gs_spi_master_driver_t;


/**
   SPI master driver entry
 */
typedef struct {
    /**
       SPI slave, to which the driver is used (if #GS_SPI_ALL_SLAVES, then all slaves uses this driver).
     */
    uint8_t slave;
    /**
       SPI master driver.
     */
    const gs_spi_master_driver_t * driver;
    /**
       Driver specific data.
     */
    void * driver_data;
} gs_spi_master_driver_entry_t;


/**
   SPI slave driver
 */
typedef struct {
    /**
       Function for handling slave start.
     */
    gs_spi_slave_start_t start_handler;
    /**
       Function for handling the 'setting of rx callback'.
     */
    gs_spi_slave_set_rx_t set_rx_handler;
    /**
       Function for handling 'set response'.
     */
    gs_spi_slave_set_response_t set_response_handler;
} gs_spi_slave_driver_t;


/**
   SPI slave driver entry.
 */
typedef struct {
    /**
       SPI device, to which the driver is used (if #GS_SPI_ALL_DEVICES, then all devices uses this driver).
     */
    uint8_t device;
    /**
       SPI slave driver.
     */
    const gs_spi_slave_driver_t * driver;
    /**
       Driver specific data.
     */
    void * driver_data;
} gs_spi_slave_driver_entry_t;


/**
   Register a master driver.

   A specific driver can be assigned to a slave or it can be assigned to every slave.

   The latest registered driver, which fit the slave, is the one used.

   @param[in] driver_entry driver and configuration to be registered
   @return_gs_error_t
 */
gs_error_t gs_spi_master_register_driver(const gs_spi_master_driver_entry_t * driver_entry);

/**
   Register a slave driver

   A specific driver can be assigned to a specific device or a driver can be assigned to every device.

   The latest registered driver, which fit the device, is the one used.

   @param[in] driver_entry driver and configuration to be registered
   @return_gs_error_t
 */
gs_error_t gs_spi_slave_register_driver(const gs_spi_slave_driver_entry_t * driver_entry);

#ifdef __cplusplus
}
#endif
#endif
