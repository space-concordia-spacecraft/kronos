/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/gosh/console.h>
#include <conf_util.h>  // console defines set through Waf options

/* Max history length (elements) */
#if !defined(GS_CONSOLE_HISTORY_LEN)
#if (__linux__)
#define GS_CONSOLE_HISTORY_LEN       50
#else
#define GS_CONSOLE_HISTORY_LEN       10
#endif
#endif

/* Max input length */
#if !defined(GS_CONSOLE_INPUT_LEN)
#define GS_CONSOLE_INPUT_LEN         100
#endif

/**
   Change console mode.
   @param[in] mode console mode, 'rgosh', 'normal'
   @return_gs_error_t
*/
int gs_console_change_mode(const char * mode);
