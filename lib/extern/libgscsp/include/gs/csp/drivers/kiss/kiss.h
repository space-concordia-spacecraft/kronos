#ifndef LIBGSCSP_INCLUDE_GS_CSP_DRIVERS_KISS_KISS_H
#define LIBGSCSP_INCLUDE_GS_CSP_DRIVERS_KISS_KISS_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

/**
   @file

   GomSpace KISS API for CSP.
*/

#include <csp/interfaces/csp_if_kiss.h>
#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Default KISS interface name.
*/
#define GS_CSP_KISS_DEFAULT_IF_NAME CSP_IF_KISS_DEFAULT_NAME

/**
   Initialize and add KISS interface.
   @note The device must be initialized.

   @param[in] device UART device.
   @return_gs_error_t
   @see gs_csp_kiss_init2()
*/
gs_error_t gs_csp_kiss_init(uint8_t device);

/**
   Initialize and add KISS interface.
   @note The device must be initialized.

   @param[in] device UART device.
   @param[in] mtu MTU size, use 0 for default value.
   @param[in] name CSP interface name, use NULL to use default name.
   @param[in] set_default_route \a true to set interface as default route.
   @param[out] csp_if the added CSP interface, if different from NULL.
   @return_gs_error_t
*/
gs_error_t gs_csp_kiss_init2(uint8_t device, uint32_t mtu, const char * name, bool set_default_route, csp_iface_t ** csp_if);

#ifdef __cplusplus
}
#endif
#endif
