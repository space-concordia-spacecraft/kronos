#ifndef GS_UTIL_CRC32_H
#define GS_UTIL_CRC32_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   CRC32 checksumes.

   https://en.wikipedia.org/wiki/Cyclic_redundancy_check.
*/

#include <gs/util/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Return init/seed value for CRC-32.
   @return initial/seed value for CRC-32, using 0xffffffff.
   @see gs_crc32_update(), gs_crc32_finalize()
*/
uint32_t gs_crc32_init(void);

/**
   Update CRC-32.
   @param[in] crc current CRC-32
   @param[in] block start of memory block.
   @param[in] length length of \a block.
   @return updated CRC-32.
   @see gs_crc32_init(), gs_crc32_finalize()
*/
uint32_t gs_crc32_update(uint32_t crc, const void * block, size_t length);

/**
   Return finalized CRC-32.
   @param[in] crc Checksum is finalized by xor'ing 0xffffffff.
   @return finalized CRC-32.
   @see gs_crc32_init(), gs_crc32_update()
*/
uint32_t gs_crc32_finalize(uint32_t crc);

/**
   Return finalized CRC-32 on amemory block.

   @param[in] block block to calculate CRC-32 on.
   @param[in] length length/size of \a block.
   @return finalized CRC-32.
*/
uint32_t gs_crc32(const void *block, size_t length);

#ifdef __cplusplus
}
#endif
#endif
