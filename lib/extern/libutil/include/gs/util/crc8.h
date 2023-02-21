#ifndef GS_UTIL_CRC8_H
#define GS_UTIL_CRC8_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   CRC8 checksumes.

   https://en.wikipedia.org/wiki/Cyclic_redundancy_check.
*/

#include <gs/util/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Return init/seed value for CRC-8.
   @return initial/seed value for CRC-8, using 0xff.
   @see gs_crc8_update(), gs_crc8_finalize()
*/
uint8_t gs_crc8_init(void);

/**
   Update CRC-8.
   @param[in] crc current CRC-8
   @param[in] block start of memory block.
   @param[in] length length of \a block.
   @return updated CRC-8.
   @see gs_crc8_init(), gs_crc8_finalize()
*/
uint8_t gs_crc8_update(uint8_t crc, const void * block, size_t length);

/**
   Return finalized CRC-8.
   @param[in] crc Checksum is finalized by xor'ing 0xffffffff.
   @return finalized CRC-8.
   @see gs_crc8_init(), gs_crc8_update()
*/
uint8_t gs_crc8_finalize(uint8_t crc);

/**
   Return finalized CRC-8 on amemory block.

   @param[in] block block to calculate CRC-8 on.
   @param[in] length length/size of \a block.
   @return finalized CRC-8.
*/
uint8_t gs_crc8(const void *block, size_t length);

#ifdef __cplusplus
}
#endif
#endif
