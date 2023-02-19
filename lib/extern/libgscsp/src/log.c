/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/csp/log.h>
#include <csp/external/csp_debug.h>

gs_error_t gs_csp_log_init(void)
{
    return gs_log_group_register(gs_csp_log);
}
