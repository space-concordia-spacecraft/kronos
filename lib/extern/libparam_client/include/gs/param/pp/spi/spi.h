#ifndef GS_PARAM_PP_SPI_SPI_H
#define GS_PARAM_PP_SPI_SPI_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   SPI Param Protocol (pp) client interface.
*/

#include <gs/param/pp/pp.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Initialize the param protocol handle for SPI.

   @param[in] pp handle.
   @param[in] slave slave to communicate with.
   @param[in] big_endian \a true if slave is big endian. Used to convert to host endian.
   @return_gs_error_t
*/
gs_error_t gs_pp_spi_init(gs_pp_t * pp, uint8_t slave, bool big_endian);

#ifdef __cplusplus
}
#endif
#endif
