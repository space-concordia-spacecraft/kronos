/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include "command_local.h"
#include "console_local.h"
#include "../cmd/command.h"

#include <stdlib.h>

#if defined(__linux__)
#include <unistd.h>
#include <termios.h>
#endif

#include <gs/util/stdio.h>
#include <gs/util/clock.h>
#include <gs/util/time.h>
#include <gs/util/string.h>
#include <conf_util.h>

int gs_command_cmd_help_handler(gs_command_context_t * context)
{
    return gs_command_show_help(gs_command_args(context), context->out);
}

int gs_command_cmd_sleep_handler(gs_command_context_t * context)
{
    uint32_t sleep_ms;
    if (gs_string_to_uint32(context->argv[1], &sleep_ms) != GS_OK) {
        return GS_ERROR_ARG;
    }

    gs_time_sleep_ms(sleep_ms);

    return GS_OK;
}

static int cmd_watch(gs_command_context_t * context, bool check_error)
{
    uint32_t sleep_ms;
    if (gs_string_to_uint32(context->argv[1], &sleep_ms) != GS_OK) {
        return GS_ERROR_ARG;
    }

    const char * new_command = strstr(gs_command_args(context), " ");
    if (new_command == NULL) {
        return GS_ERROR_ARG;
    }
    ++new_command;

    while(1) {

        gs_error_t cmd_result;
        gs_error_t error = gs_command_execute(new_command, &cmd_result, context->out, context->io_functions, context->io_ctx);
        if (error) {
            return error;
        }
        if (check_error && cmd_result) {
            return cmd_result;
        }

        if (gs_stdio_getchar_timed(sleep_ms, NULL) != GS_ERROR_TIMEOUT) {
            break;
        }
    }

    return GS_OK;
}

int gs_command_cmd_watch_handler(gs_command_context_t * context)
{
    return cmd_watch(context, false);
}

int gs_command_cmd_watch_check_handler(gs_command_context_t * context)
{
    return cmd_watch(context, true);
}

#define CONTROL(X)  ((X) - '@')

int gs_command_cmd_batch_handler(gs_command_context_t * ctx)
{
    char c;
    int quit = 0, execute = 0;
    unsigned int batch_size = 100;
    unsigned int batch_input = 0;
    unsigned int batch_count = 0;
    char * batch[20] = {};
    printf("Type each command followed by enter, hit ctrl+e to end typing, ctrl+x to cancel:\r\n");

    /* Wait for ^q to quit. */
    while (quit == 0) {

        /* Get character */
        c = getchar();

        switch (c) {

            /* CTRL + X */
            case 0x18:
                quit = 1;
                break;

		/* CTRL + E */
            case 0x05:
                execute = 1;
                quit = 1;
                break;

		/* Backspace */
            case CONTROL('H'):
            case 0x7f:
                if (batch_input > 0) {
                    putchar('\b');
                    putchar(' ');
                    putchar('\b');
                    batch_input--;
                }
                break;

            case '\r':
                putchar('\r');
                putchar('\n');
                if ((batch[batch_count] != NULL) && (batch_input < batch_size))
                    batch[batch_count][batch_input++] = '\r';
                if ((batch[batch_count] != NULL) && (batch_input < batch_size))
                    batch[batch_count][batch_input++] = '\0';
                batch_count++;
                batch_input = 0;
                if (batch_count == 20)
                    quit = 1;
                break;

            default:
                putchar(c);
                if (batch[batch_count] == NULL) {
                    batch[batch_count] = calloc(GS_CONSOLE_INPUT_LEN+1, 1);
                }

                if ((batch[batch_count] != NULL) && (batch_input < batch_size))
                    batch[batch_count][batch_input++] = c;
                break;
        }
    }

    if (execute) {
        printf("\r\n");
        for (unsigned int i = 0; i <= batch_count; i++) {
            if (batch[i])
                printf("[%02u] %s\r\n", i, batch[i]);
        }
        printf("Press ctrl+e to execute, or any key to abort\r\n");
        c = getchar();
        if (c != 0x05)
            execute = 0;
    }

    /* Run/Free batch job */
    for (unsigned int i = 0; i <= batch_count; i++) {
        if (execute && batch[i]) {
            printf("EXEC [%02u] %s\r\n", i, batch[i]);
            gs_command_run(batch[i], NULL);
        }
        free(batch[i]);
    }

    return GS_OK;
}

#if defined(__linux__)
int gs_command_cmd_exit_handler(gs_command_context_t * context)
{
    gs_console_exit();
    exit(EXIT_SUCCESS);
    return GS_OK;
}
#endif

int gs_command_cmd_clock_handler(gs_command_context_t * ctx)
{
    if (ctx->argc > 1) {
        gs_timestamp_t ts;
        if (gs_clock_from_string(ctx->argv[1], &ts) != GS_OK) {
            return GS_ERROR_ARG;
        }
        gs_error_t error = gs_clock_set_time(&ts);
        if (error) {
            return error;
        }
    }

    timestamp_t clock;
    gs_clock_get_monotonic(&clock);
    gs_command_set_output_printf(ctx, "", "monotonic", "%"PRIu32".%09"PRIu32"", clock.tv_sec, clock.tv_nsec);

    char tbuf[25];
    gs_clock_get_time(&clock);
    gs_command_set_output_printf(ctx, "", "realtime", "%"PRIu32".%09"PRIu32"", clock.tv_sec, clock.tv_nsec);

    gs_clock_to_iso8601_string(&clock, tbuf, sizeof(tbuf));
    fprintf(ctx->out, "realtime: %"PRIu32".%09"PRIu32" seconds -> %s\r\n", clock.tv_sec, clock.tv_nsec, tbuf);

    return GS_OK;
}

int gs_command_cmd_console_mode_handler(gs_command_context_t * ctx)
{
    return gs_console_change_mode(ctx->argv[1]);
}

gs_error_t gs_command_register_default_commands(void)
{
    return gs_command_register_commands();
}
