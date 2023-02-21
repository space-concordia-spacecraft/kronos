#ifndef GS_PARAM_INTERNAL_TYPES_H
#define GS_PARAM_INTERNAL_TYPES_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#if (GS_PARAM_INTERNAL_USE)

#include <gs/param/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Table instance.
*/
struct gs_param_table_instance {
    /**
       Name of table.
    */
    const char * name;

    /**
       Table id.
    */
    gs_param_table_id_t id;

    /**
       Table elements/rows.
    */
    const gs_param_table_row_t * rows;
    /**
       Table element/row count.
    */
    unsigned int row_count;

    /**
       Table memory - volatile parameter store.
       The allocated size must be at least \a memory_size bytes.
    */
    void * memory;

    /**
       Size of table data memory in bytes, normally size of memory allocated \a memory.
       Size must always be specified, even when using function interface and no memory is allocated.
    */
    unsigned int memory_size;

    /**
       Function interface, e.g. get/set.
    */
    gs_param_function_interface_t function_interface;

    /**
       Checksum - based on host order (e.g. le or be).
       @see gs_param_table_checksum()
    */
    uint16_t checksum;

    /**
       Checksum - based on big-endian (address converted to big-endian).
       @see gs_param_table_checksum_be()
    */
    uint16_t checksum_be;

    /**
       Checksum - based on little-endian (address converted to little-endian).
       @see gs_param_table_checksum_le()
    */
    uint16_t checksum_le;
    
    /**
       Lock.
       Internal access/use only, use gs_param_lock() and gs_param_unlock() to lock and un-lock table.
    */
    gs_mutex_t lock;

    /**
       Callback for table (data) change.
    */
    void (*callback)(uint16_t addr, gs_param_table_instance_t * tinst);

    /**
       Store location(s).
       CSV format, e.g. \"persistent,protected\".
    */
    const char * stores;
 
    /**
       Auto-persist.
    */
    struct {
        /**
           Store.
        */
        const char * store;

        /**
           User context(s) for the \a set function.
        */
        void * context1;
        void * context2;

        /**
           Set/write parameter.
        */
        gs_error_t (*set)(gs_param_table_instance_t * tinst, uint16_t addr, gs_param_type_t type, const void * item, size_t size, uint32_t flags);
    } auto_persist;

    /**
       Function for initializing table.
    */
    gs_error_t (*initializer_function)(gs_param_table_instance_t * tinst);

    /**
       Default values for initializing table.
    */
    const void * default_values;
    
    /**
       Future flags.
    */
    uint32_t flags;
};

#ifdef __cplusplus
}
#endif
#endif
#endif
