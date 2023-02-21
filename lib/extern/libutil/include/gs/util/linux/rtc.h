#ifndef GS_UTIL_LINUX_RTC_H
#define GS_UTIL_LINUX_RTC_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Real Time Clock interface (linux).
*/

#include <gs/util/rtc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Register Real Time Clock interface.
   @note Setting the RTC will normally require special permission.
   @param[in] get if true, get will be registered.
   @param[in] set if true, set will be registered.
   @return_gs_error_t
*/
gs_error_t gs_rtc_register_linux(bool get, bool set);

#ifdef __cplusplus
}
#endif
#endif
