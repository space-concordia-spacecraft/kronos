#ifndef GS_UTIL_LOCK_H
#define GS_UTIL_LOCK_H
/* Copyright (c) 2013-2019 GomSpace A/S. All rights reserved. */
/**
   @file

   Basic/core locking.

   @note Should only be used for rare/fast locking, e.g. protecting register/de-regsiter functions.
*/

#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Initialize lock.

   @return_gs_error_t
*/
gs_error_t gs_lock_init(void);

/**
   Take lock.

   Initialize lock it not already done, by calling gs_lock_init().

   @return_gs_error_t
*/
gs_error_t gs_lock_lock(void);

/**
   Release lock.

   @return_gs_error_t
*/
gs_error_t gs_lock_unlock(void);

#ifdef __cplusplus
}
#endif
#endif
