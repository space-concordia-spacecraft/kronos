#ifndef GS_UTIL_TIMESTAMP_H
#define GS_UTIL_TIMESTAMP_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Timestamp utilities, for add, subtract, compare, copy, etc.
*/

#include <gs/util/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Number of nano seconds per second.
*/
#define GS_TIMESTAMP_NSEC_PER_SEC   1000000000

/**
   Portable time structure.

   Stanadard timespec_t is non-portable, so this structure must be used instead
*/
typedef struct {
    /** Seconds. */
    uint32_t tv_sec;
    /** Nano seconds. */
    uint32_t tv_nsec;
} gs_timestamp_t;

/**
   @deprecated Use gs_timestamp_t
*/
typedef gs_timestamp_t timestamp_t;

/**
   Add 2 timestamp's (t1 = t1 + t2).
   @param[in,out] t1 timestamp
   @param[in] t2 timestamp.
   @return 0 on success, otherwise -1
*/
int timestamp_add(gs_timestamp_t * t1, const gs_timestamp_t * t2);

/**
   Subtract 2 timestamp's (t1 = t1 - t2)
   @param[in,out] t1 timestamp
   @param[in] t2 timestamp.
   @return 0 on success, otherwise -1
*/
int timestamp_diff(gs_timestamp_t * t1, const gs_timestamp_t * t2);

/**
   Check if t2 is greate than t1.
   @param[in] t1 time to compare
   @param[in] t2 time to compare
   @return 1 if t2 > t1, else 0. -1 on bad arguments.
*/
int timestamp_ge(const gs_timestamp_t * t1, const gs_timestamp_t * t2);

/**
   Copy timestamp.
   @param[in] from from timestamp
   @param[out] to to timestamp
   @return 0 on success, otherwise -1
*/
int timestamp_copy(const gs_timestamp_t * from, gs_timestamp_t * to);

#ifdef __cplusplus
}
#endif
#endif
