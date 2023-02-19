/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   The console interface provides support for executing commands over stdout (typically a serial port).

   The connection can run in 2 modes:
   - normal, standard GOSH interface (Human Machine Interface), echo characters, prompt, etc.
   - cci, Computer Computer Interface. Simple text interface, but with tagged output format - easier to parse by a computer.
*/
#include "console_local.h"
#include "command_local.h"

#include <gs/util/string.h>
#include <gs/util/stdio.h>
#include <gs/util/thread.h>
#include <gs/util/mutex.h>
#include <gs/util/time.h>
#include <gs/util/log/appender/console.h>
#include <stdlib.h>
#include <ctype.h>

#if (__linux__)
#include <gs/util/linux/signal.h>
#include <gs/util/linux/command_line.h>
#include <unistd.h>
#include <termios.h>
#endif

#if (GS_CONSOLE_INPUT_LEN < 2)
#error GS_CONSOLE_INPUT_LEN must at least be 2 bytes
#endif

#define CONTROL(X)  ((X) - '@')

typedef enum {
    CONSOLE_NORMAL = 0,
    CONSOLE_ESCAPE = 1,
    CONSOLE_PRE_ESCAPE = 2,
} console_escape_t;

static const char hash_prompt[]  = "\033[1;30m # ";

static const char backspace_char = '\b';
static const char space_char     = ' ';
static const char cr_char        = '\r';
static const char nl_char        = '\n';

static const char * user_prompt  = "gosh";

static console_escape_t escape   = CONSOLE_NORMAL;

#if (GS_CONSOLE_HISTORY_LEN > 0)
static int history_elements;
static int history_cur;
static int history_browse;
static char history[GS_CONSOLE_HISTORY_LEN][GS_CONSOLE_INPUT_LEN+1];
#endif

static int size;
static int pos;
static char buf[GS_CONSOLE_INPUT_LEN+1];
static gs_thread_t console_thread;

#if (__linux__)
static bool termios_changed;
static struct termios old_stdin;
static struct termios old_stdout;
#endif

static gs_mutex_t g_cci_lock; // Lock for protecting stdout for async output, e.g. log messages
static gs_error_t command_io_cci_set_result(gs_command_context_t *ctx, const char *group, const char *key, const char *value);
static const gs_command_io_functions_t cci_io_functions = {
    .set_result   = command_io_cci_set_result,
    .flush        = gs_command_stdio_flush,
    .wait_for_key = gs_command_stdio_wait_for_key,
};
#define CCI_START_TAG "[X["
#define CCI_END_TAG   "]X]"

static void gs_console_write(const char *str, int length)
{
    for (int i = 0; i < length; i++) {
        putchar(str[i]);
    }
}

static void gs_console_prompt(void)
{
    static const char col_start[]    = "\033[1;32m";
    static const char col_end[]      = "\033[0m";

    gs_console_write(col_start, sizeof(col_start) - 1);
    gs_console_write(user_prompt, strlen(user_prompt));
    gs_console_write(hash_prompt, sizeof(hash_prompt) - 1);
    gs_console_write(col_end, sizeof(col_end) - 1);
}

void gs_console_set_prompt(const char * _prompt)
{
    if (gs_string_empty(_prompt) == false) {
        user_prompt = _prompt;
    }
}

static void gs_console_reset(void)
{
    pos = size = 0;
    buf[pos] = 0;
    gs_console_prompt();
}

static void gs_console_rewind(void)
{
    int plen = strlen(hash_prompt) + strlen(user_prompt);
    gs_console_write(&cr_char, 1);
    while (size-- + plen) {
        gs_console_write(&space_char, 1);
    }
    pos = size = 0;
    gs_console_write(&cr_char, 1);
}

void gs_console_clear(void)
{
    static const char clear[] = "\033[H\033[2J";
    gs_console_write(clear, sizeof(clear) - 1);
    gs_console_rewind();
    gs_console_reset();
}

void gs_console_update(void)
{
    gs_console_rewind();
    gs_console_prompt();
    pos = size = strlen(buf);
    gs_console_write(buf, size);
}

#if (GS_CONSOLE_HISTORY_LEN > 0)

static void gs_console_history_add(void)
{
    strncpy(history[history_cur], buf, GS_CONSOLE_INPUT_LEN);
    history[history_cur][GS_CONSOLE_INPUT_LEN] = 0;

    history_browse = 0;
    history_cur = (history_cur + 1) % GS_CONSOLE_HISTORY_LEN;

    if (history_elements < GS_CONSOLE_HISTORY_LEN) {
        history_elements++;
    }
}

static void gs_console_last_line(void)
{
    if (history_elements < 1) {
        return;
    }

    if (history_browse >= history_elements) {
        return;
    }

    gs_console_rewind();
    history_browse++;
    strcpy(buf, history[(history_cur - history_browse + GS_CONSOLE_HISTORY_LEN) % GS_CONSOLE_HISTORY_LEN]);
    gs_console_update();
}

static void gs_console_next_line(void)
{
    if (history_elements < 1) {
        return;
    }

    if (history_browse < 1) {
        return;
    }

    gs_console_rewind();
    history_browse--;
    if (history_browse > 0) {
        strcpy(buf, history[(history_cur - history_browse + GS_CONSOLE_HISTORY_LEN) % GS_CONSOLE_HISTORY_LEN]);
    } else {
        buf[0] = '\0';
    }
    gs_console_update();
}

#endif

static void gs_console_forward_char(void)
{
    if (pos < size) {
        gs_console_write(&buf[pos], 1);
        pos++;
    }
}

static void gs_console_end_of_line(void)
{
    while (pos < size) {
        gs_console_forward_char();
    }
}

static void gs_console_backward_char(void)
{
    if (pos > 0) {
        pos--;
        gs_console_write(&backspace_char, 1);
    }
}

static void gs_console_beginning_of_line(void)
{
    while (pos) {
        gs_console_backward_char();
    }
}

static void gs_console_newline(void)
{
    gs_console_write(&cr_char, 1);
    gs_console_write(&nl_char, 1);
}

static bool gs_command_not_empty(const char *ibuf)
{
    while (*ibuf) {
        if (!isblank((int) *ibuf++)) {
            return true;
        }
    }
    return false;
}

static void show_help(const char * command)
{
    gs_error_t error = gs_command_show_help(command, stdout);
    if (error) {
        printf("Could not show help for \'%s\': %s (%d)\r\n", command, gs_error_string(error), error);
    }
}

static void gs_console_execute(void)
{
    gs_console_newline();
    buf[GS_CONSOLE_INPUT_LEN] = 0; // ensure 0 termination
    if (size > 0 && gs_command_not_empty(buf)) {
#if (GS_CONSOLE_HISTORY_LEN > 0)
        gs_console_history_add();
#endif
        const uint32_t start_ms = gs_time_rel_ms();
        gs_error_t result = GS_OK;
        gs_error_t error = gs_command_execute_stdio(buf, &result);
        const uint32_t elapsed_ms = gs_time_diff_ms(start_ms, gs_time_rel_ms());
        if (error == GS_ERROR_TYPE) {
            show_help(buf);
        } else if (error == GS_ERROR_NOT_FOUND) {
            printf("Unknown command \'%s\'\r\n", buf);
        } else if (error == GS_ERROR_ARG) {
            show_help(buf);
        } else if (error) {
            printf("Command \'%s\' did not execute: %s (%d)\r\n", buf, gs_error_string(error), error);
        } else if (result == GS_ERROR_ARG) {
            show_help(buf);
        } else if (result) {
            printf("Command \'%s\' executed, but returned error: %s (%d) after %"PRIu32" mS\r\n",
                   buf, gs_error_string(result), result, elapsed_ms);
        }
    }
    gs_console_reset();
}

static void gs_console_complete(void)
{
    /* We don't expand in the middle of a line */
    if (size != pos) {
        return;
    }

    const size_t old_buf_len = strlen(buf);
    gs_error_t ret = gs_command_complete(buf, sizeof(buf), stdout);
    if ((ret == GS_OK) && (old_buf_len == strlen(buf))) {
        // completed (again) and no change - show help
        ret = GS_ERROR_AMBIGUOUS;
    }
    switch (ret) {
        case GS_ERROR_AMBIGUOUS:
            gs_console_newline();
            show_help(buf);
            gs_console_update();
            break;
        case GS_OK:
            gs_console_update();
            break;
        default:
        case GS_ERROR_NOT_FOUND:
            break;
    }
}

static void gs_console_insert(char c)
{
    int i;
    int diff = size - pos;

    if (size >= GS_CONSOLE_INPUT_LEN) {
        return;
    }

    memmove(&buf[pos + 1], &buf[pos], diff);
    buf[pos] = c;

    gs_console_write(&buf[pos], diff + 1);
    for (i = 0; i < diff; i++) {
        gs_console_write(&backspace_char, 1);
    }

    size++;
    pos++;
    buf[size] = '\0';
}

static void gs_console_insert_overwrite(char c)
{
    buf[pos++] = c;

    if (pos > size) {
        size++;
    }

    gs_console_write(&c, 1);
}

static void gs_console_delete(void)
{
    int i;
    int diff = size - pos;

    /* Nothing to delete */
    if (size == pos) {
        return;
    }

    size--;
    memmove(&buf[pos], &buf[pos + 1], diff - 1);
    buf[size] = '\0';

    gs_console_write(&buf[pos], diff - 1);
    gs_console_write(&space_char, 1);
    for (i = 0; i < diff; i++) {
        gs_console_write(&backspace_char, 1);
    }
}

static void gs_console_backspace(void)
{
    if (pos < 1) {
        return;
    }

    gs_console_backward_char();
    gs_console_delete();
}

static void gs_console_kill_line(void)
{
    int i;
    int diff;

    diff = size - pos;

    if (diff == 0) {
        return;
    }

    for (i = 0; i < diff; i++) {
        gs_console_write(&space_char, 1);
    }
    for (i = 0; i < diff; i++) {
        gs_console_write(&backspace_char, 1);
    }

    memset(&buf[pos], 0, diff);
    size = pos;
}

static void gs_console_kill_line_from_beginning(void)
{
    gs_console_beginning_of_line();
    gs_console_kill_line();
}

static void gs_console_backward_kill_word(void)
{
    while (pos > 0 && buf[pos - 1] == ' ') {
        gs_console_backspace();
    }
    while (pos > 0 && buf[pos - 1] != ' ') {
        gs_console_backspace();
    }
}

static void gs_console_transpose_chars(void)
{
    char c1, c2;

    if (size < 2 || pos < 1) {
        return;
    }

    if (pos == size) {
        c1 = buf[pos - 1];
        c2 = buf[pos - 2];

        gs_console_backward_char();
        gs_console_backward_char();
        gs_console_insert_overwrite(c1);
        gs_console_insert_overwrite(c2);
    } else {
        c1 = buf[pos];
        c2 = buf[pos - 1];

        gs_console_backward_char();
        gs_console_insert_overwrite(c1);
        gs_console_insert_overwrite(c2);
    }
}

static void gs_console_normal(char c)
{
    switch (c) {
        case CONTROL('A'):
            gs_console_beginning_of_line();
            break;
        case CONTROL('B'):
            gs_console_backward_char();
            break;
        case CONTROL('C'):
            // Either ignored or handled through signals
            break;
        case CONTROL('D'):
            gs_console_delete();
            break;
        case CONTROL('E'):
            gs_console_end_of_line();
            break;
        case CONTROL('F'):
            gs_console_forward_char();
            break;
        case CONTROL('K'):
            gs_console_kill_line();
            break;
        case CONTROL('L'):
            gs_console_clear();
            break;
#if (GS_CONSOLE_HISTORY_LEN > 0)
        case CONTROL('N'):
            gs_console_next_line();
            break;
        case CONTROL('P'):
            gs_console_last_line();
            break;
#endif
        case CONTROL('T'):
            gs_console_transpose_chars();
            break;
        case CONTROL('U'):
            gs_console_kill_line_from_beginning();
            break;
        case CONTROL('W'):
            gs_console_backward_kill_word();
            break;
        case CONTROL('Z'):
            // We cannot suspend
            break;
        case CONTROL('H'):
        case 0x7f:
            gs_console_backspace();
            break;
        case '\r':
        case '\n':
            gs_console_execute();
            break;
        case '\t':
            gs_console_complete();
            break;
        case '\033':
            escape = CONSOLE_ESCAPE;
            break;
        default:
            if (escape == CONSOLE_ESCAPE) {
                if ((c == '[') || (c == 'O')) {
                    c = getchar();
                    if (c == 'F')
                        gs_console_end_of_line();
                    if (c == 'H')
                        gs_console_beginning_of_line();
#if (GS_CONSOLE_HISTORY_LEN > 0)
                    if (c == 'A')
                        gs_console_last_line();
                    if (c == 'B')
                        gs_console_next_line();
#endif
                    if (c == 'C')
                        gs_console_forward_char();
                    if (c == 'D')
                        gs_console_backward_char();
                    if (c == '1')
                        if (getchar() == '~')
                            gs_console_beginning_of_line();
                    if (c == '3')
                        if (getchar() == '~')
                            gs_console_delete();
                }
                escape = CONSOLE_NORMAL;
                break;
            }

            if (isprint((unsigned char) c)) {
                gs_console_insert(c);
            }

            break;
    }
}

static gs_error_t command_io_cci_set_result(gs_command_context_t *ctx, const char *group, const char *key, const char *value)
{
    gs_mutex_lock(g_cci_lock);
    {
        printf(CCI_START_TAG "cmd_res,%s,%s,%s" CCI_END_TAG, group, key, value);
    }
    gs_mutex_unlock(g_cci_lock);
    return GS_OK;
}

static void gs_console_cci_log(gs_log_appender_t *appender, gs_log_level_t level, const gs_log_group_t *group, const gs_timestamp_t * ts, const char * format, va_list va)
{
    va_list my_va;
    va_copy(my_va, va);

    gs_mutex_lock(g_cci_lock);
    {
        printf(CCI_START_TAG "log,%04"PRIu32".%06"PRIu32",%c,%s,", ts->tv_sec, ts->tv_nsec / 1000, gs_log_level_to_char(level), group->name);
        vprintf(format, my_va);
        printf(CCI_END_TAG "\r\n");
    }
    gs_mutex_unlock(g_cci_lock);

    va_end(my_va);
}

static void gs_console_cci(char c)
{
    switch (c) {
        case CONTROL('C'):
        case CONTROL('L'):
            size = 0;
            buf[0] = 0;
            break;
        case '\r':
        case '\n':
            buf[GS_CONSOLE_INPUT_LEN] = 0; // ensure 0 termination
            if (size > 0 && gs_command_not_empty(buf)) {
                static unsigned int seq; // simple sequence number keep incrementing

                gs_mutex_lock(g_cci_lock);
                ++seq;
                printf(CCI_START_TAG "cmd_exec_begin,%u,%s" CCI_END_TAG "\r\n", seq, buf);
                gs_mutex_unlock(g_cci_lock);

                gs_error_t result = GS_OK;
                gs_error_t error = gs_command_execute(buf, &result, stdout, &cci_io_functions, NULL);

                gs_mutex_lock(g_cci_lock);
                printf(CCI_START_TAG "cmd_exec_end,%u,%d,%d" CCI_END_TAG "\r\n", seq, error, result);
                gs_mutex_unlock(g_cci_lock);
            }
            size = 0;
            buf[0] = 0;
            break;
        default:
            if (isprint((unsigned char) c) && (size < GS_CONSOLE_INPUT_LEN)) {
                buf[size++] = c;
                buf[size] = 0;
            }
            break;
    }
}

// Currrent mode handler, switch by sending command
static void (*console_handler)(char c) = gs_console_normal;

int gs_console_change_mode(const char * mode)
{
    if (strcasecmp(mode, "cci") == 0) {
        gs_error_t error = GS_OK;
        if (console_handler != gs_console_cci) {
            error = gs_mutex_create(&g_cci_lock);
            if (error == GS_OK) {
                gs_log_appender_console_set_cb(gs_console_cci_log);
                console_handler = gs_console_cci; // change console handler
            }
        }
        return error;
    }
    return GS_ERROR_NOT_SUPPORTED;
}

static void * gs_console_thread(void * param)
{
#if (__AVR__ == 0)
    // Disable buffering - done from console-thread in order to support newlib::reentrant correctly.
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
#endif

    gs_console_reset();

    while (1) {
        char c = getchar();
        console_handler(c);
    }

    gs_thread_exit(NULL);
}

gs_error_t gs_console_exit(void)
{
#if (__linux__)
    if (termios_changed) {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_stdin);
        tcsetattr(STDOUT_FILENO, TCSANOW, &old_stdout);
    }
#endif
    return GS_OK;
}

#if (__linux__)
static inline void exithandler(void)
{
    printf("\n");
    gs_console_exit();
}
#endif

static gs_error_t gs_console_init2(uint32_t flags)
{
#if (__linux__)
    // save current stdio setting, for restoring when terminating process
    tcgetattr(STDIN_FILENO, &old_stdin);
    tcgetattr(STDOUT_FILENO, &old_stdout);

    // change stdin settings
    {
        struct termios new = old_stdin;
        new.c_iflag &= ~(IGNCR | ICRNL);
        new.c_lflag &= ~(ECHO | ICANON | IEXTEN);
        new.c_cc[VTIME]=0;
        new.c_cc[VMIN]=1;
        tcsetattr(STDIN_FILENO, TCSANOW, &new);
    }
    // change stdout settings
    {
        struct termios new = old_stdout;
        new.c_iflag &= ~(IGNCR | ICRNL);
        new.c_lflag &= ~(ECHO | ICANON | IEXTEN);
        new.c_cc[VTIME]=0;
        new.c_cc[VMIN]=1;
        tcsetattr(STDOUT_FILENO, TCSANOW, &new);
    }

    termios_changed = true;

    // add exit-handler to restore original termianl settings
    atexit(exithandler);

    // install signal handlers to ensure terminal settings are restored
    if ((flags & GS_CONSOLE_F_NO_SIGNAL_HANDLER) == 0) {
        // install signal handler(s) to ensure atexit() is called
        gs_signal_catch(SIGTERM, NULL);

        if (gs_command_line_ignore_ctrlc() == false) {
            gs_signal_catch(SIGINT, NULL);
        }
    }
#endif

    return GS_OK;
}

gs_error_t gs_console_init()
{
    return gs_console_init2(0);
}

static gs_error_t _console_create_thread(gs_thread_priority_t priority, gs_thread_t * handle, uint32_t thread_create_flags)
{
    gs_error_t error = gs_thread_create("CONSOLE",
                                        gs_console_thread, NULL,
                                        gs_command_get_stack_size(),
                                        priority,
                                        thread_create_flags,
                                        handle);
    if (error == GS_OK) {
        // give thread a few moments to print prompt
        gs_time_sleep_ms(20);
    }
    return error;
}

gs_error_t gs_console_create_thread(gs_thread_t * handle)
{
    return _console_create_thread(GS_THREAD_PRIORITY_LOW, handle, 0);
}

gs_error_t gs_console_create_thread_with_priority(gs_thread_priority_t priority, gs_thread_t * handle)
{
    return _console_create_thread(priority, handle, 0);
}

gs_error_t gs_console_start(const char * prompt, uint32_t flags)
{
    if (console_thread) {
        return GS_ERROR_EXIST;
    }

    gs_console_init2(flags);
    gs_console_set_prompt(prompt);

    return _console_create_thread(GS_THREAD_PRIORITY_LOW, &console_thread, GS_THREAD_CREATE_JOINABLE);
}

gs_error_t gs_console_stop(void)
{
    if (console_thread == 0) {
        return GS_ERROR_HANDLE;
    }
#if (__linux__)
    if (pthread_cancel(console_thread) != 0) {
        return GS_ERROR_IO;
    }
    gs_error_t error = gs_thread_join(console_thread, NULL);
    if (error == GS_OK) {
        console_thread = 0;
    }
    return error;
#else
    return GS_ERROR_NOT_SUPPORTED;
#endif
}
