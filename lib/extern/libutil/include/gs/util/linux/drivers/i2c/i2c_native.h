/* Copyright (c) 2013-2019 GomSpace A/S. All rights reserved. */
#ifndef UTIL_LINUX_DRIVERS_I2C_I2C_NATIVE_H
#define UTIL_LINUX_DRIVERS_I2C_I2C_NATIVE_H
/**
   @file

   Linux I2C driver API. Support for registering linux version of gs_i2c_master_transaction

   Linux plugin driver API is used
*/

#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
       Init driver to handle gs_i2c_master_transaction on linux

       @param[in] device Device number for future reference (integer between 0 and 3)
       @param[in] path Path to the linux i2c bus to use for this instance (e.g /dev/i2c-0)

       @return_gs_error_t

    */
    gs_error_t gs_linux_i2c_init(uint8_t device, const char * path);

#ifdef __cplusplus
}
#endif
#endif
