/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/vmem.h>
#include <gs/util/string.h>
#include <gs/util/log.h>
#include <gs/util/stdio.h>
#include <gs/util/check.h>

static const gs_vmem_t * g_vmem_map;

gs_error_t gs_vmem_set_map(const gs_vmem_t * map)
{
    g_vmem_map = map;
    return GS_OK;
}

const gs_vmem_t * gs_vmem_get_map(void)
{
    return g_vmem_map;
}

gs_error_t gs_vmem_list(FILE * out)
{
    const gs_vmem_t * mem = g_vmem_map;
    if (mem) {
        unsigned int found = 0;
        for (; mem->name; ++mem) {
            if (found == 0) {
                fprintf(out, "%-25s %-10s %-6s %s\r\n", "name", "virt.", "phys.", "size");
            }
            fprintf(out, "%-25s 0x%08lx 0x%04lx 0x%04x/%u\r\n",
                    mem->name, (unsigned long) mem->virtmem.u, (unsigned long) mem->physmem.u, (unsigned int) mem->size, (unsigned int) mem->size);
            ++found;
        }
        if (found) {
            return GS_OK;
        }
    }
    return GS_ERROR_NOT_FOUND;
}

const gs_vmem_t * gs_vmem_get_by_name(const char * name)
{
    if (name) {
        const gs_vmem_t * mem = g_vmem_map;
        if (mem) {
            for (; mem->name; ++mem) {
                if (strcasecmp(name, mem->name) == 0) {
                    return mem;
                }
            }
        }
    }
    return NULL;
}

gs_error_t gs_vmem_lock_by_name(const char * name, bool on)
{
    const gs_vmem_t * mem = gs_vmem_get_by_name(name);
    if (mem) {
        if (mem->drv && mem->drv->lock) {
            return (mem->drv->lock)(mem, on);
        }
        return GS_ERROR_NOT_SUPPORTED;
    }
    return GS_ERROR_NOT_FOUND;
}

gs_error_t gs_vmem_lock_all(bool on)
{
    const gs_vmem_t * mem = g_vmem_map;
    if (mem) {
        unsigned int locked = 0;
        for (; mem->name; ++mem) {
            if (mem->drv && mem->drv->lock) {
                (mem->drv->lock)(mem, on);
                ++locked;
            }
        }
        if (locked) {
            return GS_OK;
        }
    }
    return GS_ERROR_NOT_FOUND;
}

gs_error_t gs_vmem_info_by_name(const char * name, char * buffer, size_t buffer_size)
{
    const gs_vmem_t * mem = gs_vmem_get_by_name(name);
    if (mem) {
        if (mem->drv && mem->drv->info) {
            return (mem->drv->info)(mem, buffer, buffer_size);
        }
        // add generic information
        gs_snprintf(buffer, buffer_size, "vmem: 0x%08lx -> 0x%04x, size: 0x%04x/%u",
                    (unsigned long) mem->virtmem.u, (unsigned int) mem->physmem.u, (unsigned int) mem->size, (unsigned int) mem->size);
        return GS_OK;
    }
    return GS_ERROR_NOT_FOUND;
}

/**
   @note NO LOGGING - currently the log system uses this interface, and logging can therefor create circular/forever loops.
*/
void * gs_vmem_cpy(void* to, const void* from, size_t size)
{
    /* Search memories */
    const gs_vmem_t *vmem_from = NULL;
    const gs_vmem_t *vmem_to = NULL;
    const gs_vmem_t *mem = g_vmem_map;
    const gs_address_t _to = {.p = to};
    const gs_address_t _from = {.p = (void*) from};

    if (mem) {
        while(mem->name) {
            //printf("0x%lx 0x%lx %"PRIu32" %lu %lu\r\n", mem->start, mem->physmem_start, mem->size, to, from);
            if ((_to.u >= mem->virtmem.u) && (_to.u < mem->virtmem.u + mem->size)) {
                vmem_to = mem;
            }
            if ((_from.u >= mem->virtmem.u) && (_from.u < mem->virtmem.u + mem->size)) {
                vmem_from = mem;
            }
            mem++;
        }
    }

    // VMEM -> VMEM
    if (vmem_to && vmem_from) {
        printf("%s: VMEM to VMEM is not supported\r\n", __FUNCTION__);
        return NULL;
    }

    // RAM -> VMEM
    if (vmem_to) {
        if ((vmem_to->drv == NULL) || (vmem_to->drv->write == NULL)) {
            printf("%s: Writting to VMEM %p is not supported\r\n", __FUNCTION__, to);
            return NULL;
        }
        gs_address_t physaddr = {.u = (_to.u - vmem_to->virtmem.u) + vmem_to->physmem.u};
        //printf("Copying from ram 0x%lx to physaddr 0x%lx %u\r\n", from, physaddr, (unsigned int) size);
        vmem_to->drv->write(vmem_to, physaddr.p, from, size);
        return to;
    }

    // VMEM -> RAM
    if (vmem_from) {
        if (vmem_from->drv == NULL || (vmem_from->drv->read == NULL)) {
            printf("%s: Reading from VMEM %p is not supported\r\n", __FUNCTION__, from);
            return NULL;
        }
        gs_address_t physaddr = {.u = (_from.u - vmem_from->virtmem.u) + vmem_from->physmem.u};
        //printf("Copying from mem physaddr 0x%lx to 0x%lx %u\r\n", physaddr, to, (unsigned int) size);
        vmem_from->drv->read(vmem_from, to, physaddr.p, size);
        return to;
    }

    // RAM -> RAM (no VMEM mapping found)
    return memcpy(to, from, size);
}
