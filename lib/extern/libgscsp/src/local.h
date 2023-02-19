#ifndef GS_CSP_SRC_LOCAL_H
#define GS_CSP_SRC_LOCAL_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/csp/csp.h>
#include <gs/util/log.h>
#if (__linux__)
#define GS_CSP_COMMAND_LINE_SUPPORT 1
#include <gs/csp/linux/command_line.h>
#endif

GS_LOG_GROUP_EXTERN(gs_csp_log);
#define LOG_DEFAULT gs_csp_log

// local command line APIs
bool gs_csp_command_line_is_address_set(void);
uint8_t gs_csp_command_line_get_address(void);
const char * gs_csp_command_line_get_rtable(void);
gs_error_t gs_csp_command_line_configure_interfaces(void);

#endif
