#ifndef GS_UTIL_DRIVERS_SPI_SLAVE_H
#define GS_UTIL_DRIVERS_SPI_SLAVE_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   SPI slave interface.
*/

#include <gs/util/drivers/spi/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Cross-platform slave SPI configuration.
*/
typedef struct {
    /**
       Data order, \a True: MSB first, \a False: LSB first
       Default: \a True.
    */
    bool data_order_msb;
    /**
       Mode, specifying polarity and phase.
       Default: #GS_SPI_MODE_CPOL0_CPHA0.
    */
    gs_spi_mode_t mode;
    /**
       Character size in bits, 8-16 bits.
       Default: 8 bits (prefered).
    */
    uint8_t bits;
} gs_spi_slave_config_t;

/**
   Start/enable SPI device reception.

   Reception should not automatically be enabled by their init() functions, as this will complicate adding additional layers/hooks.

   @param[in] device SPI device (handle)
   @return_gs_error_t
*/
gs_error_t gs_spi_slave_start(uint8_t device);

/**
   Rx callback.

   Function called as data is recevied on the device.

   @param[in] device SPI device (handle).
   @param[in] rx_buffer Pointer to start of rx buffer.
   @param[in] rx number of bytes received so far.
   @param[in] new_request \a true on the first callback of new data, \a false on receiving additional data during same \a chip-select. Can be used to bring receiver back in sync with new request.
   @param_cswitch
   @return total number of bytes to receive before next call back. Return 0 to ignore rest of data - no additional call backs will be done for current SPI transaction.
*/
typedef uint8_t (* gs_spi_slave_receive_t)(uint8_t device, const uint8_t * rx_buffer, size_t rx, bool new_request, gs_context_switch_t * cswitch);

/**
   Set rx callback.

   @param[in] device SPI device (handle).
   @param[in] rx Rx callback.
   @return_gs_error_t
*/
gs_error_t gs_spi_slave_set_rx(uint8_t device, gs_spi_slave_receive_t rx);

/**
   Set response data.

   @param[in] device SPI device (handle).
   @param[in] offset offset (in bytes) for the response, counted from start of request, i.e. offset of 2 means data will be sent as the 3rd byte.
   @param[in] tx pointer to data. NOTE: data is not copied due to performance, so data must stay valid until the response has been sent.
   @param[in] size size of data.
   @return_gs_error_t
*/
gs_error_t gs_spi_slave_set_response(uint8_t device, size_t offset, const uint8_t * tx, size_t size);

#ifdef __cplusplus
}
#endif
#endif
