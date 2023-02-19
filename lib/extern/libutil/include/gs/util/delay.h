#ifndef GS_UTIL_DELAY_H
#define GS_UTIL_DELAY_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Delay execution.

   @note Most implementations uses busy waiting.
*/

#include <gs/util/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Delay for number of microseconds.
   @note Linux doesn't busy wait.
   @param us Number of microseconds to wait
*/
void gs_delay_us(uint32_t us);

/**
   Return current counter used for us delays
   @return timestamp in us
*/
uint16_t gs_delay_ts_get(void);

/**
   Wait until delay has passed since timestamp

   @param[in] ts Timestamp in us
   @param[in] delay The requested delay since ts
*/
void gs_delay_from_ts(uint16_t ts, uint16_t delay);

#ifdef __cplusplus
}
#endif
#endif
