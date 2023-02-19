#ifndef GS_PARAM_INTERNAL_PP_SPI_SPI_H
#define GS_PARAM_INTERNAL_PP_SPI_SPI_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#if (GS_PARAM_INTERNAL_USE)

#include <gs/param/pp/spi/spi.h>
#include <gs/param/internal/pp/spi/slave_dispatch.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Domain commands.
*/
typedef enum {
    GS_PARAM_SPI_COMMAND_SET = 1,
    GS_PARAM_SPI_COMMAND_GET = 2,
    GS_PARAM_SPI_COMMAND_SET_WITH_CHECKSUM = 10,
    GS_PARAM_SPI_COMMAND_GET_WITH_CHECKSUM = 11,
} gs_param_spi_command_t;

#define GS_PARAM_SPI_LENGTH_TABLE(length, table)  ((length << 3) | (table & 0x07))
#define GS_PARAM_SPI_LENGTH_TABLE_TO_LENGTH(lt)   ((lt >> 3) & 0x1f)
#define GS_PARAM_SPI_LENGTH_TABLE_TO_TABLE(lt)    (lt & 0x07)

/**
   Data structure for setting parameter.
*/
typedef struct __attribute__ ((packed)) gs_param_spi_set {
    gs_spi_slave_dispatch_header_t header;
    uint8_t length_table;
    uint8_t addr;
    uint8_t data[]; // data (+ checksum)
} gs_param_spi_set_t;

/**
   Data structure for getting parameter.
*/
typedef struct __attribute__ ((packed)) gs_param_spi_get {
    gs_spi_slave_dispatch_header_t header;
    uint8_t length_table;
    uint8_t addr;
    uint8_t filler; // filler/delay - allow slave to find and prepare data/response
    uint8_t data[]; // data
} gs_param_spi_get_t;

/**
   Data structure for getting parameter with checksum
*/
typedef struct __attribute__ ((packed)) gs_param_spi_get_with_checksum {
    gs_spi_slave_dispatch_header_t header;
    uint8_t length_table;
    uint8_t addr;
    uint8_t checksum;
    uint8_t filler; // filler/delay - allow slave to find and prepare data/response
    uint8_t data[]; // data + checksum
} gs_param_spi_get_with_checksum_t;

#ifdef __cplusplus
}
#endif
#endif
#endif
