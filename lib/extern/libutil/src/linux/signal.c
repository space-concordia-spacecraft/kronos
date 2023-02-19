/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/linux/signal.h>
#include <gs/util/linux/exitcode.h>

static void gs_signal_default_handler(int signo, siginfo_t *si, void *context)
{
    exit(GS_EXITCODE_SIGNAL(signo)); // ensure atexit are invoked
}

gs_error_t gs_signal_catch(int signal, gs_signal_handler handler)
{
    if (handler == NULL) {
        handler = gs_signal_default_handler;
    }
    struct sigaction sa = { .sa_flags = SA_SIGINFO,
                            .sa_sigaction = handler};
    if (sigemptyset(&sa.sa_mask)) {
        return GS_ERROR_UNKNOWN;
    }
    if (sigaction(signal, &sa, NULL)) {
        return GS_ERROR_UNKNOWN;
    }
    return GS_OK;
}

gs_error_t gs_signal_ignore(int signal)
{
    struct sigaction sa = { .sa_flags = 0,
                            .sa_handler = SIG_IGN}; // handle signal by ignoring
    if (sigemptyset(&sa.sa_mask)) {
        return GS_ERROR_UNKNOWN;
    }
    if (sigaction(signal, &sa, NULL)) {
        return GS_ERROR_UNKNOWN;
    }
    return GS_OK;
}
