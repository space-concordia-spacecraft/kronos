/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/stdio.h>
#include <unistd.h>
#include <poll.h>

gs_error_t gs_stdio_putchar(int ch)
{
    const int res = putchar(ch);
    if (res < 0) {
        return GS_ERROR_IO;
    }
    return GS_OK;
}

gs_error_t gs_stdio_put_direct(const char * buf, size_t len)
{
    return gs_stdio_put(buf, len, false);
}

gs_error_t gs_stdio_getchar_timed(int timeout_ms, int * ch)
{
    struct pollfd fds = {STDIN_FILENO, POLLIN, 0};
    const int res = poll(&fds, 1, timeout_ms);

    if (res == 0) {
        return GS_ERROR_TIMEOUT;
    }

    if ((res > 0) && (fds.revents & POLLIN)) {
        int tmp = getchar();
        if (tmp >= 0) {
            if (ch) {
                *ch = tmp;
            }
            return GS_OK;
        }
    }

    return GS_ERROR_IO;
}
