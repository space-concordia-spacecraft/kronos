/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */

#include <gs/util/gosh/command.h>

#ifdef __cplusplus
extern "C" {
#endif

int        gs_log_cmd_log_group_list_handler(gs_command_context_t* ctx);
gs_error_t gs_log_cmd_log_group_list_completer(gs_command_context_t* ctx, int arg_to_complete);
int        gs_log_cmd_log_group_mask_handler(gs_command_context_t* ctx);
gs_error_t gs_log_cmd_log_group_mask_completer(gs_command_context_t* ctx, int arg_to_complete);
int        gs_log_cmd_log_group_insert_handler(gs_command_context_t* ctx);
gs_error_t gs_log_cmd_log_group_insert_completer(gs_command_context_t* ctx, int arg_to_complete);
int        gs_log_cmd_log_group_color_handler(gs_command_context_t* ctx);
int        gs_log_cmd_log_appender_list_handler(gs_command_context_t* ctx);
gs_error_t gs_log_cmd_log_appender_list_completer(gs_command_context_t* ctx, int arg_to_complete);
int        gs_log_cmd_log_appender_mask_handler(gs_command_context_t* ctx);
gs_error_t gs_log_cmd_log_appender_mask_completer(gs_command_context_t* ctx, int arg_to_complete);
int        gs_log_cmd_log_appender_hist_handler(gs_command_context_t* ctx);
gs_error_t gs_log_cmd_log_appender_hist_completer(gs_command_context_t* ctx, int arg_to_complete);
int        gs_log_cmd_log_appender_clear_handler(gs_command_context_t* ctx);
gs_error_t gs_log_cmd_log_appender_clear_completer(gs_command_context_t* ctx, int arg_to_complete);
int        gs_log_cmd_debug_handler(gs_command_context_t* ctx);
gs_error_t gs_log_cmd_debug_completer(gs_command_context_t* ctx, int arg_to_complete);

gs_error_t gs_log_register_commands(void);

#ifdef __cplusplus
}
#endif
