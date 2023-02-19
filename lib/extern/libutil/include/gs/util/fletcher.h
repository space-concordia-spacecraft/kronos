#ifndef GS_UTIL_FLETCHER_H
#define GS_UTIL_FLETCHER_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Fletcher16 checksum,
*/

#include <gs/util/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Fletcher16 checksum (read using copy function).

   Data is read from \a data, using the specified \a memcpyfcn function.

   @param[in] data data.
   @param[in] size number of \a data bytes.
   @param[in] memcpyfcn memory copy function. If NULL is specified, standard memcpy will be used.
   @returns fletcher16 checksum
*/
uint16_t gs_fletcher16_memcpy(const void * data, size_t size, void * (*memcpyfcn)(void *, const void *, size_t));

/**
   Fletcher16 checksum (read from program memory).

   AVR8: reads from program memory.
   Other architectures: identical to gs_fletcher16().

   @param[in] data_in data.
   @param[in] size number of \a data bytes.
   @returns fletcher16 checksum
*/
uint16_t gs_fletcher16_P(const void * data_in, size_t size);

/**
   Fletcher16 checksum.

   @param[in] data data.
   @param[in] size number of \a data bytes.
   @returns fletcher16 checksum
*/
uint16_t gs_fletcher16(const void * data, size_t size);

/**
   Fletcher16 working set.
   @see gs_fletcher16_init(), gs_fletcher16_update(), gs_fletcher16_finalize()
*/
typedef struct {
    /**
       Sum1 - internal.
    */
    uint16_t sum1;
    /**
       Sum2 - internal.
    */
    uint16_t sum2;
} gs_fletcher16_t;

/**
   Initialize fletcher16 working set.
   @param[in] f16 working set.
*/
void gs_fletcher16_init(gs_fletcher16_t * f16);

/**
   Update fletcher16 checksum.
   @param[in] f16 working set.
   @param[in] data data.
   @param[in] size number of \a data bytes.
*/
void gs_fletcher16_update(gs_fletcher16_t * f16, const void * data, size_t size);

/**
   Finalize fletcher16 checksum and return it.

   @param[in] f16 working set.
   @returns fletcher16 checksum
*/
uint16_t gs_fletcher16_finalize(gs_fletcher16_t * f16);
    
#ifdef __cplusplus
}
#endif
#endif
