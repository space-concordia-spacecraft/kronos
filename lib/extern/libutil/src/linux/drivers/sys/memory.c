/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <sys/sysinfo.h>
#include <gs/util/drivers/sys/memory.h>

gs_error_t gs_mem_get_int_ram_stat(gs_mem_ram_stat_t * ram_stat)
{
    return GS_ERROR_NOT_SUPPORTED;
}

gs_error_t gs_mem_get_ext_ram_stat(gs_mem_ram_stat_t * ram_stat)
{
    struct sysinfo info;
    int res = sysinfo(&info);
    if (res == 0) {
        ram_stat->total = info.totalram;
        ram_stat->max_available = -1;
        ram_stat->min_available = -1;
        ram_stat->available = info.freeram;
        return GS_OK;
    }
    return GS_ERROR_UNKNOWN;
}

gs_mem_ram_type_t gs_mem_get_ram_default()
{
    return GS_MEM_RAM_TYPE_EXTERNAL;
}
