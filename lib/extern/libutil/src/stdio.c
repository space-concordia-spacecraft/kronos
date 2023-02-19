/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/stdio.h>
#include <stdarg.h>

gs_error_t gs_stdio_get(char * buf, size_t len)
{
    while (len > 0) {
        int ch;
        gs_error_t error = gs_stdio_getchar(&ch);
        if (error) {
            return error;
        }
        *buf++ = ch;
        --len;
    }

    return GS_OK;
}

gs_error_t gs_stdio_put(const char * buf, size_t len, bool text)
{
    while (len > 0) {
        if ((*buf == '\n') && text) {
            gs_stdio_putchar('\r');
        }
        gs_stdio_putchar(*buf++);
        --len;
    }

    return GS_OK;
}

void gs_color_printf(gs_color_printf_t color_arg, const char * format, ...)
{
    va_list args;
    va_start(args, format);

    if ((color_arg & GS_COLOR_ATTRS) == GS_COLOR_BOLD) {
        printf("\033[1;");
    } else {
        printf("\033[0;");
    }

    switch(color_arg & GS_COLOR_COLORS) {
	case GS_COLOR_NONE:
            printf("0m");
            break;
	case GS_COLOR_BLACK:
            printf("30m");
            break;
	case GS_COLOR_RED:
            printf("31m");
            break;
	case GS_COLOR_GREEN:
            printf("32m");
            break;
	case GS_COLOR_YELLOW:
            printf("33m");
            break;
	case GS_COLOR_BLUE:
            printf("34m");
            break;
	case GS_COLOR_MAGENTA:
            printf("35m");
            break;
	case GS_COLOR_CYAN:
            printf("36m");
            break;
	case GS_COLOR_WHITE:
            printf("37m");
            break;
	default:
            break;
    }

    vprintf(format, args);
    printf("\033[0m");

    va_end(args);
}
