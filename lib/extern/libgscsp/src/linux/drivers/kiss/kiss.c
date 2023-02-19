/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/csp/drivers/kiss/kiss.h>

gs_error_t gs_csp_kiss_init2(uint8_t device, uint32_t mtu, const char * name, bool set_default_route, csp_iface_t ** csp_if)
{
    // On linux KISS interfaces are added through command-line options.
    return GS_ERROR_NOT_SUPPORTED;
}
