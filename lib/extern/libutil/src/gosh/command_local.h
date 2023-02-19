/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/gosh/command.h>

/**
   Command I/O function - flush stdout.
*/
gs_error_t gs_command_stdio_flush(gs_command_context_t *ctx);

/**
   Command I/O function - wait for a key.
*/
gs_error_t gs_command_stdio_wait_for_key(gs_command_context_t *ctx, int *ch, int timeout_ms);

/**
   Complete command.
   @param[in] line command line to complete
   @param[in] max \a length (size)
   @param[in] out output stream, e.g. stdout
*/
gs_error_t gs_command_complete(char *line, size_t max_line_length, FILE* out);

/**
   Show help.
   @param line command line to show help for.
   @param out output stream, e.g. stdout
*/
gs_error_t gs_command_show_help(const char * command, FILE * out);

/**
   Change console mode.
   @param[in] mode console mode, 'cci'
   @return_gs_error_t
*/
int gs_console_change_mode(const char * mode);
