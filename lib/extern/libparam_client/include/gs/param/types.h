#ifndef GS_PARAM_TYPES_H
#define GS_PARAM_TYPES_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Parameter types.
*/

#include <gs/util/mutex.h>
#include <gs/util/pgm.h>
#include <gs/util/minmax.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Macros for accessing table row members.
   These macros can be used to access members in a cross-platform way, compensating for the AVR8's memory model.
   @{
*/
#define GS_PARAM_ARRAY_SIZE(p)  gs_max(GS_PGM_UINT8((p)->array_size), 1)
#define GS_PARAM_SIZE(p)        GS_PGM_UINT8((p)->size)
#define GS_PARAM_TYPE(p)        GS_PGM_UINT8((p)->type)
#define GS_PARAM_ADDR(p)        GS_PGM_UINT16((p)->addr)
#define GS_PARAM_FLAGS(p)       GS_PGM_UINT8((p)->flags)
/** @} */

/**
   Max parameter name - including 0 termination.
   @note In some rare/old table definitions, the name may not be NULL terminated.
*/
#define GS_PARAM_MAX_NAME	14

/**
   Parameter flags.
   Flags must be in range: bit 0 - 7, to avoid clash with other parts of the parameter system.
*/
typedef enum {
    /**
       Parameter will be stored in configured auto-persist store when set.
       @note Flag must be specified when setting the parameter.
    */
    GS_PARAM_F_AUTO_PERSIST = (1 << 0),
    /**
       @deprecated Not supported in version 4.0
    */
    PARAM_F_READONLY = (1 << 1),
    /**
       Skip callback, when parameter is set.
       @note Flag must be specified when setting the parameter.
    */
    GS_PARAM_F_NO_CALLBACK = (1 << 2),
    /**
       Show/display parameter in hex.
    */
    GS_PARAM_F_SHOW_HEX = (1 << 3),
    /**
       Show double/float using scientific notation.
    */
    GS_PARAM_F_SHOW_SCIENTIFIC = (1 << 4),

    PARAM_F_NOCALLBACK = GS_PARAM_F_NO_CALLBACK,
    PARAM_F_PERSIST = GS_PARAM_F_AUTO_PERSIST,
} gs_param_flags_t;

/**
 * Parameter types.
 */
typedef enum __attribute__((__packed__)) {
    /**
       Unsigned 8 bit (uint8_t).
    */
    GS_PARAM_UINT8 = 0,
    /**
       Unsigned 16 bit (uint16_t).
    */
    GS_PARAM_UINT16 = 1,
    /**
       Unsigned 32 bit (uint32_t).
    */
    GS_PARAM_UINT32 = 2,
    /**
       Unsigned 64 bit (uint64_t).
    */
    GS_PARAM_UINT64 = 3,
    /**
       Signed 8 bit (int8_t).
    */
    GS_PARAM_INT8 = 4,
    /**
       Signed 16 bit (int16_t).
    */
    GS_PARAM_INT16 = 5,
    /**
       Signed 32 bit (int32_t).
    */
    GS_PARAM_INT32 = 6,
    /**
       Signed 64 bit (int64_t).
    */
    GS_PARAM_INT64 = 7,
    /**
       @deprecated - use #GS_PARAM_UINT8 and #GS_PARAM_F_SHOW_HEX.
    */
    PARAM_X8 = 8,
    /**
       @deprecated - use #GS_PARAM_UINT16 and #GS_PARAM_F_SHOW_HEX.
    */
    PARAM_X16 = 9,
    /**
       @deprecated - use #GS_PARAM_UINT32 and #GS_PARAM_F_SHOW_HEX.
    */
    PARAM_X32 = 10,
    /**
       @deprecated - use #GS_PARAM_UINT64 and #GS_PARAM_F_SHOW_HEX.
    */
    PARAM_X64 = 11,
    /**
       Double.
    */
    GS_PARAM_DOUBLE = 12,
    /**
       Float.
    */
    GS_PARAM_FLOAT = 13,
    /**
       C or null-terminated string.
       @note The specified \a size must include space for the NUL character.
    */
    GS_PARAM_STRING = 14,
    /**
       Data (binary blob).
       Binary blob: [0, 0x40, 0x4f] -> '00404f'
    */
    GS_PARAM_DATA = 15,
    /**
       Boolean.
       Expected same size as uint8_t.
    */
    GS_PARAM_BOOL = 16,

    PARAM_UINT8 = GS_PARAM_UINT8,
    PARAM_UINT16 = GS_PARAM_UINT16,
    PARAM_UINT32 = GS_PARAM_UINT32,
    PARAM_UINT64 = GS_PARAM_UINT64,
    PARAM_INT8 = GS_PARAM_INT8,
    PARAM_INT16 = GS_PARAM_INT16,
    PARAM_INT32 = GS_PARAM_INT32,
    PARAM_INT64 = GS_PARAM_INT64,
    PARAM_DOUBLE = GS_PARAM_DOUBLE,
    PARAM_FLOAT = GS_PARAM_FLOAT,
    PARAM_STRING = GS_PARAM_STRING,
    PARAM_DATA = GS_PARAM_DATA,
    PARAM_BOOL = GS_PARAM_BOOL,
} gs_param_type_t;

/**
   Table row.

   A table row defines one parameter, and a table is defined by one or more rows.

   @note Make sure to update gs_param_table_checksum2(), if adding fields > 1 byte.

   @note AVR8: Table definitions must be located in \a program memory, i.e. must be const.
*/
typedef struct __attribute__((__packed__)) {
    /**
       Address (or offset) in table.
    */
    uint16_t addr;
    /**
       Type.
    */
    gs_param_type_t type;
    /**
       Size of element.
       uint32_t = 4, string[5] = 5 (4 characters + 1 for NUL), etc.
    */
    uint8_t size;
    /**
       Array size.
       Size greater than 1, will make the parameter an array - if the value is 0 or 1, the parameter is not an array.
    */
    uint8_t array_size;
    /**
       Flags.
       @see gs_param_flags_t
    */
    uint8_t flags;
    /**
       Name (C string).
       @note In some rare/old table definitions, the name may not be NUL terminated.
    */
    char name[GS_PARAM_MAX_NAME];
} gs_param_table_row_t;

/**
   Table instance.
*/
typedef struct gs_param_table_instance gs_param_table_instance_t;

/**
   Table id.

   Tables can be associated with a number/id, which normally is unique on a specific node.
*/
typedef uint8_t gs_param_table_id_t;

/**
   Undefined table id.
*/
#define GS_PARAM_UNDEFINED_TABLE_ID 255

/**
   Function for setting a parameter.

   @param[in] context user context/reference.
   @param[in] tinst table instance.
   @param[in] addr parameter address.
   @param[in] type parameter type.
   @param[in] item parameter value.
   @param[in] size parameter size (e.g. how many bytes to copy from \a item).
   @param[in] flags flags related to the operation - these may vary depending on the context.
   @return_gs_error_t
*/
typedef gs_error_t (*gs_param_table_function_set_t)(void * context, gs_param_table_instance_t * tinst, uint16_t addr, gs_param_type_t type, const void * item, size_t size, uint32_t flags);

/**
   Function for getting a parameter.

   @param[in] context user context/reference.
   @param[in] tinst table instance.
   @param[in] addr parameter address.
   @param[in] type parameter type.
   @param[out] item parameter buffer (provided by the caller).
   @param[in] size parameter size (e.g. how many bytes to copy to \a item).
   @param[in] flags flags related to the operation - these may vary depending on the context.
   @return_gs_error_t
*/
typedef gs_error_t (*gs_param_table_function_get_t)(void * context, gs_param_table_instance_t * tinst, uint16_t addr, gs_param_type_t type, void * item, size_t size, uint32_t flags);
    
/**
   Function interface for setting and getting parameters.
   Functions will be invoked, when set/get is called on the table instance.
*/
typedef struct {
    /**
       User context, provided in the callback functions.
    */
    void * context;
    /**
       Called when setting a parameter.
    */
    gs_param_table_function_set_t set;
    /**
       Called when getting a parameter.
    */
    gs_param_table_function_get_t get;
} gs_param_function_interface_t;

/**
   Callback function for changed parameter.
   See gs_param_table_create() for details.
*/
typedef void (*gs_param_callback_func_t)(uint16_t addr, gs_param_table_instance_t * tinst);

#ifdef __cplusplus
}
#endif
#endif
