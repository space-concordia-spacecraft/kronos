#ifndef DEPRECATED_UTIL_COLOR_PRINTF_H
#define DEPRECATED_UTIL_COLOR_PRINTF_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/stdio.h>

typedef enum color_printf_e {
    /* Colors */
    COLOR_COLORS   = GS_COLOR_COLORS,
    COLOR_NONE     = GS_COLOR_NONE,
    COLOR_BLACK    = GS_COLOR_BLACK,
    COLOR_RED      = GS_COLOR_RED,
    COLOR_GREEN    = GS_COLOR_GREEN,
    COLOR_YELLOW   = GS_COLOR_YELLOW,
    COLOR_BLUE     = GS_COLOR_BLUE,
    COLOR_MAGENTA  = GS_COLOR_MAGENTA,
    COLOR_CYAN     = GS_COLOR_CYAN,
    COLOR_WHITE    = GS_COLOR_WHITE,
    /* Attributes */
    COLOR_ATTRS    = GS_COLOR_ATTRS,
    COLOR_BOLD     = GS_COLOR_BOLD,
} color_printf_t;

#define color_printf gs_color_printf

#endif
