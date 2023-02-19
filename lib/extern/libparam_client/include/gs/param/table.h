#ifndef GS_PARAM_TABLE_H
#define GS_PARAM_TABLE_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Client table API.
*/

#include <gs/param/types.h>
#include <gs/util/stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Allocate table memory when creating a table.

   Flags must be in range: bit 16 - 23, to avoid clash with other parts of the parameter system.
*/
#define GS_PARAM_TABLE_F_ALLOC_MEMORY  0x0100
/**
   Allocate table rows.

   Flags must be in range: bit 16 - 23, to avoid clash with other parts of the parameter system.
*/
#define GS_PARAM_TABLE_F_ALLOC_ROWS    0x0200
/**
   Disable table locking.

   Flags must be in range: bit 16 - 23, to avoid clash with other parts of the parameter system.
*/
#define GS_PARAM_TABLE_F_NO_LOCK       0x0400

/**
   Calculate memory size based on table rows.

   @param[in] rows rows
   @param[in] row_count row count.
   @return size of table or 0 in case of invalid arguments.
*/
size_t gs_param_calc_table_size(const gs_param_table_row_t * rows, size_t row_count);

/**
   Return size of table instance.
*/
size_t gs_param_table_instance_size(void);

/**
   Clear (and check size) of memory for table instance.

   @param[in] var user allocated space of at least gs_param_table_instance_size() bytes.
   @param[in] var_size of \a var.
   @return table instance
   @see gs_param_table_instance_size()
   @see #GS_PARAM_TINST_VAR
*/
gs_param_table_instance_t * gs_param_table_instance_clear(void * var, size_t var_size);

/**
   Allocates aligned space on the stack for a table instance structure.
   @param[in] var name of table instance variable.
*/
#define GS_PARAM_TINST_VAR(var) uint8_t var##__data [gs_param_table_instance_size()] __attribute__ ((aligned(4))); gs_param_table_instance_t * var = gs_param_table_instance_clear(var##__data, sizeof(var##__data))

/**
   Allocate memory for table instance.

   Use gs_param_table_free() to free any internal resources.

   Use standard free() to free allocated memory.

   @return table instance on success, otherwise NULL.
*/
gs_param_table_instance_t * gs_param_table_instance_alloc(void);

/**
   Find row by name.

   @param[in] name parameter name.
   @param[in] rows rows
   @param[in] row_count row count.
   @return row or NULL if not found.
*/
const gs_param_table_row_t * gs_param_row_by_name(const char * name, const gs_param_table_row_t * rows, size_t row_count);

/**
   Find row by address.

   @param[in] addr parameter address.
   @param[in] rows rows
   @param[in] row_count row count.
   @return row or NULL if not found.
*/
const gs_param_table_row_t * gs_param_row_by_address(uint16_t addr, const gs_param_table_row_t * rows, size_t row_count);

/**
   Return table memory.

   @note handle with care - any read/write should be atomic to prevent inconsistent data.

   @param[in] tinst table instance
   @param[out] return_size if not NULL, the memory size is returned.
   @return pointer to the table's data memory.
*/
void * gs_param_table_get_memory(gs_param_table_instance_t * tinst, size_t * return_size);

/**
   Return table rows.

   @param[in] tinst table instance
   @param[out] return_count if not NULL, the row count is returned.
   @return pointer to the table rows.
*/
const gs_param_table_row_t * gs_param_table_get_rows(gs_param_table_instance_t * tinst, size_t * return_count);
    
/**
   Lock table (recursive).

   @param[in] tinst table instance
   @return_gs_error_t
*/
gs_error_t gs_param_table_lock(gs_param_table_instance_t * tinst);

/**
   Unlock table.

   Unlock must be called once for every time gs_param_table_lock() has been called.

   @param[in] tinst table instance
   @return_gs_error_t
*/
gs_error_t gs_param_table_unlock(gs_param_table_instance_t * tinst);

/**
   Free internal resources and clears instance.

   @param[in] tinst table instance
   @return_gs_error_t
*/
gs_error_t gs_param_table_free(gs_param_table_instance_t * tinst);

/**
   Print a single parameter on stream.

   @param[in] tinst table instanc.
   @param[in] row row to print.
   @param[in] list_data \a true includes parameter value.
   @param[in] flags flags to control output format: #GS_PARAM_F_SHOW_SCIENTIFIC, #GS_PARAM_F_SHOW_HEX.
   @param[in] out output stream.
   @return_gs_error_t
*/
gs_error_t gs_param_list_single_to_stream(gs_param_table_instance_t * tinst, const gs_param_table_row_t * row, bool list_data, uint32_t flags, FILE * out);

/**
   Print a single parameter on stdout.

   @param[in] tinst table instanc.
   @param[in] row row to print.
   @param[in] list_data \a true includes parameter value.
   @return_gs_error_t
*/
static inline gs_error_t gs_param_list_single(gs_param_table_instance_t * tinst, const gs_param_table_row_t * row, bool list_data)
{
    return gs_param_list_single_to_stream(tinst, row, list_data, 0, stdout);
}

/**
   Print entire table on stream.

   @param[in] tinst table instanc.
   @param[in] list_data \a true includes parameter value.
   @param[in] flags flags to control output format: #GS_PARAM_F_SHOW_SCIENTIFIC, #GS_PARAM_F_SHOW_HEX.
   @param[in] out output stream.
   @return_gs_error_t
*/
gs_error_t gs_param_list_to_stream(gs_param_table_instance_t * tinst, bool list_data, uint32_t flags, FILE * out);

/**
   Print entire table on stdout.

   @param[in] tinst table instanc.
   @param[in] list_data \a true includes parameter value.
   @return_gs_error_t
*/
static inline gs_error_t gs_param_list(gs_param_table_instance_t * tinst, bool list_data)
{
    return gs_param_list_to_stream(tinst, list_data, 0, stdout);
}
    
/**
   Convert string to parameter.

   @param[in] row row defining the parameter to convert.
   @param[in] string string to convert.
   @param[out] return_value user supplied buffer for returning the value - must be at least the size specified in \a row
   @return_gs_error_t
*/
gs_error_t gs_param_from_string(const gs_param_table_row_t * row, const char * string, void * return_value);

/**
   Convert parameter to string.

   @param[in] row row defining the parameter to convert.
   @param[in] value parameter value to convert.
   @param[in] with_type \a true includes data type.
   @param[in] flags flags to control output format: #GS_PARAM_F_SHOW_SCIENTIFIC, #GS_PARAM_F_SHOW_HEX.
   @param[out] buf user supplied buffer of \a buf_size bytes.
   @param[in] buf_size size of \a buf in bytes.
   @param[in] buf_pos buffer position to insert string.
   @param[out] return_buf_written number of bytes written to \a buf.
   @return_gs_error_t
*/
gs_error_t gs_param_to_string2(const gs_param_table_row_t * row, const void * value, bool with_type, uint32_t flags, char * buf, unsigned int buf_size, unsigned int buf_pos, unsigned int * return_buf_written);

/**
   Convert parameter to string.

   @param[in] row row defining the parameter to convert.
   @param[in] value parameter value to convert.
   @param[in] with_type \a true includes data type.
   @param[out] buf user supplied buffer of \a buf_size bytes.
   @param[in] buf_size size of \a buf in bytes.
   @param[in] buf_pos buffer position to insert string.
   @param[out] return_buf_written number of bytes written to \a buf.
   @return_gs_error_t
*/
static inline gs_error_t gs_param_to_string(const gs_param_table_row_t * row, const void * value, bool with_type, char * buf, unsigned int buf_size, unsigned int buf_pos, unsigned int * return_buf_written)
{
    return gs_param_to_string2(row, value, with_type, 0, buf, buf_size, buf_pos, return_buf_written);
}

/**
   Convert parameter type to string.

   @param[in] type parameter type.
   @return pointer to a static string.
*/
const char * gs_param_type_to_string(gs_param_type_t type);

/**
   Return size of parameter type.

   @param[in] type parameter type.
   @return size of parameter type in bytes.
*/
uint8_t gs_param_type_size(gs_param_type_t type);

/**
   Get table checksum - little-endian.
   @note Use/exchange gs_param_table_checksum_be(), as this is calculated the same on all platforms.
   @param[in] tinst table instance.
   @returns 16-bit fletcher checksum
*/
uint16_t gs_param_table_checksum_le(gs_param_table_instance_t * tinst);

/**
   Get table checksum - big-endian/network-order (prefered).
   @param[in] tinst table instance.
   @returns 16-bit fletcher checksum
*/
uint16_t gs_param_table_checksum_be(gs_param_table_instance_t * tinst);

/**
   Get table checksum - host-order (not cross-platform).
   @deprecated use gs_param_table_checksum_be()
   @param[in] tinst table instance.
   @returns 16-bit fletcher checksum
*/
uint16_t gs_param_table_checksum(gs_param_table_instance_t * tinst);

/**
   Get table checksum - big-endian.
   @deprecated use gs_param_table_checksum_be()
   @param[in] tinst table instance.
   @returns 16-bit fletcher checksum
*/
static inline uint16_t gs_param_table_checksum2(gs_param_table_instance_t * tinst)
{
    return gs_param_table_checksum_be(tinst);
}
    
/**
   Get/read parameter from table.

   @param[in] tinst table instanc.
   @param[in] addr parameter address (offset in table).
   @param[in] type parameter type.
   @param[out] return_value value of parameter - user supplied memory of at least \a size size.
   @param[in] size number of bytes to get/read - must match \a type, e.g. 4 bytes for an uint32_t.
   @param[in] flags flags.
   @return_gs_error_t
*/
gs_error_t gs_param_get(gs_param_table_instance_t * tinst, uint16_t addr, gs_param_type_t type, void * return_value, size_t size, uint32_t flags);

/**
   Set/write parameter in table.

   @param[in] tinst table instanc.
   @param[in] addr parameter address (offset in table).
   @param[in] type parameter type.
   @param[in] value value of parameter.
   @param[in] size number of bytes to set/write - must match \a type, e.g. 4 bytes for an uint32_t.
   @param[in] flags flags.
   @return_gs_error_t
*/
gs_error_t gs_param_set(gs_param_table_instance_t * tinst, uint16_t addr, gs_param_type_t type, const void * value, size_t size, uint32_t flags);

/**
   Get string parameter.

   @param[in] tinst table instanc.
   @param[in] addr parameter address (offset in table).
   @param[out] buf value of parameter - user supplied memory of at least parameter size + 1 to hold NUL termination.
   @param[in] buf_size size of \a buf - ensure room for NUL termination.
   @param[in] flags flags.
   @return GS_ERROR_OVERFLOW if string + NUL termination exceeds \a buf_size.
   @return_gs_error_t
*/
gs_error_t gs_param_get_string(gs_param_table_instance_t * tinst, uint16_t addr, char * buf, size_t buf_size, uint32_t flags);

/**
   Set string parameter.

   @param[in] tinst table.
   @param[in] addr parameter address (offset in table).
   @param[in] value string to save - parameter must be able to hold string + NUL termination.
   @param[in] flags flags.
   @return GS_ERROR_OVERFLOW if string + NUL termination exceeds parameter size.
   @return_gs_error_t
*/
gs_error_t gs_param_set_string(gs_param_table_instance_t * tinst, uint16_t addr, const char * value, uint32_t flags);

/**
   Get data parameter.

   @param[in] tinst table instanc.
   @param[in] addr parameter address (offset in table).
   @param[out] buf value of parameter - user supplied memory of at least parameter size.
   @param[in] buf_size size of \a buf.
   @param[in] flags flags.
   @return GS_ERROR_OVERFLOW if parameter size is greater than \a buf_size.
   @return_gs_error_t
*/
gs_error_t gs_param_get_data(gs_param_table_instance_t * tinst, uint16_t addr, void * buf, size_t buf_size, uint32_t flags);

/**
   Set data parameter.

   @param[in] tinst table instanc.
   @param[in] addr parameter address (offset in table).
   @param[in] value value of parameter.
   @param[in] value_size size of \a value.
   @param[in] flags flags.
   @return GS_ERROR_OVERFLOW if parameter size is greater than \a buf_size.
   @return_gs_error_t
*/
gs_error_t gs_param_set_data(gs_param_table_instance_t * tinst, uint16_t addr, const void * value, size_t value_size, uint32_t flags);

/**
   Macro for expanding get/set functions.
   @param[in] name function suffix name.
   @param[in] native_type native type
   @param[in] param_type parameter type
*/
#define GS_PARAM_PASTE(name, native_type, param_type)                   \
    static inline gs_error_t gs_param_get_##name(gs_param_table_instance_t * tinst, uint16_t addr, native_type * buf, uint32_t flags) { \
        return gs_param_get(tinst, addr, param_type, buf, sizeof(*buf), flags); \
    }									\
    static inline native_type gs_param_get_##name##_nc(gs_param_table_instance_t * tinst, uint16_t addr, uint32_t flags) { \
        native_type value = 0;                                          \
        gs_param_get(tinst, addr, param_type, &value, sizeof(value), flags); \
        return value;                                                   \
    }									\
    static inline gs_error_t gs_param_set_##name(gs_param_table_instance_t * tinst, uint16_t addr, native_type value, uint32_t flags) { \
        return gs_param_set(tinst, addr, param_type, &value, sizeof(value), flags); \
    }

/**
   Get/set boolean.
*/
GS_PARAM_PASTE(bool, bool, GS_PARAM_BOOL)
/**
   Get/set uint8.
*/
GS_PARAM_PASTE(uint8, uint8_t, GS_PARAM_UINT8)
/**
   Get/set uint16.
*/
GS_PARAM_PASTE(uint16, uint16_t, GS_PARAM_UINT16)
/**
   Get/set uint32.
*/
GS_PARAM_PASTE(uint32, uint32_t, GS_PARAM_UINT32)
/**
   Get/set uint64.
*/
GS_PARAM_PASTE(uint64, uint64_t, GS_PARAM_UINT64)
/**
   Get/set int8.
*/
GS_PARAM_PASTE(int8, int8_t, GS_PARAM_INT8)
/**
   Get/set int16.
*/
GS_PARAM_PASTE(int16, int16_t, GS_PARAM_INT16)
/**
   Get/set int32.
*/
GS_PARAM_PASTE(int32, int32_t, GS_PARAM_INT32)
/**
   Get/set int64.
*/
GS_PARAM_PASTE(int64, int64_t, GS_PARAM_INT64)
/**
   Get/set double.
*/
GS_PARAM_PASTE(double, double, GS_PARAM_DOUBLE)
/**
   Get/set float.
*/
GS_PARAM_PASTE(float, float, GS_PARAM_FLOAT)

#ifdef __cplusplus
}
#endif
#endif
