#ifndef GS_UTIL_DRIVERS_SPI_COMMON_H
#define GS_UTIL_DRIVERS_SPI_COMMON_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Common (master and slave) SPI definitions.
*/

#include <gs/util/log.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Default log group for SPI driver.
*/
GS_LOG_GROUP_EXTERN(gs_spi_log);

/**
   SPI mode - clock polarity and phase.
*/
typedef enum {
    /**
       Polarity = 0, Phase = 0 (default).
    */
    GS_SPI_MODE_CPOL0_CPHA0 = 0,
    /**
       Polarity = 0, Phase = 1.
    */
    GS_SPI_MODE_CPOL0_CPHA1 = 1,
    /**
       Polarity = 1, Phase = 0.
    */
    GS_SPI_MODE_CPOL1_CPHA0 = 2,
    /**
       Polarity = 1, Phase = 1.
    */
    GS_SPI_MODE_CPOL1_CPHA1 = 3
} gs_spi_mode_t;

/**
   Default bit-rate.
*/
#define GS_SPI_DEFAULT_BPS          400000

/**
   Speed (command line sub-option).
*/
#define GS_SPI_COMMAND_LINE_SPEED   "speed"

/**
   Slave (command line sub-option).
*/
#define GS_SPI_COMMAND_LINE_SLAVE   "slave"

/**
   Device (command line sub-option).
*/
#define GS_SPI_COMMAND_LINE_DEVICE  "device"

#ifdef __cplusplus
}
#endif
#endif
