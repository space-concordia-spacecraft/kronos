#ifndef GS_UTIL_LINUX_EXITCODE_H
#define GS_UTIL_LINUX_EXITCODE_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   "standard" Linux exit codes.
*/

#include <stdlib.h>
#include <sysexits.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Program completed ok (from stdlib.h)
*/
#define GS_EXITCODE_OK       EXIT_SUCCESS

/**
   Program terminated due to an error (from stdlib.h).
*/
#define GS_EXITCODE_ERROR    EXIT_FAILURE

/**
   Program terminated due to invalid usage, eg argument (from sysexits.h).
*/
#define GS_EXITCODE_USAGE    EX_USAGE

/**
   Program terminated due to a signal (from [TLDP](http://www.tldp.org/LDP/abs/html/exitcodes.html)).
*/
#define GS_EXITCODE_SIGNAL(sig)  (128 + sig)
    
#ifdef __cplusplus
}
#endif
#endif
