#ifndef GS_UTIL_LINUX_DRIVERS_CAN_CAN_H
#define GS_UTIL_LINUX_DRIVERS_CAN_CAN_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Linux CAN interface.

   @note Only 1 filter/mask can be set, using gs_can_set_standard_filter_mask() or gs_can_set_extended_filter_mask()
*/

#include <gs/util/drivers/can/can.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Open and initialize a CAN handle.
   @param[in] ifname name of CAN interface.
   @param[out] handle opened CAN handle.
   @return_gs_error_t
*/
gs_error_t gs_can_open(const char * ifname, int * handle);

#ifdef __cplusplus
}
#endif
#endif
