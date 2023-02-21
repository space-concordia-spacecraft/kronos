#ifndef GS_PARAM_INTERNAL_PP_I2C_SLAVE_DISPATCH_H
#define GS_PARAM_INTERNAL_PP_I2C_SLAVE_DISPATCH_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#if (GS_PARAM_INTERNAL_USE)

#include <gs/util/drivers/i2c/slave.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Header for I2C slave dispatch protocol - must be first in all protocols.
*/
typedef struct __attribute__ ((packed)) gs_i2c_slave_dispatch_header {
    uint8_t domain_command;
} gs_i2c_slave_dispatch_header_t;

/**
   Make header from domain and command.
*/
#define GS_I2C_SLAVE_DISPATCH_HEADER(domain, command)   ((domain << 5) | (command & 0x1f))

/**
   Extract domain from header.
*/
#define GS_I2C_SLAVE_DISPATCH_HEADER_TO_DOMAIN(header)  ((header >> 5) & 0x7)

/**
   Extract comman from header.
*/
#define GS_I2C_SLAVE_DISPATCH_HEADER_TO_COMMAND(header) (header & 0x1f)

/**
   Domain handler.

   Basically same features as normal I2C slave rx callback. The generic I2C dispatch interface will parse the header (first byte)
   and call the associated handler, based on the domain.

   @param[in] channel I2C channel (handle).
   @param[in] cmd domain specific command.
   @param[in] rx_buffer Pointer to start of rx buffer.
   @param[in] rx number of bytes received so far.
   @param[in] cswitch If called from within an ISR (embedded platform), this will none NULL.
   @return total number of bytes to receive before next call back. Return 0 to ignore rest of data - no additional call backs will be done for current I2C transaction.
*/
typedef void (* gs_i2c_slave_dispatch_handler_t)(uint8_t channel, uint8_t cmd, const uint8_t * rx, size_t rx_length, gs_context_switch_t * cswitch);

/**
   Dispatch domains.

   Standard domains should be assigned from the lowest value.
   Application/board/product should be assigned from highest value.
*/
typedef enum {
    GS_I2C_SLAVE_DISPATCH_DOMAIN_RESERVED = 0, //! Avoid use - reserved for GSSB, GSSB broadcasts UID request on domain=0, cmd=13
    GS_I2C_SLAVE_DISPATCH_DOMAIN_USER1,        //! Avoid use - overlap with GSSB commands
    GS_I2C_SLAVE_DISPATCH_DOMAIN_USER2,        //! Avoid use - may overlap with GSSB commands
    GS_I2C_SLAVE_DISPATCH_DOMAIN_USER3,        //! Avoid use - may overlap with GSSB commands
    GS_I2C_SLAVE_DISPATCH_DOMAIN_PARAM,        //! Reserved for libparam.
    GS_I2C_SLAVE_DISPATCH_DOMAIN_USER5,
    GS_I2C_SLAVE_DISPATCH_DOMAIN_USER6,
    GS_I2C_SLAVE_DISPATCH_DOMAIN_USER7,
} gs_i2c_slave_dispatch_domain_t;

#ifdef __cplusplus
}
#endif
#endif
#endif
