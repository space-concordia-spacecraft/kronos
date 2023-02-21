/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include "../cmd/vmem.h"
#include <gs/util/vmem.h>
#include <gs/util/hexdump.h>

int gs_vmem_cmd_vmem_read_handler(gs_command_context_t * ctx)
{
    if (gs_vmem_get_map() == NULL) {
        return GS_ERROR_NOT_FOUND;
    }
    
    void * addr;
    if (gs_string_to_pointer(ctx->argv[1], &addr)) {
        return GS_ERROR_ARG;
    }

    uint32_t length;
    if (gs_string_to_uint32(ctx->argv[2], &length)) {
        return GS_ERROR_ARG;
    }

    char data[length];
    void* to = gs_vmem_cpy(data, addr, length);
    if (to == NULL) {
        return GS_ERROR_ARG;
    }

    gs_hexdump_to_stream(data, length, addr, ctx->out);

    return GS_OK;
}

static unsigned int to_int(char c)
{
    if (c >= '0' && c <= '9') return      c - '0';
    if (c >= 'A' && c <= 'F') return 10 + c - 'A';
    if (c >= 'a' && c <= 'f') return 10 + c - 'a';
    return -1;
}

int gs_vmem_cmd_vmem_write_handler(gs_command_context_t * ctx)
{
    if (gs_vmem_get_map() == NULL) {
        return GS_ERROR_NOT_FOUND;
    }

    void * addr;
    if (gs_string_to_pointer(ctx->argv[1], &addr)) {
        return GS_ERROR_ARG;
    }

    int len = strlen(ctx->argv[2]) / 2;
    char data[len];

    for (int i = 0; (i < len); ++i) {
        data[i] = 16 * to_int(ctx->argv[2][2*i]) + to_int(ctx->argv[2][2*i+1]);
    }

    gs_vmem_cpy(addr, data, len);
    return GS_OK;
}

int gs_vmem_cmd_vmem_list_handler(gs_command_context_t * ctx)
{
    return gs_vmem_list(ctx->out);
}

int gs_vmem_cmd_vmem_info_handler(gs_command_context_t * ctx)
{
    const gs_vmem_t * mem = gs_vmem_get_map();
    if (mem) {
        unsigned int found = 0;
        char vinfo[100];
        for (; mem->name; ++mem) {
            if (found == 0) {
                fprintf(ctx->out, "%-25s %-10s %-12s %s\r\n", "name", "virt.", "size", "info");
            }

            vinfo[0] = 0;
            gs_vmem_info_by_name(mem->name, vinfo, sizeof(vinfo));

            fprintf(ctx->out, "%-25s 0x%08lx 0x%04x/%-5u %s\r\n",
                    mem->name, (unsigned long) mem->virtmem.u, (unsigned int) mem->size, (unsigned int) mem->size, vinfo);
            ++found;
        }
        if (found) {
            return GS_OK;
        }
    }
    return GS_ERROR_NOT_FOUND;
}

int gs_vmem_cmd_vmem_lock_handler(gs_command_context_t * context)
{
    return gs_vmem_lock_by_name(context->argv[1], true);
}

int gs_vmem_cmd_vmem_unlock_handler(gs_command_context_t * context)
{
    return gs_vmem_lock_by_name(context->argv[1], false);
}
