#ifndef GS_UTIL_DRIVERS_I2C_COMMON_H
#define GS_UTIL_DRIVERS_I2C_COMMON_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Common (master and slave) I2C definitions.
*/

#include <gs/util/log.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Default log group for I2C driver.
*/
GS_LOG_GROUP_EXTERN(gs_i2c_log);

/**
   I2C mode.
*/
typedef enum {
    //! Master mode
    GS_I2C_MASTER = 0,
    //! Multimaster mode
    GS_I2C_MULTI_MASTER = 1,
    //! Slave mode
    GS_I2C_SLAVE = 2,
} gs_i2c_mode_t;

/**
   Cross-platform I2C configuration.
*/
typedef struct {
    //! Data order, True: MSB first, False: LSB first (default = True)
    bool data_order_msb;
    //! Device mode (master, multimaster, or slave)
    gs_i2c_mode_t mode;
    //! Address of node in multimaster and slave mode (not used in master mode)
    uint16_t addr;
    //! Bits per second (default/standard is #GS_I2C_DEFAULT_BPS_v2, legacy default is #GS_I2C_DEFAULT_BPS)
    uint32_t bps;
    //! Address size in bits, 7, 8 or 10 bits (default/standard is #GS_I2C_DEFAULT_ADDRESS_SIZE)
    uint8_t addrbits;
} gs_i2c_config_t;

/**
   Cross-platform I2C configuration.
   @deprecated use gs_i2c_config_t.
*/
typedef gs_i2c_config_t gs_i2c_bus_config_t;

/**
   Default bit-rate (legacy).
   @see #GS_I2C_DEFAULT_BPS_v2
*/
#define GS_I2C_DEFAULT_BPS             100000

/**
   Default bit-rate - version 2.
   New default bit rate, should be used instead of #GS_I2C_DEFAULT_BPS.
*/
#define GS_I2C_DEFAULT_BPS_v2          400000

/**
   Default address size.
*/
#define GS_I2C_DEFAULT_ADDRESS_SIZE    7

/**
   Default data order (MSB).
*/
#define GS_I2C_DEFAULT_DATA_ORDER_MSB  1

/**
   Speed (command line sub-option).
*/
#define GS_I2C_COMMAND_LINE_SPEED      "speed"

/**
   Device (command line sub-option).
*/
#define GS_I2C_COMMAND_LINE_DEVICE     "device"

/**
   Address (command line sub-option).
*/
#define GS_I2C_COMMAND_LINE_ADDRESS    "address"

#ifdef __cplusplus
}
#endif
#endif
