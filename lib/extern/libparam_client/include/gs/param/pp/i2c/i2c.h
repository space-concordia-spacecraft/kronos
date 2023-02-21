#ifndef GS_PARAM_PP_I2C_I2C_H
#define GS_PARAM_PP_I2C_I2C_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   I2C param protocol client interface.
*/

#include <gs/param/pp/pp.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Commands.
*/
typedef enum {
    /**
       Set parameter.
    */
    GS_PARAM_I2C_COMMAND_SET = 1,
    /**
       Get parameter.
    */
    GS_PARAM_I2C_COMMAND_GET = 2,
    /**
       Lock table.
    */
    GS_PARAM_I2C_COMMAND_SET_LOCK_WITH_CHECKSUM = 3,
    /**
       Get table lock state.
    */
    GS_PARAM_I2C_COMMAND_GET_LOCK_WITH_CHECKSUM = 4,
    /**
       Set parameter with checksum.
    */
    GS_PARAM_I2C_COMMAND_SET_WITH_CHECKSUM = 10,
    /**
       Get parameter with checksum.
    */
    GS_PARAM_I2C_COMMAND_GET_WITH_CHECKSUM = 11,
} gs_param_i2c_command_t;

/**
   Initialize the param protocol handle for I2C.

   @param[in] pp handle.
   @param[in] bus bus to communicate on.
   @param[in] addr address of node.
   @param[in] big_endian \a true if slave is big endian. Used to convert to host endian.
   @return_gs_error_t
*/
gs_error_t gs_pp_i2c_init(gs_pp_t * pp, uint8_t bus, uint8_t addr, bool big_endian);

#ifdef __cplusplus
}
#endif
#endif
