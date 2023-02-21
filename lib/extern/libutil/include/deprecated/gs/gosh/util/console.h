#ifndef GS_GOSH_UTIL_CONSOLE_H
#define GS_GOSH_UTIL_CONSOLE_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   Legacy header file - use gs/util/gosh/console.h
*/

#include <gs/util/gosh/console.h>
#include <gs/gosh/command/command.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline int console_init(void)
{
    return gs_console_init();
}

static inline int console_exit(void)
{
    return gs_console_exit();
}

static inline void console_set_hostname(const char *host)
{
    gs_console_set_prompt(host);
}

static inline void console_clear(void)
{
    gs_console_clear();
}

static inline void console_update(void)
{
    gs_console_update();
}

#ifdef __cplusplus
}
#endif
#endif
