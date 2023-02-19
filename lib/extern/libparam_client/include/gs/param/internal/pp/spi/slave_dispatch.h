#ifndef GS_PARAM_INTERNAL_PP_SPI_SLAVE_DISPATCH_H
#define GS_PARAM_INTERNAL_PP_SPI_SLAVE_DISPATCH_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#if (GS_PARAM_INTERNAL_USE)

#include <gs/util/drivers/spi/slave.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Header for SPI slave dispatch protocol - must be first in all protocols.
*/
typedef struct __attribute__ ((packed)) gs_spi_slave_dispatch_header {
    uint8_t domain_command;
} gs_spi_slave_dispatch_header_t;

/**
   Make header from domain and command.
*/
#define GS_SPI_SLAVE_DISPATCH_HEADER(domain, command)   ((domain << 5) | (command & 0x1f))

/**
   Extract domain from header.
*/
#define GS_SPI_SLAVE_DISPATCH_HEADER_TO_DOMAIN(header)  ((header >> 5) & 0x7)

/**
   Extract comman from header.
*/
#define GS_SPI_SLAVE_DISPATCH_HEADER_TO_COMMAND(header) (header & 0x1f)

/**
   Domain handler.

   Basically same features as normal SPI slave rx callback. The generic SPI dispatch interface will parse the header (first byte)
   and call the associated handler, based on the domain.

   @param[in] channel SPI channel (handle).
   @param[in] cmd domain specific command.
   @param[in] rx_buffer Pointer to start of rx buffer.
   @param[in] rx number of bytes received so far.
   @param[in] cswitch If called from within an ISR (embedded platform), this will none NULL.
   @return total number of bytes to receive before next call back. Return 0 to ignore rest of data - no additional call backs will be done for current SPI transaction.
*/
typedef uint8_t (* gs_spi_slave_dispatch_handler_t)(uint8_t channel, uint8_t cmd, const uint8_t * rx_buffer, size_t rx, gs_context_switch_t * cswitch);

/**
   Dispatch domains.

   Standard domains should be assigned form the lowest value.
   Application/board/product should be assigned from highest value.
*/
typedef enum {
    GS_SPI_SLAVE_DISPATCH_DOMAIN_RESERVED = 0, //! Avoid using 0,
    GS_SPI_SLAVE_DISPATCH_DOMAIN_USER1,
    GS_SPI_SLAVE_DISPATCH_DOMAIN_USER2,
    GS_SPI_SLAVE_DISPATCH_DOMAIN_USER3,
    GS_SPI_SLAVE_DISPATCH_DOMAIN_PARAM,
    GS_SPI_SLAVE_DISPATCH_DOMAIN_USER5,
    GS_SPI_SLAVE_DISPATCH_DOMAIN_USER6,
    GS_SPI_SLAVE_DISPATCH_DOMAIN_USER7,
} gs_spi_slave_dispatch_domain_t;

/**
   Slave dispatch SPI receiver.

   Must be added on the channel as receiver function, using gs_spi_slave_set_rx().

   @param[in] cswitch If called from within an ISR (embedded platform), this will be set and allow for triggering context switch.
*/
uint8_t gs_spi_slave_dispatch_rx(uint8_t channel, const uint8_t * rx_buffer, size_t rx, bool new_request, gs_context_switch_t * cswitch);

#ifdef __cplusplus
}
#endif
#endif
#endif
