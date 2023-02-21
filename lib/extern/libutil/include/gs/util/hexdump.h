#ifndef GS_UTIL_HEXDUMP_H
#define GS_UTIL_HEXDUMP_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Dump memory as hex numbers and ascii characters.
*/

#include <stdio.h>
#include <gs/util/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Dump memory to an output stream.
   @param[in] src memory address.
   @param[in] len number of bytes to dump.
   @param[in] disp_addr display address, used instead of \a src.
   @param[in] out output stream.
*/
void gs_hexdump_to_stream(const void * src, size_t len, const void * disp_addr, FILE* out);

/**
   Dump memory on stdout.

   @param[in] src memory address.
   @param[in] len number of bytes to dump.
*/
static inline void gs_hexdump(const void *src, size_t len)
{
    gs_hexdump_to_stream(src, len, src, stdout);
}

/**
   Dump memory on stdout.
   @param[in] src memory address.
   @param[in] len number of bytes to dump.
   @param[in] disp_addr display address, used instead of \a src.
*/
static inline void gs_hexdump_addr(const void * src, size_t len, const void * disp_addr)
{
    gs_hexdump_to_stream(src, len, disp_addr, stdout);
}

#ifdef __cplusplus
}
#endif
#endif


