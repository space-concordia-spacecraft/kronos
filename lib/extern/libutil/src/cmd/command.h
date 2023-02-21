/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */

#include <gs/util/gosh/command.h>

#ifdef __cplusplus
extern "C" {
#endif

int        gs_command_cmd_help_handler(gs_command_context_t* ctx);
int        gs_command_cmd_sleep_handler(gs_command_context_t* ctx);
int        gs_command_cmd_watch_handler(gs_command_context_t* ctx);
int        gs_command_cmd_watch_check_handler(gs_command_context_t* ctx);
int        gs_command_cmd_batch_handler(gs_command_context_t* ctx);
int        gs_command_cmd_clock_handler(gs_command_context_t* ctx);
int        gs_command_cmd_console_mode_handler(gs_command_context_t* ctx);
int        gs_command_cmd_exit_handler(gs_command_context_t* ctx);

gs_error_t gs_command_register_commands(void);

#ifdef __cplusplus
}
#endif
