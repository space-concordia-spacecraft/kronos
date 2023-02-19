/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/embed/drivers/sys/reset.h>
#include <csp/arch/csp_system.h>

int gs_csp_sys_reboot(void)
{
    gs_sys_reset(GS_SYS_RESET_CSP);
    return CSP_ERR_NONE;
}

// libcsp requires this function, but the FreeRTOS port's used by GomSpace doesn't have it
size_t xPortGetFreeHeapSize(void)
{
    return 0;
}
