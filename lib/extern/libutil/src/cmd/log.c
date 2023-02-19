/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */

#include "log.h"

static const gs_command_t GS_COMMAND_SUB group_commands[] = {
    {
        .name="list",
        .help="List groups",
        .usage="[group]",
        .optional_args=1,
#if (__AVR__ == 0)
        .completer=gs_log_cmd_log_group_list_completer,
#endif
        .handler=gs_log_cmd_log_group_list_handler,
    },
    {
        .name="mask",
        .help="Set mask: e|w|n|i|d|t|stand|all|off",
        .usage="<group[,group]> <[+-]level[,level]>",
        .mandatory_args=2,
#if (__AVR__ == 0)
        .completer=gs_log_cmd_log_group_mask_completer,
#endif
        .handler=gs_log_cmd_log_group_mask_handler,
    },
    {
        .name="insert",
        .help="Log message to group",
        .usage="<group> <level> <message>",
        .mandatory_args=3,
#if (__AVR__ == 0)
        .completer=gs_log_cmd_log_group_insert_completer,
#endif
        .handler=gs_log_cmd_log_group_insert_handler,
    },
    {
        .name="color",
        .help="Enable/disable color logging (stdout)",
        .usage="<on>",
        .mandatory_args=1,
        .handler=gs_log_cmd_log_group_color_handler,
    },
};

static const gs_command_t GS_COMMAND_SUB appender_commands[] = {
    {
        .name="list",
        .help="List appenders",
        .usage="[appender]",
        .optional_args=1,
#if (__AVR__ == 0)
        .completer=gs_log_cmd_log_appender_list_completer,
#endif
        .handler=gs_log_cmd_log_appender_list_handler,
    },
    {
        .name="mask",
        .help="Set mask: e|w|n|i|d|t|stand|all|off",
        .usage="<appender[,appender]> <[+-]level[,level]>",
        .mandatory_args=2,
#if (__AVR__ == 0)
        .completer=gs_log_cmd_log_appender_mask_completer,
#endif
        .handler=gs_log_cmd_log_appender_mask_handler,
    },
    {
        .name="hist",
        .help="Show history",
        .usage="<appender> [count] [pattern]",
        .mandatory_args=1,
        .optional_args=2,
#if (__AVR__ == 0)
        .completer=gs_log_cmd_log_appender_hist_completer,
#endif
        .handler=gs_log_cmd_log_appender_hist_handler,
    },
    {
        .name="clear",
        .help="Clear history",
        .usage="<appender>",
        .mandatory_args=1,
#if (__AVR__ == 0)
        .completer=gs_log_cmd_log_appender_clear_completer,
#endif
        .handler=gs_log_cmd_log_appender_clear_handler,
    },
};

static const gs_command_t GS_COMMAND_SUB log_commands[] = {
    {
        .name="group",
        .help="Group commands",
        .chain=GS_COMMAND_INIT_CHAIN(group_commands),
    },
    {
        .name="appender",
        .help="Appender commands",
        .chain=GS_COMMAND_INIT_CHAIN(appender_commands),
    },
};

static const gs_command_t GS_COMMAND_ROOT root_commands[] = {
    {
        .name="log",
        .help="Log system",
        .chain=GS_COMMAND_INIT_CHAIN(log_commands),
    },
    {
        .name="debug",
        .help="Set log group mask: e|w|n|i|d|t|stand|all|off",
        .usage="<group[,group]> <[+-]level[,level]>",
        .mandatory_args=2,
#if (__AVR__ == 0)
        .completer=gs_log_cmd_debug_completer,
#endif
        .handler=gs_log_cmd_debug_handler,
    },
};

gs_error_t gs_log_register_commands(void)
{
    return GS_COMMAND_REGISTER(root_commands);
}
