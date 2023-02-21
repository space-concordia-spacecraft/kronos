#ifndef GS_UTIL_DRIVERS_SYS_MEMORY_H
#define GS_UTIL_DRIVERS_SYS_MEMORY_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Cross platform memory usage API.
*/

#include <gs/util/stdio.h>
#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   RAM usage.
   Unsupported/unknown values are set to -1.
   @see gs_mem_get_int_ram_stat(), gs_mem_get_ext_ram_stat()
*/
typedef struct {
    /**
       Total size of RAM (in bytes).
    */
    long total;
    /**
       Max available bytes for dynamic allocation - 0 if no bytes available for dynamic allocation.
    */
    long max_available;
    /**
       Current free/available bytes for dynamic allocation.
    */
    long available;
    /**
       Lowest free/available bytes for dynamic allocation (since boot).
    */
    long min_available;
} gs_mem_ram_stat_t;

/**
   RAM area/type.
   Defines the different RAM types (external/internal) supported on the various platforms.
 */
typedef enum {
    GS_MEM_RAM_TYPE_INTERNAL = 0, //!< Internal RAM type
    GS_MEM_RAM_TYPE_EXTERNAL = 1, //!< External RAM type
} gs_mem_ram_type_t;

/**
   Get usage of internal RAM.

   @param[out] ram_stat RAM usage.
   @return_gs_error_t
 */
gs_error_t gs_mem_get_int_ram_stat(gs_mem_ram_stat_t * ram_stat);

/**
   Get usage of external RAM.

   @param[out] ram_stat RAM usage.
   @return_gs_error_t
 */
gs_error_t gs_mem_get_ext_ram_stat(gs_mem_ram_stat_t * ram_stat);


/**
   Get usage of a specific RAM area.

   @param[in]  type RAM area/type.
   @param[out] ram_stat RAM usage.
   @return_gs_error_t
 */
gs_error_t gs_mem_get_ram_stat(gs_mem_ram_type_t type, gs_mem_ram_stat_t * ram_stat);


/**
   Get default RAM type

   returns the default RAM type used for allocations (Heap).
   @return gs_mem_ram_type_t
 */
gs_mem_ram_type_t gs_mem_get_ram_default();


/**
   Print RAM status.

   @param[in] ram_stat RAM status
   @param[in] out output stream
   @return_gs_error_t
 */
gs_error_t gs_mem_print_ram_stat(gs_mem_ram_stat_t * ram_stat, FILE * out);

#ifdef __cplusplus
}
#endif
#endif
