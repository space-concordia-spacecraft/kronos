/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */

#include <gs/util/gosh/command.h>

#ifdef __cplusplus
extern "C" {
#endif

int        gs_vmem_cmd_vmem_read_handler(gs_command_context_t* ctx);
int        gs_vmem_cmd_vmem_write_handler(gs_command_context_t* ctx);
int        gs_vmem_cmd_vmem_lock_handler(gs_command_context_t* ctx);
int        gs_vmem_cmd_vmem_unlock_handler(gs_command_context_t* ctx);
int        gs_vmem_cmd_vmem_list_handler(gs_command_context_t* ctx);
int        gs_vmem_cmd_vmem_info_handler(gs_command_context_t* ctx);

gs_error_t gs_vmem_register_commands(void);

#ifdef __cplusplus
}
#endif
