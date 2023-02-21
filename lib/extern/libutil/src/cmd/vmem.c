/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */

#include "vmem.h"

static const gs_command_t GS_COMMAND_SUB vmem_commands[] = {
    {
        .name="read",
        .help="Read from virtual memory",
        .usage="<addr> <length>",
        .mandatory_args=2,
        .handler=gs_vmem_cmd_vmem_read_handler,
    },
    {
        .name="write",
        .help="Write to virtual memory",
        .usage="<addr> <data>",
        .mandatory_args=2,
        .handler=gs_vmem_cmd_vmem_write_handler,
    },
    {
        .name="lock",
        .help="Lock the virtual memory",
        .usage="<entry>",
        .mandatory_args=1,
        .handler=gs_vmem_cmd_vmem_lock_handler,
    },
    {
        .name="unlock",
        .help="Unlock the virtual memory",
        .usage="<entry>",
        .mandatory_args=1,
        .handler=gs_vmem_cmd_vmem_unlock_handler,
    },
    {
        .name="list",
        .help="Show virtual memory mappings",
        .mandatory_args=GS_COMMAND_NO_ARGS,
        .handler=gs_vmem_cmd_vmem_list_handler,
    },
    {
        .name="info",
        .help="Show virtual memory mappings + info",
        .mandatory_args=GS_COMMAND_NO_ARGS,
        .handler=gs_vmem_cmd_vmem_info_handler,
    },
};

static const gs_command_t GS_COMMAND_ROOT root_commands[] = {
    {
        .name="vmem",
        .help="Virtual memory",
        .chain=GS_COMMAND_INIT_CHAIN(vmem_commands),
    },
};

gs_error_t gs_vmem_register_commands(void)
{
    return GS_COMMAND_REGISTER(root_commands);
}
