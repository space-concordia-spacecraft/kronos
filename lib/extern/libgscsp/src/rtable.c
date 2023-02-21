/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/csp/rtable.h>
#include <gs/util/string.h>
#include "local.h"

// Return interface, if only one configured
static csp_iface_t * get_single_if(unsigned int * return_count)
{
    unsigned int count = 0;
    csp_iface_t * found = NULL;

    for (csp_iface_t * ifc = csp_iflist_get(); ifc; ifc = ifc->next) {
        if (strcasecmp(ifc->name, "LOOP") == 0) {
            // ignore loopback
        } else {
            ++count;
            found = ifc;
        }
    }
    *return_count = count;
    return found;
}

gs_error_t gs_csp_rtable_load(const char * rtable, bool set_default_route, bool use_command_line_option)
{
#if GS_CSP_COMMAND_LINE_SUPPORT
    if (use_command_line_option && gs_string_empty(rtable)) {
        // try rtable from command line (if set)
        rtable = gs_csp_command_line_get_rtable();
    }
#endif

    if (gs_string_empty(rtable) == false) {

        if (csp_rtable_check(rtable) > 0) {
            csp_rtable_load(rtable);
            log_info("%s: loaded routing table [%s]", __FUNCTION__, rtable);
            return GS_OK;
        }

        log_warning("%s: ignoring route table: [%s] due to error(s)", __FUNCTION__, rtable);
    }

    if (set_default_route) {
        unsigned int count = 0;
        csp_iface_t * ifc = get_single_if(&count);
        if (count == 0) {
            log_warning("%s: no interfaces configured", __FUNCTION__);
            return GS_ERROR_NOT_FOUND;
        }

        if (count > 1) {
            log_warning("%s: %u interfaces configured - will not set default routes", __FUNCTION__, count);
            return GS_ERROR_AMBIGUOUS;
        }

        // set default route
        int res = csp_route_set(CSP_DEFAULT_ROUTE, ifc, CSP_NO_VIA_ADDRESS);
        if (res != CSP_ERR_NONE) {
            log_warning("%s: failed to set default route on interface: [%s], CSP error: %d", __FUNCTION__, ifc->name, res);
            return gs_csp_error(res);
        }
    }

    return GS_OK;
}
