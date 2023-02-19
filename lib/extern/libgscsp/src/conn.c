/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/csp/csp.h>
#include <../lib/libcsp/src/csp_conn.h>  // internal libcsp header

unsigned int gs_csp_conn_get_open(void)
{
    unsigned int open = 0;
    size_t max_connections;
    const csp_conn_t * connections = csp_conn_get_array(&max_connections);
    if (connections) {
        for (unsigned int i = 0; i < max_connections; ++i) {
            if (connections[i].state != CONN_CLOSED) {
                ++open;
            }
        }
    }

    // csp_conn_print_table();

    return open;
}
