#ifndef GS_UTIL_DRIVERS_SPI_MASTER_H
#define GS_UTIL_DRIVERS_SPI_MASTER_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   SPI master interface.
*/

#include <gs/util/drivers/spi/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Cross-platform master SPI configuration.
*/
typedef struct {
    /**
       Data order, \a True: MSB first, \a False: LSB first
       Default: \a True.
    */
    bool data_order_msb;
    /**
       Bits per second.
       Default: #GS_SPI_DEFAULT_BPS.
    */
    uint32_t bps;
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
} gs_spi_master_slave_config_t;

/**
   Single master transaction.
*/
typedef struct {
    /**
       Pointer to tx data, or NULL if no tx.
    */
    const void *tx;
    /**
       Pointer to rx buffer, or NULL if no rx.
    */
    void *rx;
    /**
       Size/length of rx/tx (bytes).
    */
    size_t size;
} gs_spi_master_trans_t;

/**
   Close/free slave.
   Freeing resources associated with the slave.
   @param[in] slave SPI slave
   @return_gs_error_t
*/
gs_error_t gs_spi_master_close_slave(uint8_t slave);

/**
   Perform transaction to/from a pre-configured SPI slave.
   Basically for i < size: send tx[i] and receive rx[i].
   @note: 8 bit SPI character size required!
   @param[in]  slave SPI slave
   @param[in]  tx tx buffer
   @param[out] rx rx buffer - can be NULL.
   @param[in]  size number of to send and also receive.
   @param[in]  timeout_ms timeout in milliseconds, primarily for locking the SPI device.
   @return_gs_error_t
*/
gs_error_t gs_spi_master_transaction(uint8_t slave, const void * tx, void * rx, size_t size, int timeout_ms);

/**
   Perform N transaction to/from a pre-configured SPI slave within one chip selection
   @note: 8 bit SPI character size required!
   @param[in] slave SPI slave
   @param[in] trans Pointer to transactions
   @param[in] count Number of transactions (rx and/or tx) to complete
   @param[in] timeout_ms timeout in milliseconds, primarily for locking the SPI device.
   @return_gs_error_t
*/
gs_error_t gs_spi_master_transactions(uint8_t slave, gs_spi_master_trans_t *trans, size_t count, int timeout_ms);

#ifdef __cplusplus
}
#endif
#endif
