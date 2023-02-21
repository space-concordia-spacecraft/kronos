#ifndef GS_PARAM_INTERNAL_PP_I2C_I2C_H
#define GS_PARAM_INTERNAL_PP_I2C_I2C_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#if (GS_PARAM_INTERNAL_USE)

#include <gs/param/pp/i2c/i2c.h>
#include <gs/param/internal/pp/i2c/slave_dispatch.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GS_PARAM_I2C_LENGTH_TABLE(length, table)  ((length << 3) | (table & 0x07))
#define GS_PARAM_I2C_LENGTH_TABLE_TO_LENGTH(lt)   ((lt >> 3) & 0x1f)
#define GS_PARAM_I2C_LENGTH_TABLE_TO_TABLE(lt)    (lt & 0x07)

/**
   Data structure for setting parameter.
*/
typedef struct __attribute__ ((packed)) gs_param_i2c_set_request {
    gs_i2c_slave_dispatch_header_t header;
    uint8_t length_table;
    uint8_t addr;
    uint8_t data[]; // data (+ checksum)
} gs_param_i2c_set_request_t;

/**
   Data structure for getting parameter.
*/
typedef struct __attribute__ ((packed)) gs_param_i2c_get_request {
    gs_i2c_slave_dispatch_header_t header;
    uint8_t length_table;
    uint8_t addr;
    uint8_t checksum; // optional
} gs_param_i2c_get_request_t;

#ifdef __cplusplus
}
#endif
#endif
#endif
