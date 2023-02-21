#ifndef GS_UTIL_ERROR_H
#define GS_UTIL_ERROR_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Common error code definitions.
*/

#include <gs/util/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Common/generic error codes.
   Based on POSIX \a errno values, but negative instead of positive.
*/
typedef enum gs_error_t {
  /**
     Success - ok (POSIX).
  */
  GS_OK = 0,
  /**
     Operation not permitted (POSIX.1: EPERM).
  */
  GS_ERROR_PERM = -1,
  /**
     Interrupted system call (or Interrupted function call) (POSIX: EINTR).
  */
  GS_ERROR_INTR = -4,
  /**
     Input/output error (POSIX.1: EIO)
  */
  GS_ERROR_IO = -5,
  /**
     Resource temporarily unavailable (may be the same value as EWOULDBLOCK) (POSIX.1: EAGAIN).
  */
  GS_ERROR_AGAIN = -11,
  /**
     Cannot allocate memory (or Not enough space) (POSIX.1: ENOMEM).
  */
  GS_ERROR_ALLOC = -12,
  /**
     Permission denied (POSIX.1: EACCES).
  */
  GS_ERROR_ACCESS = -13,
  /**
     Device or resource busy (POSIX.1: EBUSY).
  */
  GS_ERROR_BUSY = -16,
  /**
     File exists (POSIX.1-2001: EEXIST).
  */
  GS_ERROR_EXIST = -17,
  /**
     Invalid argument (POSIX.1: EINVAL).
  */
  GS_ERROR_ARG = -22,
  /**
     Function not implemented (POSIX.1: ENOSYS)
  */
  GS_ERROR_NOT_IMPLEMENTED = -38,
  /**
     Value too large to be stored in data type (POSIX.1: EOVERFLOW).
     Example: trying to put 50 characters into a 10 character array.
     @see GS_ERROR_RANGE.
  */
  GS_ERROR_OVERFLOW = -75,
  /**
     Operation not supported (POSIX.1: ENOTSUP)
  */
  GS_ERROR_NOT_SUPPORTED = -95,
  /**
     Address already in use (POSIX.1: EADDRINUSE).
  */
  GS_ERROR_IN_USE = -98,
  /**
     Connection reset (POSIX.1-2001: ECONNRESET).
  */
  GS_ERROR_CONNECTION_RESET = -104,
  /**
     No buffer space available (POSIX.1 (XSI STREAMS option): ENOBUFS).
  */
  GS_ERROR_NO_BUFFERS = -105,
  /**
     Timeout (POSIX.1-2001: ETIMEDOUT).
  */
  GS_ERROR_TIMEOUT = -110,
  /**
     Connection already in progress (POSIX.1-2001: EALREADY).
  */
  GS_ERROR_ALREADY_IN_PROGRESS = -114,

  /**
     Handle error (GOMspace).
  */
  GS_ERROR_HANDLE = -2000,    // from errno.h: #define __ELASTERROR 2000       /* Users can add values starting here */
  /**
     Not found (GOMspace).
  */
  GS_ERROR_NOT_FOUND = -2001,
  /**
     Full (GOMspace).
  */
  GS_ERROR_FULL = -2002,
  /**
     Range error (GOMspace).
     Example: specifying 120 hours, where only 0-23 is valid.
     @see GS_ERROR_OVERFLOW
  */
  GS_ERROR_RANGE = -2003,
  /**
     Data error (GOMspace).
  */
  GS_ERROR_DATA = -2004,
  /**
     Unknown error (GOMspace).
     @note avoid use - use specific error to improve debugging/troubleshooting.
  */
  GS_ERROR_UNKNOWN = -2005,
  /**
     No data available (GOMspace).
  */
  GS_ERROR_NO_DATA = -2006,
  /**
     Stale data - not updated (GOMspace).
  */
  GS_ERROR_STALE = -2007,
  /**
     Type error (GOMspace).
  */
  GS_ERROR_TYPE = -2008,
  /**
     Ambiguous error (GOMspace).
  */
  GS_ERROR_AMBIGUOUS = -2009,
  /**
     State error (GOMspace).
  */
  GS_ERROR_STATE = -2010,

} gs_error_t;

/**
 * Convert an error code to a string.
 * Uses standard POSIX strerror() under the hood.
 * @param[in] error error to convert. If negative (e.g. \a gs_error_t), it is first converted to a positive value.
 * @return string usefull for logging purposes (should not be used for programatically processing).
 */
const char * gs_error_string(int error);

/**
   Convert standard POSIX \a errno to gs_error_t.
   @param[in] error POSIX error code (errno).
   @return convert error code, by simply converting to a negative number.
*/
gs_error_t gs_error(int error);

#if (GS_UTIL_DEPRECATED_ERROR_CODES)
/**
   Legacy error definitions.
   @deprecated Use standard gs_error_t codes - these defines are only kept, so very old code (not yet update to use #gs_error_t) can compile.
   @{
*/
#define E_NO_ERR           -1
#define E_NO_DEVICE        -2
#define E_MALLOC_FAIL      -3
#define E_THREAD_FAIL      -4
#define E_NO_QUEUE         -5
#define E_INVALID_BUF_SIZE -6
#define E_INVALID_PARAM    -7
#define E_NO_SS            -8
#define E_GARBLED_BUFFER   -9
#define E_FLASH_ERROR      -10
#define E_BOOT_SER         -13
#define E_BOOT_DEBUG       -14
#define E_BOOT_FLASH       -15
#define E_TIMEOUT          -16
#define E_NO_BUFFER        -17
#define E_OUT_OF_MEM       -18
#define E_FAIL             -19
/** @} */

/**
   Converts legacy error definitions to string.
   @deprecated Use standard gs_error_t codes - this function is only kept, so very old code (not yet update to use #gs_error_t) can compile.
   @param[in] code error code
   @return string describing the error.
*/
const char * error_string(int code) __attribute__((deprecated));

#endif // GS_UTIL_DEPRECATED_ERROR_CODES

#ifdef __cplusplus
}
#endif
#endif
