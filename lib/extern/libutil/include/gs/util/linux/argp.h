#ifndef GS_UTIL_LINUX_ARGP_H
#define GS_UTIL_LINUX_ARGP_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Extensions to GNU argp parser (convenience functions).
*/

#include <argp.h>
#include <gs/util/linux/exitcode.h>
#include <gs/util/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Wrapper for argp_parse.

   This function will call exit/terminate the process, if parsing fails.

   \a argv may be re-organized.

   @param[in] argp argp struct.
   @param[in] argc argument count, i.e. standard argc.
   @param[in] argv argument array, i.e. standard argv.
   @param[in] flags future use.
   @param[out] arg_index first unparsed option (-> argv modified).
   @param[in] revision program revision, e.g. 3.0.1-12-g0cf1b59+.
*/
void gs_argp_parse(const struct argp * argp,
                   int argc, char ** argv,
                   unsigned int flags, int * arg_index,
                   const char * revision);

#ifdef __cplusplus
}
#endif
#endif
