#ifndef GS_UTIL_VMEM_H
#define GS_UTIL_VMEM_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Virtual memory interface.

   The API provides support for accessing different hardware components using a common API, by providing a component specific driver.
*/

#include <gs/util/stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Virtual memory mapping.
*/
typedef struct gs_vmem gs_vmem_t;

/**
   VMEM driver write.

   @param[in] vmem vmem entry.
   @param[in] to   Address where to write data to.
   @param[in] from Address where to write data from.
   @param[in] size Number of bytes to write.
   @return_gs_error_t
*/
typedef gs_error_t (*gs_vmem_write_function_t)(const gs_vmem_t * vmem, void* to, const void * from, size_t size);

/**
   VMEM driver read.

   @param[in] vmem vmem entry.
   @param[in] to   Address where to read data to.
   @param[in] from Address where to read data from.
   @param[in] size Number of bytes to read.
   @return_gs_error_t
*/
typedef gs_error_t (*gs_vmem_read_function_t)(const gs_vmem_t * vmem, void* to, const void * from, size_t size);

/**
   VMEM driver lock.

   @param[in] vmem vmem entry.
   @param[in] on   Enable/Disable lock.
   @return_gs_error_t
*/
typedef gs_error_t (*gs_vmem_lock_function_t)(const gs_vmem_t * vmem, bool on);

/**
   VMEM driver information.

   Return relevant information for the VMEM driver.

   @param[in] vmem vmem entry.
   @param[in] buffer user allocated buffer for returning information.
   @param[in] buffer_size size (length) of \a buffer.
   @return_gs_error_t
*/
typedef gs_error_t (*gs_vmem_info_function_t)(const gs_vmem_t * vmem, char * buffer, size_t buffer_size);

/**
   VMEM driver check.

   Checks/validates configuratione VMEM driver.

   @param[in] vmem vmem entry.
   @param[in] log  if \a true, information will logged.
   @param[in] out  if set (not \a NULL), information will printed to stream.
   @return_gs_error_t
*/
typedef gs_error_t (*gs_vmem_check_function_t)(const gs_vmem_t * vmem, bool log, FILE * out);

/**
   VMEM driver interface.
*/
typedef struct {
    /**
       Write function.
    */
    const gs_vmem_write_function_t write;
    /**
       Read function.
    */
    const gs_vmem_read_function_t  read;
    /**
       Lock function.
    */
    const gs_vmem_lock_function_t  lock;
    /**
       Information function.
    */
    const gs_vmem_info_function_t  info;
    /**
       Check function.
    */
    const gs_vmem_check_function_t  check;
} gs_vmem_driver_t;

/**
   Virtual memory mapping.

   @note Call gs_vmem_set_map() for registering mappings.
*/
struct gs_vmem {
    /**
       Logical name of entry.
       The name of the last entry in the VMEM table must be NULL.
    */
    const char *const name;
    /**
       Virtual memory start.
    */
    gs_address_t virtmem;
    /**
       Physical memory start.
       This address only makes sense for the VMEM driver.
    */
    gs_address_t physmem;
    /**
       Size of memory block.
    */
    const size_t size;
    /**
       Driver function.
    */
    const gs_vmem_driver_t* drv;
    /**
       Driver data.
    */
    const void* drv_data;
};

/**
   Set VMEM mapping.
   Must be done for the API to work.
   @param[in] map VMEM mapping table, must be terminated with an NULL entry.
   @return_gs_error_t
*/
gs_error_t gs_vmem_set_map(const gs_vmem_t * map);

/**
   Return VMEM map.
*/
const gs_vmem_t * gs_vmem_get_map(void);

/**
   Print all VMEM entries to stdout.
   @param[in] out output stream
   @return_gs_error_t
*/
gs_error_t gs_vmem_list(FILE * out);

/**
   Get VMEM entry by name.
   @param[in] name name of VMEM entry.
   @return VMEM mapping or NULL if not found.
*/
const gs_vmem_t * gs_vmem_get_by_name(const char * name);

/**
   Lock/un-lock VMEM area.
   @param[in] name name of VMEM entry.
   @param[in] on   Enable/Disable lock.
   @return GS_ERROR_NOT_FOUND area not found.
   @return GS_ERROR_NOT_SUPPORTED if locking isn't supported.
   @return_gs_error_t
*/
gs_error_t gs_vmem_lock_by_name(const char * name, bool on);

/**
   Lock/un-lock all VMEM areas.
   @param[in] on lock on or off.
   @return_gs_error_t
*/
gs_error_t gs_vmem_lock_all(bool on);

/**
   Get information for VMEM entry
   @param[in] name   VMEM entry.
   @param[in] buffer user allocated buffer for returning information.
   @param[in] buffer_size size (length) of \a buffer.
   @return_gs_error_t
*/
gs_error_t gs_vmem_info_by_name(const char * name, char * buffer, size_t buffer_size);

/**
   memcpy on VMEM.
   @note if no VMEM entries are found, a normal memcpy is called with the provided pointers.
   @param[in] to to location.
   @param[in] from from location.
   @param[in] size number of bytes to copy.
*/
void* gs_vmem_cpy(void* to, const void* from, size_t size);

/**
   Macro for calling gs_vmem_cpy().
*/
#define GS_VMEM_CPY(to, from, size) gs_vmem_cpy(to, from, size)

/**
   Macro for calling gs_vmem_cpy().
   @deprecated Use gs_vmem_cpy() directly.
*/
#define VMEM_CPY(to, from, size)    gs_vmem_cpy(to, from, size)

/**
   Register VMEM commands.
   @return_gs_error_t
*/
gs_error_t gs_vmem_register_commands(void);

#ifdef __cplusplus
}
#endif
#endif
