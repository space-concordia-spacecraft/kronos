/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/linux/command_line.h>
#include <gs/util/linux/signal.h>
#include <gs/util/string.h>
#include <gs/util/log.h>

#define KEY_IGNORE_CTRLC    200

static bool ignore_ctrlc;

static int parser(int key, char *arg, struct argp_state *state)
{
    switch (key) {
        case KEY_IGNORE_CTRLC:
            ignore_ctrlc = true;
            gs_signal_ignore(SIGINT);
            break;

        case 'h':
            argp_help(state->root_argp, state->out_stream, ARGP_HELP_STD_HELP, state->name);
            exit(0);
            break;

        case 'l':
            gs_log_set_level_mask_configuration(arg);
            break;
            
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static const struct argp_option options[] = {
    {
        .name = "ignore-ctrlc",
        .key = KEY_IGNORE_CTRLC,
        .doc = "Ignore/disable CTRL-C"
    },
    {0}
};

static const struct argp argp_console = {.options = options, .parser = parser};

const struct argp_child gs_console_command_line_ignore_ctrlc_argp = {.argp = &argp_console};

bool gs_command_line_ignore_ctrlc(void)
{
    return ignore_ctrlc;
}

static const struct argp_option help_options[] = {
    {
        .name = "help",
        .key = 'h',
        .doc = "Give this help list"
    },
    {0}
};

static const struct argp gs_argp_help = {.options = help_options, .parser = parser};

const struct argp_child gs_help_command_line_argp = {.argp = &gs_argp_help};

const char * gs_command_line_program_name(const char * argv)
{
    if (gs_string_empty(argv) == false) {
        const char * name = strrchr(argv, '/');
        if (name) {
            // skip slash
            ++name;
            if (gs_string_empty(name) == false) {
                return name;
            }
        } else {
            return argv;
        }
    }
    return "<program>";
}

static const struct argp_option log_options[] = {
    {
        .name = "log-level",
        .key = 'l',
        .arg = "LEVEL",
        .doc = "Set log level for group(s) or appender(s)\nLEVEL=<name>=<level>[,<name>=<level>]"
    },
    {0}
};

static const struct argp gs_argp_log = {.options = log_options, .parser = parser};

const struct argp_child gs_log_command_line_argp = {.argp = &gs_argp_log};
