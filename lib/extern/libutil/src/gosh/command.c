/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include "command_local.h"

#include <stdlib.h>
#include <ctype.h>

#include <gs/util/vmem.h>  // register commands
#include <gs/util/log.h>   // register commands
#include <gs/util/string.h>
#include <gs/util/clock.h>
#include <gs/util/lock.h>

#define MAX_ARGC                30

#ifdef __AVR__
#include <avr/pgmspace.h>
#define cmd_strcmp strcmp_P
#define cmd_strncmp strncmp_P
#define cmd_strlen strlen_P
#define cmd_strcpy strcpy_P
#define cmd_read_ptr(ptr) ((void *) pgm_read_word(ptr))
#define cmd_read_int(ptr) pgm_read_word(ptr)
#else
#define cmd_strcmp strcmp
#define cmd_strncmp strncmp
#define cmd_strlen strlen
#define cmd_strcpy strcpy
#define cmd_read_ptr(ptr) *ptr
#define cmd_read_int(ptr) *ptr
#endif

// define common command log group.
static GS_LOG_GROUP(gs_command_log, "command", GS_LOG_CAT_COMMAND, LOG_DEFAULT_MASK | LOG_INFO_MASK);
#define LOG_DEFAULT gs_command_log

/**
   Compile check that size of gs_command_t is multiplum of 4.
*/
GS_STATIC_ASSERT((sizeof(gs_command_t) % 4) == 0, gs_command_t_is_not_a_multiplum_of_4);

// Private context
typedef struct process_context {
    // command context - must be first, as it is used to access private context (same address)
    gs_command_context_t context;
    // process function
    gs_error_t (*process)(const gs_command_t * const cmds, int cmd_count, int arg_offset, struct process_context * pc);
    // command error
    gs_error_t error;
    // only exact match (space after last argument)
    bool requires_exact_match;
    // first command match
    const gs_command_t * cmd;
    // number of hits when hunting commands, completion etc.
    unsigned int hits;
    // complete result
    struct {
        char * line;
        size_t token_start;
    } complete;
} private_context_t;

// command block
typedef struct gs_command_block {
    //! Pointer to command block.
    const gs_command_t * commands;
    //! Number of commands in command block.
    size_t count;
    //! Reference to next command block.
    struct gs_command_block * next;
} gs_command_block_t;

// commands
static gs_command_block_t g_commands;

// minimum stack size in bytes.
static size_t g_stack_size;

// command logger callback
static gs_command_log_t g_command_logger = NULL;
static void * g_command_logger_ctx = NULL;

static gs_error_t command_stdio_set_result(gs_command_context_t *ctx, const char *group, const char *key, const char *value)
{
    static const char* printed_group_header = NULL;
    /* Print Group header if Group string is non-empty */
    if ((group != NULL) && (group[0] != '\0')) {
        if (printed_group_header != group) {
            fprintf(ctx->out, "%s:\r\n", group);
            printed_group_header = group;
        }
    }
    /* Print "<key>: <value>" if key string is non-empty */
    if (key != NULL) {
        if (key[0] != '\0') {
            if ((group != NULL) && (group[0] != '\0')) {
                fprintf(ctx->out, "   %s: ", key);
            } else {
                fprintf(ctx->out, "%s: ", key);
            }
        }
    }
    fprintf(ctx->out, "%s\r\n", value);
    return GS_OK;
}

gs_error_t gs_command_stdio_flush(gs_command_context_t *ctx)
{
    fflush(ctx->out);
    return GS_OK;
}

gs_error_t gs_command_stdio_wait_for_key(gs_command_context_t *ctx, int *ch, int timeout_ms)
{
    return gs_stdio_getchar_timed(timeout_ms, ch);
}

static const gs_command_io_functions_t stdio_functions = {
    .set_result   = command_stdio_set_result,
    .flush        = gs_command_stdio_flush,
    .wait_for_key = gs_command_stdio_wait_for_key
};

const char * gs_command_args(gs_command_context_t *ctx)
{
    if (ctx->argc > 1) {
        // find first matching argument (= starts with) - this is not 100% and doesn't handle arguments with spaces (quoted)
        const char * arg = ctx->command_line;
        while (arg && arg[0]) {
            if (strncmp(arg, ctx->argv[1], strlen(ctx->argv[1])) == 0) {
                return arg;
            }
            // skip argument
            for (; *arg && (*arg != ' '); ++arg);
            // skip spaces
            // cppcheck-suppress redundantCondition
            for (; *arg && (*arg == ' '); ++arg);
        }
    }
    return "";
}

bool gs_command_build_argv(char *line, int *argc, char **argv, int max_argc)
{
    // Skip spaces
    for (; line && *line && isspace((unsigned int)*line); ++line);

    *argc = 0;
    argv[*argc] = line;

    char quote = 0;

    while (*line) {
        // check for quote's: ' or "
        if ((*line == '\'') || (*line == '\"')) {
            if (quote == 0) {
                quote = *line;
                argv[*argc]++;
            } else if (quote == *line) {
                quote = 0;
                *line = '\0';
            }
        }
        // check for whitespace and no quotes active
        else if (isspace((unsigned int)*line) && quote == 0) {
            /* Delete space */
            *line++ = '\0';

            // skip spaces
            for (; *line && isspace((unsigned int)*line); ++line);

            /* If there is more data, we have another argument */
            if (*line) {
                (*argc)++;
                if (*argc >= max_argc) {
                    return false;
                }
                argv[*argc] = line;
            }

            continue;
        }

        line++;
    }

    (*argc)++;
    if (*argc >= max_argc) {
        return false;
    }

    // According to C11 section 5.1.2.2.1, argv[argc] must be NULL
    argv[*argc] = NULL;

    // Check for invalid number of quotes
    return (quote == 0) ? true : false;
}

static inline gs_error_t command_logger(const char *cmd_line, gs_error_t ret, gs_error_t cmd_ret, gs_timestamp_t ts, gs_timestamp_t te)
{
    gs_lock_lock();
    gs_command_log_t logger = g_command_logger;
    void * log_ctx = g_command_logger_ctx;
    gs_lock_unlock();

    if (logger) {
        return logger(cmd_line, ret, cmd_ret, ts, te, log_ctx);
    }
    return GS_OK;
}

static gs_error_t command_execute(const gs_command_t * const cmds, int cmd_count, int arg_offset, private_context_t * pc)
{
    for (int i = 0; i < cmd_count; i++) {
        const gs_command_t * cmd = &cmds[i];

        if (cmd_strcmp(pc->context.argv[arg_offset], cmd->name) == 0) {
            // check for sub-commands
            const gs_command_t * list = (void*) cmd_read_ptr(&cmd->chain.list);
            if (list) {
                ++arg_offset;
                if (arg_offset >= pc->context.argc) {
                    return GS_ERROR_TYPE;
                }
                return command_execute(list, cmd_read_int(&cmd->chain.count), arg_offset, pc);
            }

            gs_command_handler_t handler = (void *) cmd_read_ptr(&cmd->handler);
            if (handler == NULL) {
                return GS_ERROR_NOT_IMPLEMENTED;
            }

            pc->context.argc -= arg_offset;
            pc->context.argv = &pc->context.argv[arg_offset];
            pc->context.command = cmd;

            // check arguments - if specified
            if (cmd->mandatory_args || cmd->optional_args) {
                const int min_args = (cmd->mandatory_args == GS_COMMAND_NO_ARGS) ? 0 : cmd->mandatory_args;
                const int args = (pc->context.argc - 1);
                if (args < min_args) {
                    return GS_ERROR_ARG;
                }
                if (args > (min_args + cmd->optional_args)) {
                    return GS_ERROR_ARG;
                }
            }

            pc->error = handler(&pc->context);
            return GS_OK; // command was excecuted
        }
    }

    return GS_ERROR_NOT_FOUND;
}

static gs_error_t command_process(private_context_t * pc)
{
    const char * command = gs_string_skip_leading_spaces(pc->context.command_line);

    // Make copy of string, because command parser modifies it
    const size_t command_len = strlen(command);
    char command_copy[command_len + 1];
    strcpy(command_copy, command);

    if (command_len && command[command_len-1] == ' ') {
        pc->requires_exact_match = true;
    }

    pc->context.optsp  = 1;
    pc->context.optind = 1;
    pc->context.optopt = '?';
    pc->context.command_line = command;

    // split into arguments
    char *argv[MAX_ARGC + 1];
    if (gs_command_build_argv(command_copy, &pc->context.argc, argv, MAX_ARGC + 1) == false) {
        return GS_ERROR_ARG;
    }
    pc->context.argv = (const char * const *) argv;

    gs_error_t error = GS_ERROR_NOT_FOUND;
    for (const gs_command_block_t * block = &g_commands; block && (error == GS_ERROR_NOT_FOUND); block = block->next) {
        if (block->commands) {
            error = (pc->process)(block->commands, block->count, 0, pc);
        }
    }

    return error;
}

gs_error_t gs_command_run(const char * command, gs_error_t * return_command_result)
{
    return gs_command_execute_stdio(command, return_command_result);
}

gs_error_t gs_command_execute_stdio(const char * command, gs_error_t * return_command_result)
{
    return gs_command_execute(command, return_command_result, stdout, &stdio_functions, NULL);
}

gs_error_t gs_command_execute(const char * command, gs_error_t * return_command_result, FILE *out,
                              const gs_command_io_functions_t *iof, void *iof_ctx)
{
    command = gs_string_skip_leading_spaces(command);
    GS_CHECK_ARG(gs_string_empty(command) == false);

    private_context_t pc = {
        .process = command_execute,
        .error = GS_OK,
        .context = {
            .command_line = command,
            .out = out,
            .io_functions = iof,
            .io_ctx = iof_ctx,
        }
    };
    gs_timestamp_t tm_start, tm_end;
    gs_clock_get_time(&tm_start);
    gs_error_t error = command_process(&pc);
    gs_clock_get_time(&tm_end);
    command_logger(pc.context.command_line, error, pc.error, tm_start, tm_end);
    if ((error == GS_OK) && return_command_result) {
        *return_command_result = pc.error;
    }
    return error;
}

gs_error_t gs_command_set_output(gs_command_context_t *ctx, const char* group, const char* key, const char* value)
{
    GS_CHECK_ARG(ctx);

    if (ctx->io_functions && ctx->io_functions->set_result) {
        return ctx->io_functions->set_result(ctx, group, key, value);
    }

    /* If no IO-function set - ignore the data and send Success */
    return GS_OK;
}

gs_error_t gs_command_set_output_printf(gs_command_context_t *ctx, const char* group, const char* key, const char * format, ...)
{
    GS_CHECK_ARG(ctx);

    if (ctx->io_functions && ctx->io_functions->set_result)
    {
        va_list args;
        va_start(args, format);
        char value[256];
        int size = vsnprintf(value, sizeof(value), format, args);
        va_end(args);

        /* Don't allow to set truncated results - Return error in this case */
        if (size >= (int)sizeof(value)) {
            return GS_ERROR_ALLOC;
        }

        return ctx->io_functions->set_result(ctx, group, key, value);
    }

    /* If no IO-function set - ignore the data and send Success */
    return GS_OK;
}

gs_error_t gs_command_flush_output(gs_command_context_t *ctx)
{
    GS_CHECK_ARG(ctx);

    if (ctx->io_functions && ctx->io_functions->flush) {
        return ctx->io_functions->flush(ctx);
    }

    /* If no IO-function set - ignore the data and send Success */
    return GS_OK;
}

bool gs_command_wait_any_key(gs_command_context_t *ctx, int timeout_ms)
{
    int ch;
    gs_error_t ret = gs_command_wait_key(ctx, &ch, timeout_ms);

    if (ret == GS_ERROR_TIMEOUT) {
        return false;
    }

    /* Ensure that a commands handler will not stall if IO function if not available etc.
       False will only be returned in case of a positive timeout */
    return true;
}

gs_error_t gs_command_wait_key(gs_command_context_t *ctx, int* ch, int timeout_ms)
{
    if (ctx && ctx->io_functions && ctx->io_functions->wait_for_key)
    {
        return ctx->io_functions->wait_for_key(ctx, ch, timeout_ms);
    }

    /* If no IO-function set set return GS_ERROR_HANDLE */
    return GS_ERROR_HANDLE;
}

unsigned int gs_command_completer_add_token(gs_command_context_t * ctx, const char * token, bool exact)
{
    private_context_t * pc = (private_context_t *) ctx;
    char * line = &pc->complete.line[pc->complete.token_start];

    if (token == NULL) {
        // mark any pending partial token as exact
        if ((line[0] == 0) || (pc->hits != 1)) {
            return pc->hits;
        }
        exact = true;
    }

    if (exact) {
        if (token) {
            strcpy(line, token);
        }
        strcat(line, " ");
        pc->complete.token_start = strlen(pc->complete.line);
        pc->hits = 1;
    } else {
        if (pc->hits == 0) {
            strcpy(line, token);
        } else {
            for (; *line && *token && (*line == *token); ++line, ++token);
            *line = 0;
        }
        ++pc->hits;
    }

    return pc->hits;
}

static unsigned int command_complete_add(private_context_t * pc, const gs_command_t * cmd, bool exact)
{
    if (cmd) {
        pc->cmd = cmd;
        return gs_command_completer_add_token(&pc->context, cmd->name, exact);
    } else {
        return gs_command_completer_add_token(&pc->context, NULL, exact);
    }
}

static gs_error_t command_complete(const gs_command_t * const cmds, int cmd_count, int arg_offset, private_context_t * pc)
{
    if (arg_offset > 0) {
        // command we are looking for must be in this block
        pc->hits = 0;
    }
    pc->cmd = NULL;
    bool exact_match = false;

    for (int i = 0; i < cmd_count; i++) {
        const gs_command_t * cmd = &cmds[i];

        if (cmd_read_int(&cmd->mode) & GS_COMMAND_FLAG_HIDDEN) {
            continue;
        }

        if (gs_string_empty(pc->context.argv[arg_offset])) {
            // exceeding known token(s) - partial match
            command_complete_add(pc, cmd, false);
            continue;
        }

        if (pc->requires_exact_match || ((arg_offset+1) < pc->context.argc)) {
            // must be an exact match
            if (cmd_strcmp(pc->context.argv[arg_offset], cmd->name) == 0) {
                command_complete_add(pc, cmd, true);
                exact_match = true;
                break;
            }
        } else if (cmd_strncmp(pc->context.argv[arg_offset], cmd->name,
                               strlen(pc->context.argv[arg_offset])) == 0) {
            // partial match
            command_complete_add(pc, cmd, false);
        }
    }

    if (exact_match || ((arg_offset > 0) && (pc->hits == 1))) {
        command_complete_add(pc, NULL, true);

        if (strlen(pc->complete.line) > strlen(pc->context.command_line)) {
            return GS_OK;
        }

        if (pc->cmd->chain.list) {
            return command_complete(pc->cmd->chain.list, pc->cmd->chain.count, arg_offset+1, pc);
        }

        // command arguments
        pc->context.argc -= arg_offset;
        pc->context.argv = &pc->context.argv[arg_offset];
        pc->context.command = pc->cmd;

        // add the "already" completed ones
        int arg_to_complete = 1;
        for (; arg_to_complete < (pc->context.argc - 1); ++arg_to_complete) {
            gs_command_completer_add_token(&pc->context, pc->context.argv[arg_to_complete], true);
        }
        // add the last - if its completed (space after)
        if ((arg_to_complete < pc->context.argc) && pc->requires_exact_match) {
            // cppcheck-suppress unreadVariable - not used on __AVR__ because it doesn't support 'completer'
            gs_command_completer_add_token(&pc->context, pc->context.argv[arg_to_complete], true);
            ++arg_to_complete;
        }

#if (__AVR__ == 0)
        if (pc->cmd->completer) {
            pc->hits = 0;
            (pc->cmd->completer)(&pc->context, arg_to_complete);
        } else
#endif
        {
            pc->hits = 1; // no completer - assume single hit
        }

        return GS_OK; // only used for breaking loop
    }

    return GS_ERROR_NOT_FOUND;
}

gs_error_t gs_command_complete(char *line, size_t max_line_length, FILE* out)
{
    const size_t line_len = strlen(line);
    char buffer[max_line_length];
    buffer[0] = 0;
    private_context_t pc = {
        .process = command_complete,
        .context = {
            .command_line = line,
            .out = out,
        },
        .complete = {
            .line = buffer,
        },
    };
    command_process(&pc);
    gs_command_completer_add_token(&pc.context, NULL, true);
    if (strlen(buffer) > line_len ) {
        strcpy(line, buffer);
    }
    switch (pc.hits) {
        case 0:
            return GS_ERROR_NOT_FOUND;
        case 1:
            return GS_OK;
        default:
            return GS_ERROR_AMBIGUOUS;
    }
}

static void command_help_print(const gs_command_t * const cmd, private_context_t * pc)
{
    if (pc->hits == 1) {
        if (cmd->help) {
            fprintf(pc->context.out, "%s\r\n", cmd->help);
        }
        if (cmd->chain.count == 0) {
            fprintf(pc->context.out, "usage: %s %s\r\n", cmd->name, cmd->usage ? cmd->usage : "");
        } else {
            for (unsigned int i = 0; i < cmd->chain.count; ++i) {
                const gs_command_t * scmd = &cmd->chain.list[i];
                
                if (scmd->mode & GS_COMMAND_FLAG_HIDDEN) {
                    continue;
                }
                fprintf(pc->context.out, "  %-19s %s\r\n", scmd->name, scmd->help ? scmd->help : "");
            }
        }
    } else {
        fprintf(pc->context.out, "  %-19s %s\r\n", cmd->name, cmd->help ? cmd->help : "");
    }
}

static void command_help_hit(const gs_command_t * const cmd, private_context_t * pc)
{
    pc->error = GS_OK;
    ++pc->hits;
    if (pc->hits == 1) {
        // single hit so far - hold off printing until we know if we get more
        pc->cmd = cmd;
    } else {
        if (pc->cmd) {
            command_help_print(pc->cmd, pc);
            pc->cmd = NULL;
        }
        command_help_print(cmd, pc);
    }
}

static gs_error_t command_help(const gs_command_t * const cmds, int cmd_count, int arg_offset, private_context_t * pc)
{
    for (int i = 0; i < cmd_count; i++) {
        const gs_command_t * cmd = &cmds[i];

        if (cmd_read_int(&cmd->mode) & GS_COMMAND_FLAG_HIDDEN) {
            continue;
        }

        if (pc->requires_exact_match || ((arg_offset+1) < pc->context.argc)) {
            // must be an exact match
            if (cmd_strcmp(pc->context.argv[arg_offset], cmd->name) == 0) {
                const gs_command_t * list = (void*) cmd_read_ptr(&cmd->chain.list);
                if (list && ((arg_offset+1) < pc->context.argc)) {
                    return command_help(list, cmd_read_int(&cmd->chain.count), arg_offset+1, pc);
                }
                command_help_hit(cmd, pc);
            }

        } else if (cmd_strncmp(pc->context.argv[arg_offset], cmd->name,
                               strlen(pc->context.argv[arg_offset])) == 0) {
            command_help_hit(cmd, pc);
        }
    }

    return GS_ERROR_NOT_FOUND;
}

gs_error_t gs_command_show_help(const char * command, FILE* out)
{
    private_context_t pc = {
        .process = command_help,
        .error = GS_ERROR_NOT_FOUND,
        .context = {
            .command_line = command,
            .out = out,
        }
    };
    gs_error_t error = command_process(&pc);
    if (pc.cmd) {
        command_help_print(pc.cmd, &pc);
        error = GS_OK;
    } else if ((error == GS_ERROR_NOT_FOUND) && pc.hits) {
        error = GS_OK;
    }
    return error;
}

gs_error_t gs_command_register(const gs_command_t * commands, size_t count)
{
    GS_CHECK_ARG(commands != NULL);
    GS_CHECK_ARG(count > 0);

    gs_error_t error = GS_OK;

    gs_lock_lock();
    {
        // check if command block already installed
        gs_command_block_t * last_block = NULL;
        for (gs_command_block_t * block = &g_commands; block; block = block->next) {
            if (block->commands) {
                const gs_command_t * cmd = block->commands;
                // loop through because it may be in the linked blocks
                for (size_t i = 0; i < block->count; ++i, ++cmd) {
                    if (cmd == commands) {
                        error = GS_ERROR_EXIST;
                        break;
                    }
                }
            }
            last_block = block;
        }

        if (error == GS_OK) {
            gs_command_block_t * block = calloc(1, sizeof(*block));
            if (block) {
                // Insert command last, so lock isn't needed when accessing commands
                block->commands = commands;
                block->count = count;
                block->next = NULL;
                last_block->next = block;
            } else {
                error = GS_ERROR_ALLOC;
            }
        }
    }
    gs_lock_unlock();

    return (error != GS_ERROR_EXIST) ? error : GS_OK;
}

size_t gs_command_get_stack_size(void)
{
    return g_stack_size;
}

gs_error_t gs_command_init_no_commands(size_t stack_size)
{
    g_stack_size = stack_size;

    gs_error_t error = gs_lock_init();
    if (error) {
        return error;
    }

    gs_log_group_register(gs_command_log);

#if (__linux__ == 0)
    // look up static linked commands - only embedded (= none linux) systems
    gs_command_block_t * block = &g_commands;
    extern volatile unsigned int __command_start __attribute__ ((__weak__));
    extern volatile unsigned int __command_end __attribute__ ((__weak__));
    if (&__command_start) {
        block->count = ((ptrdiff_t)&__command_end - (ptrdiff_t)&__command_start) / sizeof(gs_command_t);
        block->commands = (gs_command_t *) &__command_start;
    }
#endif

    return GS_OK;
}

gs_error_t gs_command_init(size_t stack_size)
{
    gs_error_t error = gs_command_init_no_commands(stack_size);
    if (error == GS_OK) {
        // register default commands
        gs_command_register_default_commands();
        gs_log_register_commands();
    }
    return error;
}

gs_error_t gs_command_logger_default(const char* cmd_line, gs_error_t ret, gs_error_t cmd_ret, gs_timestamp_t t_start, gs_timestamp_t t_end, void *log_ctx)
{
    (void)log_ctx;

    timestamp_diff(&t_end, &t_start);
    if (ret == GS_OK) {
        log_info_group(gs_command_log, "'%s' returned '%s' ["
            "t: <%04"PRIu32".%06"PRIu32">, dt: <%01"PRIu32".%06"PRIu32">]",
            cmd_line, gs_error_string(cmd_ret),
            t_start.tv_sec, t_start.tv_nsec/1000, t_end.tv_sec, t_end.tv_nsec/1000);
    } else {
        log_info_group(gs_command_log, "'%s' could not be run, returned '%s' ["
            "t: <%04"PRIu32".%06"PRIu32">]",
            cmd_line, gs_error_string(ret),
            t_start.tv_sec, t_start.tv_nsec/1000);
    }
    return GS_OK;
}

gs_error_t gs_command_register_logger(gs_command_log_t log_cb, void *log_ctx)
{
    gs_lock_lock();
    g_command_logger = log_cb;
    g_command_logger_ctx = log_ctx;
    gs_lock_unlock();

    return GS_OK;
}

