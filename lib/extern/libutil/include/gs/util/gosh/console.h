/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
#ifndef GS_UTIL_GOSH_CONSOLE_H
#define GS_UTIL_GOSH_CONSOLE_H
/**
   @file

   Console (stdin/stdout) interface for running commands.

   This assumes a VT102 terminal emulator, and tries to fix some of minicom's quirks with e.g. home/end keys.
*/

#include <gs/util/thread.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Initialize the API and console.

   @deprecated version 3.4, use gs_console_start()

   @return_gs_error_t
*/
gs_error_t gs_console_init(void);

/**
   Restores terminal settings (only relevant on Linux).

   @deprecated version 3.4, this is handled by an installed exit-handler in gs_console_start().

   @return_gs_error_t
*/
gs_error_t gs_console_exit(void);

/**
   Set console prompt.

   @param[in] prompt user prompt - the API only stores the pointer, so do not modify/delete content. NULL or empty string is ignored (no change).
*/
void gs_console_set_prompt(const char * prompt);

/**
   Clear the console screen
*/
void gs_console_clear(void);

/**
   Update console.
*/
void gs_console_update(void);

/**
   Create console thread.

   The console thread reads from stdin and writes to stdout.

   The thread is created with low priority, #GS_THREAD_PRIORITY_LOW.

   @deprecated version 3.4, use gs_console_start()

   @param[out] handle handle to created thread - thread will be created joinable if supported by platform. Use NULL, if not wanted.
   @return_gs_error_t
*/
gs_error_t gs_console_create_thread(gs_thread_t * handle);

/**
   Create console thread with priority.

   The console thread reads from stdin and writes to stdout.

   @deprecated version 3.4, use gs_console_start()

   @param[in] priority thread priority, normally #GS_THREAD_PRIORITY_LOW.
   @param[out] handle handle to created thread - thread will be created joinable if supported by platform. Use NULL, if not wanted.
   @return_gs_error_t
*/
gs_error_t gs_console_create_thread_with_priority(gs_thread_priority_t priority, gs_thread_t * handle);

/**
   @anchor GS_CONSOLE_F
   @defgroup GS_CONSOLE_F Console flags.
   Use with gs_console_start() to configure behaviour.
   @{
*/
/**
   Linux only: no signal handlers installed (e.g. to catch SIG_TERM).
   @see gs_console_start()
*/
#define GS_CONSOLE_F_NO_SIGNAL_HANDLER  0x0001
/** @} */

/**
   Start console thread (priority: #GS_THREAD_PRIORITY_LOW).

   The console thread reads from stdin and writes to stdout. The thread is created with low priority, #GS_THREAD_PRIORITY_LOW.

   Linux: Changes terminal settings and installs an atexit() handler to restore the settings, Signal handlers will be installed to catch SIG_TERM -> exit() and ignore SIG_INT (controlled by option on command line) - unless #GS_CONSOLE_F_NO_SIGNAL_HANDLER is specified.

   @param[in] prompt set console prompt by calling gs_console_set_prompt().
   @param[in] flags configure behaviour, see @ref GS_CONSOLE_F definitions.
   @return #GS_ERROR_EXIST if console thread already created.
   @return_gs_error_t
*/
gs_error_t gs_console_start(const char * prompt, uint32_t flags);

/**
   Stop (and join with) console thread.

   @note This is only supported on Linux.

   The thread is interrupted using pthread_cancel(), which does not guarantee \a clean shutdown if the thread is busy executing a command.

   @return #GS_ERROR_NOT_SUPPORTED if not supported on current platform.
   @return #GS_ERROR_HANDLE if no console has been started with gs_console_start().
   @return_gs_error_t
*/
gs_error_t gs_console_stop(void);
    
#ifdef __cplusplus
}
#endif
#endif
