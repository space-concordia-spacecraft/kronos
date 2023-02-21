/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
 * NanoCom firmware
 *
 */

#include <param/rparam_client.h>
#include <ax100.h>

int ax100_get_hk(param_index_t * mem, uint8_t node, uint32_t timeout)
{
    mem->table = ax100_telem;
    mem->count = AX100_TELEM_COUNT;

    return rparam_get_full_table(mem, node, AX100_PORT_RPARAM, AX100_PARAM_TELEM, timeout);
}
