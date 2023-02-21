#ifndef GS_UTIL_LINUX_SIGNAL_H
#define GS_UTIL_LINUX_SIGNAL_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Signal helpers - catch and ignore.
*/

#include <signal.h>
#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Standard Linux signal handler.
*/
typedef void (*gs_signal_handler)(int signal, siginfo_t *si, void *context);

/**
   Register/catch signal and invoke handler.
   @param[in] signal signal to catch.
   @param[in] handler signal handler. If \a handler is NULL, a default handler will be invoked, which calls exit(#GS_EXITCODE_SIGNAL + signal).
   @return_gs_error_t
*/
gs_error_t gs_signal_catch(int signal, gs_signal_handler handler);

/**
   Ignore signal
   @param[in] signal signal to ignore.
   @return_gs_error_t
*/
gs_error_t gs_signal_ignore(int signal);

#ifdef __cplusplus
}
#endif
#endif
