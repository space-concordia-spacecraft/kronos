#ifndef GS_UTIL_LINUX_FUNCTION_H
#define GS_UTIL_LINUX_FUNCTION_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Function interface - invokes a function by name.
*/

#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Function prototype.
   @param[in] arg argument provided to gs_function_invoke().
   @return_gs_error_t
*/
typedef gs_error_t (*gs_function_t)(void * arg);

/**
   Register \a function by name.

   @param[in] short_name short name for function, used by gs_function_invoke() to find function to invoke.
   @param[in] long_name long name (unique) for function, used by gs_function_invoke() to find function to invoke.
   @param[in] function function to be invoked by gs_function_invoke()
   @return #GS_ERROR_FULL if registry is full.
   @return_gs_error_t
*/
gs_error_t gs_function_register(const char * short_name, const char * long_name, gs_function_t function);

/**
   Invoke \a function by name.

   The return value is from the registered function, except for #GS_ERROR_NOT_IMPLEMENTED.

   @param[in] name registered function name.
   @param[in] arg argument for function.
   @return #GS_ERROR_NOT_IMPLEMENTED if the \a name isn't found.
   @return_gs_error_t
*/
gs_error_t gs_function_invoke(const char * name, void * arg);

#ifdef __cplusplus
}
#endif
#endif
