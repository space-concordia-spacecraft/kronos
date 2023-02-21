#ifndef LIBGSCSP_INCLUDE_GS_CSP_DRIVERS_I2C_I2C_H
#define LIBGSCSP_INCLUDE_GS_CSP_DRIVERS_I2C_I2C_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   GomSpace I2C API for CSP.
*/

#include <csp/interfaces/csp_if_i2c.h>
#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Default I2C interface name.
*/
#define GS_CSP_I2C_DEFAULT_IF_NAME CSP_IF_I2C_DEFAULT_NAME

/**
   Initialize CSP for I2C device.

   @param[in] device I2C device.
   @param[in] mtu MTU size, use 0 for default value.
   @param[in] name name of CSP interface, if NULL #GS_CSP_I2C_DEFAULT_IF_NAME will be used.
   @param[in] set_default_route if \a true, the device will be set as default route.
   @param[out] csp_if the added CSP interface, if different from NULL.
   @return_gs_error_t
*/
gs_error_t gs_csp_i2c_init2(uint8_t device, uint32_t mtu, const char * name, bool set_default_route, csp_iface_t ** csp_if);

/**
   Initialize CSP for I2C device.

   The device must be pre-initialized with speed
   @note I2C device must be initialized allready with the same I2C address as the CSP address.

   @param[in] device I2C device.
   @param[in] csp_addr CSP address.
   @return_gs_error_t
   @see gs_csp_i2c_init2()
*/
gs_error_t gs_csp_i2c_init(uint8_t device, uint8_t csp_addr);

#ifdef __cplusplus
}
#endif
#endif
