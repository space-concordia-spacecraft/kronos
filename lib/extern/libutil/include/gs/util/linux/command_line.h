#ifndef GS_UTIL_LINUX_COMMAND_LINE_H
#define GS_UTIL_LINUX_COMMAND_LINE_H
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
   Command line options for ignoring CTRL-C
*/
extern const struct argp_child gs_console_command_line_ignore_ctrlc_argp;

/**
   Command line options for adding -h (help).
*/
extern const struct argp_child gs_help_command_line_argp;

/**
   Command line options for adding -l (log level).
*/
extern const struct argp_child gs_log_command_line_argp;

/**
   Return if ctrl-c ignored on command line.
   @return \a true i ctrl-c ignored.
*/
bool gs_command_line_ignore_ctrlc(void);

/**
   Return program name based on argv[0].
   @param[in] argv expected to be argv[0] amd point to the program name (possibly with full path).
   @return program name.
*/
const char * gs_command_line_program_name(const char * argv);

#ifdef __cplusplus
}
#endif
#endif
