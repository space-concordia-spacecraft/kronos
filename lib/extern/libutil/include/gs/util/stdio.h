#ifndef GS_UTIL_STDIO_H
#define GS_UTIL_STDIO_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   GomSpace extensions to standard \a stdio.h.
*/

#include <stdio.h>
#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Put character on stdout.
*/
gs_error_t gs_stdio_putchar(int ch);

/**
   Read character from stdin with timeout.
   @param[in] timeout_ms timeout, < 0: block forever, 0: poll, > 0: wait number of milli seconds.
   @param[out] ch character read. If NULL, one character from stdin is still consumed - but nothing returned.
   @return GS_ERROR_TIMEOUT on timeout
   @return_gs_error_t
*/
gs_error_t gs_stdio_getchar_timed(int timeout_ms, int *ch);

/**
   Read character from stdin.
   Blocks until a character is available.
   @param[out] ch character read. If NULL, one character from stdin is still consumed - but nothing returned.
   @return_gs_error_t
*/
static inline gs_error_t gs_stdio_getchar(int * ch)
{
    return gs_stdio_getchar_timed(-1, ch);
}

/**
   Read characters from stdin.
   Blocks until all characters are read.
   @param[in,out] buf user supplied buffer for receiving characters.
   @param[in] n number of characters to read.
   @return_gs_error_t
*/
gs_error_t gs_stdio_get(char * buf, size_t n);

/**
   Write to stdout.

   @param[in] buf characters to write.
   @param[in] len number of characters to write.
   @param[in] text if \a true, new lines (\\n) are converted to \\r\\n. Deprecated (to be removed), always use \a false.
   @return_gs_error_t
*/
gs_error_t gs_stdio_put(const char * buf, size_t len, bool text);

/**
   Write directly to stdout.
   Circumvents any interrupt based uart implementation.

   @param[in] buf characters to write.
   @param[in] len number of characters to write.
   @return_gs_error_t
*/
gs_error_t gs_stdio_put_direct(const char * buf, size_t len);
    
/**
   Pattern for printing a byte as binary.
   @see GS_STDIO_BYTETOBINARY()
*/
#define GS_STDIO_BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"

/**
   Macro for splitting a byte info 'bits'.
*/
#define GS_STDIO_BYTETOBINARY(byte)  \
    (byte & 0x80 ? 1 : 0),           \
    (byte & 0x40 ? 1 : 0),           \
    (byte & 0x20 ? 1 : 0),           \
    (byte & 0x10 ? 1 : 0),           \
    (byte & 0x08 ? 1 : 0),           \
    (byte & 0x04 ? 1 : 0),           \
    (byte & 0x02 ? 1 : 0),           \
    (byte & 0x01 ? 1 : 0)

/**
   Color definitions for gs_color_printf()
   @see gs_color_printf()
*/
typedef enum {
    /**
       Colors.
    */
    GS_COLOR_COLORS = 0x00ff,
    GS_COLOR_NONE = 0,
    GS_COLOR_BLACK = 1,
    GS_COLOR_RED = 2,
    GS_COLOR_GREEN = 3,
    GS_COLOR_YELLOW = 4,
    GS_COLOR_BLUE = 5,
    GS_COLOR_MAGENTA = 6,
    GS_COLOR_CYAN = 7,
    GS_COLOR_WHITE = 8,
    /**
       Attributes
    */
    GS_COLOR_ATTRS = 0xff00,
    GS_COLOR_BOLD = 0x100,
} gs_color_printf_t;

/**
   Printf with colors on stdout.

   Using the standard terminal escape sequences for setting the color.
   @param[in] color color settings.
   @param[in] format standard printf format string.
*/
void gs_color_printf(gs_color_printf_t color, const char * format, ...) __attribute__ ((format (__printf__, 2, 3)));

#ifdef __cplusplus
}
#endif
#endif
