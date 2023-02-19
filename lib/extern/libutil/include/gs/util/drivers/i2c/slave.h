#ifndef GS_UTIL_DRIVERS_I2C_SLAVE_H
#define GS_UTIL_DRIVERS_I2C_SLAVE_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file 

   I2C slave interface.
*/

#include <gs/util/drivers/i2c/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Start/enable I2C bus reception.

   Reception should not automatically be enabled by their init() functions, as this will complicate adding additional layers/hooks.

   @param[in] device I2C bus (handle)
   @return_gs_error_t
*/
gs_error_t gs_i2c_slave_start(uint8_t device);

/**
   Rx callback.

   Function called when data has been received on the bus (I2C write operation complete).

   @param[in] device I2C bus (handle).
   @param[in] rx receive buffer.
   @param[in] rx_length number of bytes received.
   @param_cswitch
*/
typedef void (* gs_i2c_slave_receive_t)(uint8_t device, const uint8_t * rx, size_t rx_length, gs_context_switch_t * cswitch);

/**
   Set rx callback.

   @param[in] device I2C bus (handle).
   @param[in] rx Rx callback.
   @return_gs_error_t
*/
gs_error_t gs_i2c_slave_set_rx(uint8_t device, gs_i2c_slave_receive_t rx);

/**
   Get rx buffer callback.

   Function called from driver, for getting a pointer to the rx buffer.

   @param[in] device I2C bus (handle).
*/
typedef void * (* gs_i2c_slave_get_rx_buf_t)(uint8_t device);

/**
   Set rx buffer get callback.

   @note: On avr32 platform, the maximum packet length that can be received is buf_length - 1.

   @param[in] device I2C bus (handle).
   @param[in] get_rx_buf get rx buffer callback.
   @param[in] buf_length length of buffer retrieved with this callback.
   @return_gs_error_t
*/
gs_error_t gs_i2c_slave_set_get_rx_buf(uint8_t device, gs_i2c_slave_get_rx_buf_t get_rx_buf, size_t buf_length);

/**
   Set response data.

   @param[in] device I2C bus (handle).
   @param[in] tx pointer to data. NOTE: data is not copied due to performance, so data must stay valid until the response has been sent.
   @param[in] tx_length length of data.
   @return_gs_error_t
*/
gs_error_t gs_i2c_slave_set_response(uint8_t device, const uint8_t * tx, size_t tx_length);

#ifdef __cplusplus
}
#endif
#endif
