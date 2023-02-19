/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/bytebuffer.h>
#include <gs/util/check.h>
#include <stdio.h>

#define GS_BYTEBUFFER_F_FAILED   0x01
#define GS_BYTEBUFFER_F_OVERRUN  0x02

gs_error_t gs_bytebuffer_init(gs_bytebuffer_t * bb, void * buffer, size_t buffer_size)
{
    GS_CHECK_HANDLE(bb != NULL);
    memset(bb, 0, sizeof(*bb));
    if (buffer) {
        if (buffer_size < 2) {
            // must always have room for NUL termination.
            return GS_ERROR_ARG;
        }
        bb->buffer = buffer;
        bb->size = buffer_size;
    } else {
        // dry run - don't insert anything in buffer, but increment used
    }

    return GS_OK;
}

void gs_bytebuffer_vprintf(gs_bytebuffer_t * bb, const char * format, va_list ap)
{
    int res;
    if (bb->buffer == NULL) {
        // dry run
        char buf[3];
        res = vsnprintf(buf, 0, format, ap);
        if (res >= 0) {
            bb->used += res;
        }
    } else {
        const size_t free_bytes = gs_bytebuffer_get_free(bb);
        res = vsnprintf((char*)&bb->buffer[bb->used], free_bytes, format, ap);
        if (res > 0) {
            if ((size_t)res >= free_bytes) {
                // over run
                bb->flags |= GS_BYTEBUFFER_F_OVERRUN;
                bb->used = bb->size;
                bb->buffer[bb->size - 1] = 0;
            } else {
                bb->used += res;
            }
        }
    }
    if (res < 0) {
        bb->flags |= GS_BYTEBUFFER_F_FAILED;
    }
}

void gs_bytebuffer_printf(gs_bytebuffer_t * bb, const char * format, ...)
{
    va_list ap;
    va_start(ap, format);
    gs_bytebuffer_vprintf(bb, format, ap);
    va_end(ap);
}

void gs_bytebuffer_append(gs_bytebuffer_t * bb, const void * data, size_t length)
{
    if (bb->buffer == NULL) {
        // dry run
        bb->used += length;
    } else {
        const size_t free_bytes = gs_bytebuffer_get_free(bb);
        if (free_bytes >= length) {
            memcpy(&bb->buffer[bb->used], data, length);
            bb->used += length;
        } else {
            memcpy(&bb->buffer[bb->used], data, free_bytes);
            bb->flags |= GS_BYTEBUFFER_F_OVERRUN;
            bb->used += free_bytes;
        }
    }
}

void gs_bytebuffer_append_string(gs_bytebuffer_t * bb, const char * string)
{
    if (gs_string_empty(string) == false) {
        gs_bytebuffer_append(bb, string, strlen(string));
    }
}

void gs_bytebuffer_append_string_max(gs_bytebuffer_t * bb, const char * string, size_t max_length)
{
    if (gs_string_empty(string) == false) {
        gs_bytebuffer_append(bb, string, strnlen(string, max_length));
    }
}

char * gs_bytebuffer_get_as_string(gs_bytebuffer_t * bb, gs_error_t * error)
{
    if (bb && bb->buffer) {
        // handle NUL termination
        if (bb->used < bb->size) {
            bb->buffer[bb->used] = 0;
        } else {
            // overrun - truncation buffer
            bb->flags |= GS_BYTEBUFFER_F_OVERRUN;
            bb->buffer[bb->used - 1] = 0;
        }
        if (error) {
            *error = gs_bytebuffer_get_state(bb);
        }
        return (char*) bb->buffer;
    }
    return NULL;
}

gs_error_t gs_bytebuffer_get_state(gs_bytebuffer_t * bb)
{
    if (bb) {
        if (bb->flags & GS_BYTEBUFFER_F_FAILED) {
            return GS_ERROR_DATA;
        }
        if (bb->flags & GS_BYTEBUFFER_F_OVERRUN) {
            return GS_ERROR_OVERFLOW;
        }
        return GS_OK;
    }
    return GS_ERROR_HANDLE;
}
