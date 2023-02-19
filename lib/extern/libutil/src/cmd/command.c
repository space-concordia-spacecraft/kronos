/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */

#include "command.h"

static const gs_command_t GS_COMMAND_ROOT root_commands[] = {
    {
        .name="help",
        .help="Show help",
        .usage="[command]...",
        .optional_args=255,
        .handler=gs_command_cmd_help_handler,
    },
    {
        .name="sleep",
        .help="Sleep mS",
        .usage="<mS>",
        .mandatory_args=1,
        .handler=gs_command_cmd_sleep_handler,
    },
    {
        .name="watch",
        .help="Run commands at intervals (abort on key)",
        .usage="<interval mS> <command> [arg]...",
        .mandatory_args=2,
        .optional_args=255,
        .handler=gs_command_cmd_watch_handler,
    },
    {
        .name="watch_check",
        .help="Run commands at intervals (abort on key/failure)",
        .usage="<interval mS> <command> [arg]...",
        .mandatory_args=2,
        .optional_args=255,
        .handler=gs_command_cmd_watch_check_handler,
    },
    {
        .name="batch",
        .help="Run multiple commands",
        .mode = GS_COMMAND_FLAG_HIDDEN,
        .mandatory_args=GS_COMMAND_NO_ARGS,
        .handler=gs_command_cmd_batch_handler,
    },
    {
        .name="clock",
        .help="Get/set system clock",
        .usage="[<sec.nsec> | <YYYY-MM-DDTHH:MM:SSZ>]",
        .optional_args=1,
        .handler=gs_command_cmd_clock_handler,
    },
    {
        .name="console_mode",
        .help="Console mode",
        .mode = GS_COMMAND_FLAG_HIDDEN,
        .usage="<cci>",
        .mandatory_args=1,
        .handler=gs_command_cmd_console_mode_handler,
    },
#if defined(__linux__)
    {
        .name="exit",
        .help="Exit program",
        .mandatory_args=GS_COMMAND_NO_ARGS,
        .handler=gs_command_cmd_exit_handler,
    },
#endif
};

gs_error_t gs_command_register_commands(void)
{
    return GS_COMMAND_REGISTER(root_commands);
}
