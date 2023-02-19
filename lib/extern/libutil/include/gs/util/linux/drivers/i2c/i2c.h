#ifndef LIB_LIBUTIL_INCLUDE_GS_UTIL_LINUX_DRIVERS_I2C_I2C_H_
#define LIB_LIBUTIL_INCLUDE_GS_UTIL_LINUX_DRIVERS_I2C_I2C_H_
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   @brief Linux I2C plugin driver
*/

#include <gs/util/drivers/i2c/master.h>
#include <gs/util/drivers/i2c/slave.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   GomSpace linux driver I2C master transaction.

   @see 'gs/util/drivers/i2c/master.h'

   @param[in]  device I2C device
   @param[in]  addr I2C address
   @param[in]  tx tx buffer
   @param[in]  txlen bytes to be sent
   @param[out] rx rx buffer
   @param[in]  rxlen bytes to be received
   @param[in]  timeout_ms timeout in milliseconds
   @param[in]  driver_data data to specific driver
   @return_gs_error_t
 */
typedef gs_error_t (* gs_i2c_master_transaction_t)(uint8_t device, uint8_t addr, const void * tx, size_t txlen,
                                                  void * rx, size_t rxlen, int timeout_ms, void * driver_data);

/**
   GomSpace linux driver I2C slave start.

   @see 'gs/util/drivers/i2c/slave.h'

   @param[in] device I2C device
   @param[in] driver_data data to specific driver
   @return_gs_error_t
 */
typedef gs_error_t (* gs_i2c_slave_start_t)(uint8_t device, void * driver_data);

/**
   GomSpace linux driver I2C set rx callback

   @see 'gs/util/drivers/i2c/slave.h'

   @param[in] device I2C device
   @param[in] rx rx callback
   @param[in] driver_data data to specific driver
   @return_gs_error_t
 */
typedef gs_error_t (* gs_i2c_slave_set_rx_t)(uint8_t device, gs_i2c_slave_receive_t rx, void * driver_data);

/**
   GomSpace linux driver I2C slave set 'get_rx_buffer' callback.

   @see 'gs/util/drivers/i2c/slave.h'

   @param[in] device I2C device
   @param[in] get_rx_buf get_rx_buf callback
   @param[in] buf_length length of buffer received by calling callback
   @param[in] driver_data data to specific driver
   @return_gs_error_t
 */
typedef gs_error_t (* gs_i2c_slave_set_get_rx_buf_t)(uint8_t device, gs_i2c_slave_get_rx_buf_t get_rx_buf, size_t buf_length, void * driver_data);

/**
   GomSpace linux driver I2C slave set slave response.

   @see 'gs/util/drivers/i2c/slave.h'

   @param[in] device I2C device
   @param[in] tx tx buffer
   @param[in] tx_length bytes to be send
   @param[in] driver_data data to specific driver
   @return_gs_error_t
*/
typedef gs_error_t (* gs_i2c_slave_set_response_t)(uint8_t device, const uint8_t * tx, size_t tx_length, void * driver_data);

/**
   Every I2C device ([0 : 254]).
 */
#define GS_I2C_ALL_DEVICES   255

/**
   Every I2C address (0 : 127]).
 */
#define GS_I2C_ALL_ADDR      255

/**
   I2C master driver.
 */
typedef struct {
    /**
       Function for handling master transactions.
     */
    gs_i2c_master_transaction_t master_transaction_handler;
} gs_i2c_master_driver_t;


/**
   I2C master driver entry
 */
typedef struct {
    /**
       I2C device, to which the driver is used (if GS_I2C_ALL_DEVICES, then all devices uses this driver).
     */
    uint8_t device;
    /**
       I2C addr, to which the driver is used (if GS_I2C_ALL_ADDR, then all addr on given device  uses this driver).
     */
    uint8_t addr;
    /**
       I2C master driver.
     */
    const gs_i2c_master_driver_t * driver;
    /**
       Driver specific data.
     */
    void * driver_data;
} gs_i2c_master_driver_entry_t;


/**
   I2C slave driver
 */
typedef struct {
    /**
       Function for handling slave start.
     */
    gs_i2c_slave_start_t start_handler;
    /**
       Function for handling the 'setting of rx callback'.
     */
    gs_i2c_slave_set_rx_t set_rx_handler;
    /**
       Function for handling setting of an 'rx buff get' function.
     */
    gs_i2c_slave_set_get_rx_buf_t set_get_rx_buf_handler;
    /**
       Function for handling 'set response'.
     */
    gs_i2c_slave_set_response_t set_response_handler;
} gs_i2c_slave_driver_t;


/**
   I2C slave driver entry.
 */
typedef struct {
    /**
       I2C device, to which the driver is used (if GS_I2C_ALL_DEVICES, then all devices uses this driver).
     */
    uint8_t device;
    /**
       I2C slave driver.
     */
    const gs_i2c_slave_driver_t * driver;
    /**
       Driver specific data.
     */
    void * driver_data;
} gs_i2c_slave_driver_entry_t;


/**
   Register a master driver.

   A specific driver can be assigned to a specific address and device
   or it can be registered to every address on a device or every address on every device.

   The latest registered driver, which fit the device an address, is the one used.

   @param[in] driver_entry driver and configuration to be registered
   @return_gs_error_t
 */
gs_error_t gs_i2c_master_register_driver(const gs_i2c_master_driver_entry_t * driver_entry);

/**
   Register a slave driver

   A specific driver can be assigned to a specific device or a driver can be assigned to every device.

   The latest registered driver, which fit the device, is the one used.

   @param[in] driver_entry driver and configuration to be registered
   @return_gs_error_t
 */
gs_error_t gs_i2c_slave_register_driver(const gs_i2c_slave_driver_entry_t * driver_entry);

#ifdef __cplusplus
}
#endif
#endif
