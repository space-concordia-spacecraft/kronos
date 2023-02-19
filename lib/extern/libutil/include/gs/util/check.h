#ifndef GS_UTIL_CHECK_H
#define GS_UTIL_CHECK_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Argument checking.

   Logs can be enabled through a define.
*/

#include <gs/util/error.h>
#include <gs/util/log.h>
#include <gs/util/string.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (GS_CHECK_LOG)
#define GS_CHECK_HANDLE(check)      if (!(check)) { log_error("Invalid handle - assert: " GS_DEF2STRING(check)); return GS_ERROR_HANDLE;}
#define GS_CHECK_ARG(check)         if (!(check)) { log_error("Invalid argument - assert: " GS_DEF2STRING(check)); return GS_ERROR_ARG;}
#define GS_CHECK_SUPPORTED(check)   if (!(check)) { log_error("Not supported - assert: " GS_DEF2STRING(check)); return GS_ERROR_NOT_SUPPORTED;}
#define GS_CHECK_RANGE(check)       if (!(check)) { log_error("Invalid range - assert: " GS_DEF2STRING(check)); return GS_ERROR_RANGE;}
#else
/**
   Perform evalution of 'check' and return GS_ERROR_HANDLE if not 'true'.
*/
#define GS_CHECK_HANDLE(check)      if (!(check)) { return GS_ERROR_HANDLE;}
/**
   Perform evalution of 'check' and return GS_ERROR_ARG if not 'true'.
*/
#define GS_CHECK_ARG(check)         if (!(check)) { return GS_ERROR_ARG;}
/**
   Perform evalution of 'check' and return GS_ERROR_NOT_SUPPORTED if not 'true'.
*/
#define GS_CHECK_SUPPORTED(check)   if (!(check)) { return GS_ERROR_NOT_SUPPORTED;}
/**
   Perform evalution of 'check' and return GS_ERROR_RANGE if not 'true'.
*/
#define GS_CHECK_RANGE(check)       if (!(check)) { return GS_ERROR_RANGE;}
#endif

/**
   Assert on 'value'.

   @deprecated use GS_STATIC_ASSERT()
*/
#define GS_CHECK_STATIC_ASSERT(condition, name)  GS_STATIC_ASSERT(condition, name)

#ifdef __cplusplus
}
#endif
#endif
