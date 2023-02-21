/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */


#include <gs/param/internal/rparam.h>

/**
   Rparam query handle.
*/
typedef struct {
    /**
       CSP node.
    */
    uint8_t node;
    /**
       Remote table id.
    */
    gs_param_table_id_t table_id;
    /**
       Remote table (definition) checksum.
    */
    uint16_t checksum;
    /**
       Timeout (mS).
    */
    uint32_t timeout_ms;
    /**
       If quite - no output to stdout.
    */
    bool quiet;
    /**
       Auto send.
    */
    bool auto_send;
    /**
       Type/action.
    */
    int action;
    /**
       Size of current query.
    */
    unsigned int length;
    /**
       Estimated total 'get' size.
    */
    unsigned int get_size;
    /**
       Space for payload data.
       @note must be last in struct.
    */
    union {
        uint16_t addr[0];
        uint8_t packed[GS_RPARAM_QUERY_MAX_PAYLOAD];
    } payload;
} gs_rparam_query_handle_t;

/**
   Set whether rparam API should print to stdout or not.

   @param[in] handle handle
   @param[in] quiet \a true print to stdout.
*/
void gs_rparam_query_set_quiet(gs_rparam_query_handle_t * handle, bool quiet);

/**
   Return true if any query has been set.

   @param[in] handle handle
   @return \a true if any query has been set.
*/
bool gs_rparam_query_is_set(gs_rparam_query_handle_t * handle);

/**
   Add a 'get' query to the current query, after a succesfull rparam_query_send()
   the parameter value can be read using rparam_queury_get_value()

   @param[in] handle handle
   @param[in] tinst table.
   @param[in] param_name name of the parameter.
   @return_gs_error_t
   @see rparam_query_send()
*/
gs_error_t gs_rparam_query_get(gs_rparam_query_handle_t * handle, gs_param_table_instance_t * tinst, const char* param_name);

/**
   Add a 'set' query to the current query. Use rparam_query_send() to execute the set query.

   @param[in] handle handle
   @param[in] tinst table.
   @param[in] param_name name of the parameter to set
   @param[in] values array of values to set, multiple values can be set for array type parameters
   @param[in] value_count number of elements in \a values
   @return_gs_error_t
   @see rparam_query_send()
*/
gs_error_t gs_rparam_query_set(gs_rparam_query_handle_t * handle, gs_param_table_instance_t * tinst, const char* param_name, const char * const * values, uint8_t value_count);

/**
   Send the current query

   @param[in] handle handle
   @param[in] tinst table.
   @return_gs_error_t
*/
gs_error_t gs_rparam_query_send(gs_rparam_query_handle_t * handle, gs_param_table_instance_t * tinst);

/**
   Reset/clear the current query
   @param[in] handle handle
*/
void gs_rparam_query_reset(gs_rparam_query_handle_t * handle);
