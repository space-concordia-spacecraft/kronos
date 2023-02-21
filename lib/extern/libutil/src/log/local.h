/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/log/log.h>
#include <gs/util/log/appender/appender.h>

struct gs_log_list {
    union {
        gs_log_group_t * group;
        gs_log_appender_t *appender;
    } data;
    struct gs_log_list * next;
};

/**
   De-register appender for the given log group.

   @note The de-register function is not safe when logging is active, this function
   is mostly for test and should only be used in product code with extreme caution.
   If logging is still not active, this function can be used safely.

   @param[in] group_name Name of the group.
   @param[in] appender_name Name of appender to de-register for this group.
   @return gs_error_t
*/
gs_error_t gs_log_group_deregister_appender(const char * group_name, const char * appender_name);

bool gs_log_is_group_all(const char * name);
