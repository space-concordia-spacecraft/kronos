#ifndef PARAM_PARAM_TYPES_H
#define PARAM_PARAM_TYPES_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Legacy/deprecated parameter types and definitions - use <gs/param/types.h>.
*/

#include <gs/param/table.h>
#include <gs/param/rparam.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PARAM_NAME_LEN	GS_PARAM_MAX_NAME

/*
  Legacy parameter type definition - matches gs_param_type_t exactly.
 */
typedef gs_param_type_t param_type_t;

/*
  Legacy table row definition - matches gs_param_table_row_t exactly.
*/
typedef struct param_table_s {
    uint16_t addr;
    param_type_t type;
    uint8_t size;
    uint8_t count; // -> array_size
    uint8_t flags;
    char name[MAX_PARAM_NAME_LEN];
} param_table_t;

#define PARAM_COUNT             gs_max(GS_PGM_UINT8((param)->count), 1)
#define PARAM_SIZE              GS_PARAM_SIZE(param)
#define PARAM_TYPE              GS_PARAM_TYPE(param)
#define PARAM_ADDR              GS_PARAM_ADDR(param)
#define PARAM_FLAGS             GS_PARAM_FLAGS(param)

#define RPARAM_QUERY_MAX_LEN	GS_RPARAM_MAX_QUERY

#define PARAM_MAX_FILESIZE	0x400

struct param_index_s;    

/*
  Legacy callback - matches gs_param_callback_func_t exactly.
*/
typedef void (*param_callback_func)(uint16_t addr, struct param_index_s * index);

typedef gs_param_table_id_t param_mem;

/*
  Legacy table instance definition - matches gs_param_table_instance_t exactly.
*/
typedef struct param_index_s {
    const char * name;
    param_mem mem_id;
    const param_table_t * table;
    unsigned int count;
    void * physaddr;
    unsigned int size;
    const gs_param_function_interface_t function_interface;
    const uint16_t table_chksum;
    const uint16_t chksum2;
    const uint16_t checksum_le;
    const gs_mutex_t lock;
    param_callback_func callback;
    const char *   const var1;
    const char *   const var2;
    const void *   const var3;
    const void *   const var4;
    const void *   const var5;
    const void *   const var6;
    const void *   const var7;
    const uint32_t       var8;
} param_index_t;

/**
   Return base size of a parameter type.
*/
static inline uint8_t param_type_size(param_type_t type)
{
    return gs_param_type_size(type);
}
    
#ifdef __cplusplus
}
#endif
#endif
