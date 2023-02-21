#ifndef GS_UTIL_BYTEBUFFER_h
#define GS_UTIL_BYTEBUFFER_h
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Byte buffer provides formatting/serialzing of text/binary data. The buffer keeps track of used space, and prevents overrun.

   The current buffer state can be checked using gs_bytebuffer_state().

   @dontinclude bytebuffer/bytebuffer_test.c
   @skip TEST_gs_bytebuffer_use_case
   @until }
*/

#include <gs/util/error.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Buffer handle.
   Never access handle members directly.
*/
typedef struct {
    /**
       Internal: Pointer to user supplied buffer.
       @see gs_bytebuffer_init()
    */
    uint8_t * buffer;
    /**
       Internal: Size of user supplied buffer.
       @see gs_bytebuffer_init()
    */
    size_t size;
    /**
       Internal: Number of bytes used.
    */
    size_t used;
    /**
       Internal: FUTURE: Committed used
    */
    size_t committed_used;
    /**
       Internal: flags to keep track of buffer state.
    */
    uint8_t flags;
} gs_bytebuffer_t;

/**
   Initialize buffer.

   @param[in] bb handle.
   @param[in] buffer user supplied buffer of \a buffer_size size (bytes). If NULL, the buffer will keep track of required bytes.
   @param[in] buffer_size size of \a buffer.
   @return_gs_error_t
*/
gs_error_t gs_bytebuffer_init(gs_bytebuffer_t * bb, void * buffer, size_t buffer_size);

/**
   Insert data using vprintf.

   @param[in] bb handle.
   @param[in] format printf syntax for formatting data
   @param[in] ap variable argument list.
*/
void gs_bytebuffer_vprintf(gs_bytebuffer_t * bb, const char * format, va_list ap);

/**
   Insert data using printf.

   @param[in] bb handle.
   @param[in] format printf syntax for formatting data
*/
void gs_bytebuffer_printf(gs_bytebuffer_t * bb, const char * format, ...) __attribute__ ((format (__printf__, 2, 3)));

/**
   Append data to buffer.

   @param[in] bb handle.
   @param[in] data data to append to buffer.
   @param[in] length length of data (bytes).
*/
void gs_bytebuffer_append(gs_bytebuffer_t * bb, const void * data, size_t length);

/**
   Append string to buffer.

   @param[in] bb handle.
   @param[in] string string to append to buffer.
*/
void gs_bytebuffer_append_string(gs_bytebuffer_t * bb, const char * string);

/**
   Append string to buffer.

   @param[in] bb handle.
   @param[in] string string to append to buffer.
   @param[in] max_length max characters to append from \a string.
*/
void gs_bytebuffer_append_string_max(gs_bytebuffer_t * bb, const char * string, size_t max_length);

/**
   Return buffer as string - enforcing NUL termination.

   This will always add a NUL termination (zero), which may lead to overflow/truncation of the string.
   The NUL termination is NOT added to \a used count.

   @param[in] bb handle.
   @param[out] error optional, state of buffer - see gs_bytebuffer_error().
   @return C-string (NUL terminated)
*/
char * gs_bytebuffer_get_as_string(gs_bytebuffer_t * bb, gs_error_t * error);

/**
   Return buffer state.

   @param[in] bb handle.
   @return GS_ERROR_OVERFLOW if data has been truncated.
   @return GS_ERROR_DATA in case of error during formatting.
   @return_gs_error_t
*/
gs_error_t gs_bytebuffer_get_state(gs_bytebuffer_t * bb);

/**
   Return buffer (user supplied).

   @param[in] bb handle.
*/
static inline void * gs_bytebuffer_get_buffer(gs_bytebuffer_t * bb)
{
    return bb->buffer;
}

/**
   Return buffer size (user supplied).

   @param[in] bb handle.
   @return buffer size
*/
static inline size_t gs_bytebuffer_get_size(gs_bytebuffer_t * bb)
{
    return bb->size;
}

/**
   Return number of free bytes.

   @param[in] bb handle.
   @return number of free bytes.
*/
static inline size_t gs_bytebuffer_get_free(gs_bytebuffer_t * bb)
{
    return (bb->size) ? (bb->size - bb->used) : 0;
}

/**
   Return number of used bytes.

   @param[in] bb handle.
   @return used bytes.
*/
static inline size_t gs_bytebuffer_get_used(gs_bytebuffer_t * bb)
{
    return bb->used;
}

#ifdef __cplusplus
}
#endif
#endif
