#ifndef GS_UTIL_DRIVERS_I2C_MASTER_H
#define GS_UTIL_DRIVERS_I2C_MASTER_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   I2C master interface.
*/

#include <gs/util/drivers/i2c/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Perform transaction to I2C slave.
   @param[in]  device hardware device (bus)
   @param[in]  addr slave address
   @param[in]  tx transmit buffer
   @param[in]  txlen number of bytes to transmit
   @param[out] rx receive buffer - can be NULL.
   @param[in]  rxlen number of bytes to receive.
   @param[in]  timeout_ms timeout in milliseconds, primarily for locking the I2C channel.
   @return_gs_error_t
*/
gs_error_t gs_i2c_master_transaction(uint8_t device, uint8_t addr, const void * tx, size_t txlen, void * rx, size_t rxlen, int timeout_ms);
    
#ifdef __cplusplus
}
#endif
#endif
