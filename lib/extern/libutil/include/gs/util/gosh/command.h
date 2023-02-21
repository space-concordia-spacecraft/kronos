#ifndef GS_UTIL_GOSH_COMMAND_H
#define GS_UTIL_GOSH_COMMAND_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Command framework.

   Provides a simple way of organizing commands in a hierarchy. A command is a text string mapping to a function - supporting arguments.
*/

#include <gs/util/stdio.h>
#include <gs/util/pgm.h>
#include <gs/util/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Max langth of a command (including NUL termination).
*/
#define GS_COMMAND_MAX_LEN_COMMAND      20

/**
   Flag for hiding command in help and tab-complete.
*/
#define GS_COMMAND_FLAG_HIDDEN		0x02

/**
   'root' command attribute.

   On embedded (none Linux) systems, it is prefered to store as much as possible in \a program memory, in order to save RAM.
   This is accomplished by tagging all \a root commands with this attribute, which instructs the linker to put all commands in a named
   section. This section is then through the linker-script, placed in \a program memory.
   The command framework can read commands directly from this section, and therefore doesn't need an RAM to maintain the list.

   The gs_command_register() must still be called for all \a root commands, which ensures that the linker doesn't throw away the
   command objects, due to missing code reference.

   On a Linux system, the commands are not group in a section. Instead gs_command_register() dynamicly builds a list with the commands.

   @see gs_command_register()
*/
#if (__linux__ == 0)
#define GS_COMMAND_ROOT __attribute__ ((section(".commands")))
#else
#define GS_COMMAND_ROOT 
#endif

/**
   Sub command attribute,

   Only necesasry on AVR8, due to its memory model.
*/
#define GS_COMMAND_SUB  GS_PGM_OBJECT

/**
   Macro for initializing command chains.
*/
#define GS_COMMAND_INIT_CHAIN(__list) {.list = __list, .count = GS_ARRAY_SIZE(__list)}

/**
   Macro for registering commands.

   @see gs_command_register()
*/
#define GS_COMMAND_REGISTER(__cmd) gs_command_register(__cmd, GS_ARRAY_SIZE(__cmd))

/**
   Command reference.
   @note Use gs_command_t instead of 'struct command'.
*/
typedef struct command gs_command_t;

/**
    Commands context reference
    @note Use gs_command_context_t instead of struct command_context
 */
typedef struct command_context gs_command_context_t;

/**
    Command output interface
*/
typedef struct command_io_functions {
    /**
       Function interface for setting result
       @param output_ctx pointer to output context for the given impl.
       @param group Group name specifies the group that a given key/value pair belongs to.
       @param key key name
       @param value string value of the result
       @return_gs_error_t
    */
    gs_error_t (*set_result)(gs_command_context_t *ctx, const char *group, const char *key, const char *value);
    /**
       Function interface for flushing results. Used by the command handler to ensure output/results
       are flushed to stdout/File or any other receiver of the output.
       @param output_ctx pointer to output context for the given impl.
       @return_gs_error_t
    */
    gs_error_t (*flush)(gs_command_context_t *ctx);
    /**
       Function interface for waiting for key/input
       @param output_ctx pointer to output context for the given impl.
       @param ch pointer to character returned by function
       @param timeout_ms maximum time to wait of the character.
       @return_gs_error_t
    */
    gs_error_t (*wait_for_key)(gs_command_context_t *ctx, int *ch, int timeout_ms);
} gs_command_io_functions_t;



/**
   Command context for executing a command.
*/
struct command_context {
    /**
       Input (raw) command line, including arguments.
    */
    const char * command_line;

    /**
       Command being executed.
    */
    const gs_command_t * command;

    /**
       Number of arguments (standard argc style).
    */
    int argc;

    /**
       Argument array (standard argv style).
    */
    const char * const * argv;

    /**
       FILE handle for capturing stdout from command.
     */
    FILE* out;

     /**
       getopt variable.
    */
    int optind;

    /**
       getopt variable.
    */
    int optopt;

    /**
       getopt variable.
     */
    const char *optarg;

    /**
        getopt variable.
    */
    int optsp;

    /**
       Function interface for I/O operations
    */
    const gs_command_io_functions_t *io_functions;

    /**
       Pointer for storing the context used by the I/O functions
    */
    void * io_ctx;
};

/**
   Command logging call-back

   logs information on the command called.
   @param[in] cmd_line command line string
   @param[in] ret return code from command execution framework
   @param[in] cmd_ret return code from the executed command
   @param[in] start time_stamp when command execution started.
   @param[in] end time_stamp when command execution completed.
   @param[in] ctx context pointer for the logger.
   @return_gs_error_t
*/
typedef gs_error_t (*gs_command_log_t)(const char *cmd_line, gs_error_t ret, gs_error_t cmd_ret, gs_timestamp_t start, gs_timestamp_t end, void * ctx);

/**
   Command handler.
*/
typedef int (*gs_command_handler_t)(gs_command_context_t * ctx);

/**
   Completer call-back (tab complete).

   @param[in] ctx command context.
   @param[in] arg_to_complete argument to complete
   @return #GS_OK  Found at least 1 match.
   The \a completer is expected to have completed more of the command line.
   If the framework detects multiple matches, the framework will proceed as if #GS_ERROR_AMBIGUOUS was returned.
   The framework doesn't expect anything to be printed to \a out, but will update/refresh the console line.
   @return #GS_ERROR_AMBIGUOUS Ambiguous - multiple matches or force the framework to show help.
   The \a completer may have extended/completed more of the command line.
   The framework expects the \a completer to have printed to \a out, and will show help/usage for the command on a new line.
   @return #GS_ERROR_NOT_FOUND (or others) No matches found or no more arguments to complete.
   The framewrok doesn't expect anything to be printed to \a out, and will not update the console.
*/
typedef gs_error_t (*gs_command_completer_t)(gs_command_context_t * ctx, int arg_to_complete);

/**
   Add token - helper to 'tab complete' argument(s).

   @param[in] ctx command context.
   @param[in] token possible completion - the API will find the common part.
   @param[in] exact \a true if \a token is an exact match - all other added tokens will be ignored.
   @return number of tokens added.
*/
unsigned int gs_command_completer_add_token(gs_command_context_t * ctx, const char * token, bool exact);

/**
   Chain element for chaning sub-commands.
*/
typedef struct {
    /**
       Command list.
    */
    const gs_command_t * list;
    /**
       Number of commands in the \a list.
    */
    unsigned int count;
} gs_command_chain_t;

/**
   Signals no command arguments in command definition, see mandatory arguments.
*/
#define GS_COMMAND_NO_ARGS 255

/**
   Command definition.
*/
struct command {
#if (__AVR__)
    char name[GS_COMMAND_MAX_LEN_COMMAND];
    char help[50];
    char usage[50];
#else
    /**
       Name.
    */
    const char * const name;
    /**
       Help text.
    */
    const char * const help;
    /**
       Usage text.
    */
    const char * const usage;
#endif
    /**
       Command handler - the "actual command function".
    */
    gs_command_handler_t handler;
#if (__AVR__ == 0)
    /**
       Completer function - helps completing an argument.
    */
    gs_command_completer_t completer;
#endif
    /**
       Sub-command (if any).
    */
    gs_command_chain_t chain;
    /**
       Mode/flags.
       See #GS_COMMAND_FLAG_HIDDEN.
    */
    unsigned int mode;
    /**
       Number of mandatory (minimum) arguments.

       @note Due to backwards compatibility, 0 (zero) cannot be used to signal no arguments - use #GS_COMMAND_NO_ARGS instead, if command doesn't take any arguments (mandatory or optional).
    */
    uint8_t mandatory_args;
    /**
       Number of optional arguments.
    */
    uint8_t optional_args;
    /**
       Filler for future use.
    */
    uint8_t filler[2];
};

/**
   Returns the arguments as a string, where arguments are separated by spaces.
   @param ctx command context.
   @return Pointer to concatenated arguments
*/
const char * gs_command_args(gs_command_context_t *ctx);

/**
   Execute command.
   @deprecated Replaced by gs_command_execute & gs_command_execute_stdio

   Looks up a command and executes it. If the command is executed (this function returns GS_OK), the command's return
   result is stored in \a command_result.

   @param[in] command Command to execute, including arguments separated by spaces.
   @param[out] command_result Result returned by \a command (if executed). Use \a NULL to ignore result.
   @return #GS_ERROR_NOT_FOUND if command wasn't found.
   @return #GS_ERROR_ARG if number of argumenets exceeds \a mandatory + \a optional count.
   @return_gs_error_t
*/
gs_error_t gs_command_run(const char * command, gs_error_t * command_result);

/**
   Execute command.

   Looks up a command and executes it. If the command is executed (this function returns GS_OK), the command's return
   result is stored in \a command_result.

   @param[in] command Command to execute, including arguments separated by spaces.
   @param[out] command_result Result returned by \a command (if executed). Use \a NULL to ignore result.
   @param[in] out output (FILE) stream
   @param[in] iof Pointer to function interface of IO operations
   @param[in] iof_ctx Pointer to context used by the IO function interface
   @return #GS_ERROR_NOT_FOUND if command wasn't found.
   @return #GS_ERROR_ARG if number of argumenets exceeds \a mandatory + \a optional count.
   @return_gs_error_t
*/
gs_error_t gs_command_execute(const char * command, gs_error_t * command_result, FILE *out, const gs_command_io_functions_t * iof, void * iof_ctx);

/**
   Execute command.

   Looks up a command and executes it. If the command is executed (this function returns GS_OK), the command's return
   result is stored in \a command_result. The results are printed on stdout and input captured on stdin.

   @param[in] command Command to execute, including arguments separated by spaces.
   @param[out] command_result Result from command. Use \a NULL to ignore result.
   @return #GS_OK if command was executed - result returned in \a command_result. Otherwise an error indicating why the command wasn't executed.
*/
gs_error_t gs_command_execute_stdio(const char * command, gs_error_t * command_result);

/**
    Set output

    Sets output from command, using the io function struct in ctx.

    @param[in] ctx the command context
    @param[in] group a string specifying the group of the result. Leave blank if not used.
    @param[in] key a string specifying the key/name of the result variable.
    @param[in] value a string representation of the result value.
    @return_gs_error_t
*/
gs_error_t gs_command_set_output(gs_command_context_t *ctx, const char* group, const char* key, const char* value);

/**
    Set output

    Sets output from command using printf formatting, using the io function struct in ctx.

    @param[in] ctx the command context
    @param[in] group a string specifying the group of the result. Leave blank if not used.
    @param[in] key a string specifying the key/name of the result variable.
    @param[in] format printf syntax for formatting data
    @return_gs_error_t
*/
gs_error_t gs_command_set_output_printf(gs_command_context_t *ctx, const char* group, const char* key, const char * format, ...);

/**
    Flush output/Results

    Instruct the command output stream & results to be flushed from it's buffers.

    @param[in] ctx the command context
    @return_gs_error_t
*/
gs_error_t gs_command_flush_output(gs_command_context_t *ctx);

/**
    Wait for any key input

    Instruct the command input stream to wait for any key.

    @param[in] ctx the command context
    @param[in] timeout_ms timeout, < 0: block forever, 0: poll, > 0: wait number of milliseconds.
    @return \a false on timeout, \a true if key pressed or an error occurred, e.g. no input stream available.
*/
bool gs_command_wait_any_key(gs_command_context_t *ctx, int timeout_ms);

/**
    Wait for key input

    Instruct the io stream to wait for a key, and return the pressed key in ch.

    @param[in] ctx the command context
    @param[out] ch the character that was read on the input stream
    @param[in] timeout_ms timeout, < 0: block forever, 0: poll, > 0: wait number of milliseconds.
    @return #GS_OK if key was read
    @return #GS_ERROR_HANDLE if no input stream is present
    @return #GS_ERROR_TIMEOUT on timeout.
*/
gs_error_t gs_command_wait_key(gs_command_context_t *ctx, int* ch, int timeout_ms);

/**
   Register commands.

   gs_command_init() must be called prior to registering any commands.

   See #GS_COMMAND_ROOT for details.

   @param[in] cmds Pointer to command array
   @param[in] cmd_count Number of commands in command array
   @return_gs_error_t
*/
gs_error_t gs_command_register(const gs_command_t * cmds, size_t cmd_count);

/**
   Initialize command system and register default commands.

   Registers following commands: gs_log_register_commands() and gs_command_register_default_commands().

   @param[in] min_stack_size Minimum stack size required for executing commands. The stack size will be used by other sub-systems such as gs_console, g-script. Stack size may be ignored on some platforms, e.g. Linux.
   @return_gs_error_t
   @see gs_command_init_no_commands()
*/
gs_error_t gs_command_init(size_t min_stack_size);

/**
   Initialize command system (without any default commands).

   @param[in] min_stack_size Minimum stack size required for executing commands. The stack size will be used by other sub-systems such as gs_console, g-script. Stack size may be ignored on some platforms, e.g. Linux.
   @return_gs_error_t
   @see gs_command_init()
*/
gs_error_t gs_command_init_no_commands(size_t min_stack_size);


/**
   Register a call-back used for logging of command execution.

   @param[in] log_cb the logging call back.
   @param[in] log_ctx pointer to context data. Set to NULL if not used.
   @return_gs_error_t
*/
gs_error_t gs_command_register_logger(gs_command_log_t log_cb, void *log_ctx);

/**
   Default implementation of the command logger, that can be used if no other
   custom command logger is provided by the system.

    @param[in] cmd_line command line string
    @param[in] ret return code provided by the command execution function.
    @param[in] cmd_ret return code provided by the executed command.
    @param[in] t_start time stamp when command execution started.
    @param[in] t_end time stamp when command execution completed.
    @param[in] log_ctx context for the command logger.
   @return_gs_error_t
*/
gs_error_t gs_command_logger_default(const char* cmd_line, gs_error_t ret, gs_error_t cmd_ret, gs_timestamp_t t_start, gs_timestamp_t t_end, void * log_ctx);

/**
   Return minimum stack size.
   @return minimm stack size required for executing commands. The minimum stack size is set by call to gs_command_init().
*/
size_t gs_command_get_stack_size(void);

/**
   Register set of default commands.
   @return_gs_error_t
*/
gs_error_t gs_command_register_default_commands(void);

/**
   Split line into argc/argv.

   @param[in] line line to split - the line will be chop up into argv.
   @param[out] argc argc count.
   @param[out] argv argv array.
   @param[in] max_argc max argv elements.
   @return \a true if successfull, else \a false.
*/
bool gs_command_build_argv(char *line, int *argc, char **argv, int max_argc);

/**
   Parse options.

   Adapted from AT&T public domain source from:
   http://www.informatica.co.cr/unix-source-code/research/1985/1103.html

   @param[in] ctx command context.
   @param[in] opts options
   @return option character
*/
int gs_command_getopt(gs_command_context_t *ctx, const char *opts);
    
#ifdef __cplusplus
}
#endif
#endif
