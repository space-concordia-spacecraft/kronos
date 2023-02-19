#ifndef GS_UTIL_SYSFS_HELPER_H
#define GS_UTIL_SYSFS_HELPER_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Sysfs interface.
*/

#include <gs/util/types.h>
#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Sysfs write (GPIO).
 *
 * @param path File path.
 * @param value Value to write.
 * @return gs_error_t
 */
gs_error_t gs_sysfs_write_file(const char *path, const char *value);

/**
 * @brief Sysfs read (GPIO).
 *
 * @param path File path.
 * @param value Buffer to read file value/contents into.
 * @param len Length of value buffer.
 *
 * @warning Value buffer is not zero/nul terminated, if the read size equals the
 *          buffer length.
 *
 * @return gs_error_t
 */
gs_error_t gs_sysfs_read_file(const char *path, char *value, size_t len);

#ifdef __cplusplus
}
#endif
#endif
