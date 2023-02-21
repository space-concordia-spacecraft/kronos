#ifndef GS_CSP_LINUX_COMMAND_LINE_H
#define GS_CSP_LINUX_COMMAND_LINE_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Command line support.
*/

#include <gs/util/linux/argp.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Command line options.
*/
extern const struct argp_child gs_csp_command_line_options;

/**
   Command line options - devices only.
*/
extern const struct argp_child gs_csp_command_line_options_devices;

#ifdef __cplusplus
}
#endif
#endif
