/* Copyright (c) 2013-2019 GomSpace A/S. All rights reserved. */

#include <gs/util/vmem_test.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <gs/util/vmem.h>
#include <gs/util/time.h>
#include <gs/util/clock.h>

gs_error_t gs_vmem_test_data_bus_test(gs_address_t virtual_start, FILE * out)
{
    /* Walkings ones test */
    unsigned int buffer;
    unsigned int data;

    for (data = 1; data != 0; data <<= 1) {
        gs_vmem_cpy(virtual_start.p, &data, sizeof(data));
        gs_vmem_cpy(&buffer, virtual_start.p, sizeof(buffer));
        if (buffer != data) {
            fprintf(out, "data (%#x)  !=  read back (%#x) (addr: 0x%p)\n\r", data, buffer,virtual_start.p);
            return GS_ERROR_DATA;
        }
    }
    return GS_OK;
}

gs_error_t gs_vmem_test_addr_bus_test(gs_address_t virtual_start, size_t mem_size, FILE * out)
{   
    unsigned int *virtual_start_addr = virtual_start.p;
    const unsigned int data = (unsigned int)0x55555555;
    const unsigned int data_inverted = (unsigned int)0xAAAAAAAA;
    unsigned int address_mask = 1;
    
    mem_size -= 1;
    mem_size /= sizeof(data);
   
    while(mem_size > 1) {
        mem_size >>= 1;
        address_mask <<= 1;
        address_mask += 1;
    }
    long unsigned int offset = 0;

    /* Set data at every power of 2 offset */
    do {
        gs_vmem_cpy(&virtual_start_addr[offset], &data, sizeof(data));
        if (offset == 0) {
            offset = 1;
        } else {
            offset <<= 1;
        }
    } while (offset & address_mask);

    /* Check data is actually set */
    offset = 0;
    unsigned int buffer;
    do {
        gs_vmem_cpy(&buffer, &virtual_start_addr[offset], sizeof(buffer));
        if (buffer != data) {
            fprintf(out, "Could not set data at addr %p : *addr (%#x) !=  data (%#x)\n\r",
                    &virtual_start_addr[offset], buffer, data);
            return GS_ERROR_DATA;
        }
        if (offset == 0) {
            offset = 1;
        } else {
            offset <<= 1;
        }
    } while (offset & address_mask);

    /* For every power of two added to start address */
    long unsigned int test_offset = 0;
    do {
        /* Set every address of a power of two to inverted data*/
        gs_vmem_cpy(&virtual_start_addr[test_offset], &data_inverted, sizeof(data_inverted));

        /* Check that at every power of two we still have data.
         * If any of the address lines are shorted, this should catch it */
        offset = 0;
        do {
            gs_vmem_cpy(&buffer, &virtual_start_addr[offset], sizeof(buffer));
            if ((buffer != data) && (offset != test_offset)) {
                fprintf(out, "at addr %p :  *addr (%#x) !=  data (%#x), test_addr %p\n\r",
                        &virtual_start_addr[offset], buffer, data, &virtual_start_addr[test_offset]);
                return GS_ERROR_DATA;
            }
            if (offset == 0) {
                offset = 1;
            } else {
                offset <<= 1;
            }
        } while (offset & address_mask);

        /* Check that test address was actually set to inverted data */
        gs_vmem_cpy(&buffer, &virtual_start_addr[test_offset], sizeof(buffer));
        if (buffer != data_inverted) {
            fprintf(out, "Could not set inverted data at addr %p : *addr (%#x) !=  data (%#x)\n\r",
                    &virtual_start_addr[test_offset], buffer, data);
            return GS_ERROR_DATA;
        }

        /* Set back to data */
        gs_vmem_cpy(&virtual_start_addr[test_offset], &data, sizeof(data));

        if (test_offset == 0) {
            test_offset = 1;
        } else {
            test_offset <<= 1;
        }
    } while (test_offset & address_mask);

    return GS_OK;
}

gs_error_t gs_vmem_test_cell_test(gs_address_t virtual_start, size_t mem_size, FILE * out)
{
    unsigned int *virtual_start_addr = virtual_start.p;
    long unsigned int offset;
    unsigned int data;
    unsigned int data_inverted;

     for(data = 1, offset = 0; (offset < (mem_size / sizeof(data))); data++, offset++) {
        gs_vmem_cpy(&virtual_start_addr[offset], &data, sizeof(data));
    }

    unsigned int buffer;
    for(data = 1, offset = 0; (offset < (mem_size / sizeof(data))); data++, offset++) {
        gs_vmem_cpy(&buffer, &virtual_start_addr[offset], sizeof(buffer));
        if (buffer != data) {
            fprintf(out, "Could not set data at addr %p : *addr (%#x) !=  data (%#x)\n\r",
                    &virtual_start_addr[offset], buffer, data);
            return GS_ERROR_DATA;
        }
    }

    for(data = 1, offset = 0; (offset < (mem_size / sizeof(data))); data++, offset++) {
        data_inverted = ~data;
        gs_vmem_cpy(&virtual_start_addr[offset], &data_inverted, sizeof(data_inverted));
    }

    for(data = 1, offset = 0; (offset < (mem_size / sizeof(data))); data++, offset++) {
        gs_vmem_cpy(&buffer, &virtual_start_addr[offset], sizeof(buffer));
        data_inverted = ~data;
        if (buffer != data_inverted) {
            fprintf(out, "Could not set data_inverted at addr %p : *addr (%#x) !=  data (%#x)\n\r",
                    &virtual_start_addr[offset], buffer, data_inverted);
            return GS_ERROR_DATA;
        }
    }

    return GS_OK;
}

gs_error_t gs_vmem_test_speed_test(gs_address_t virtual_start, size_t mem_size, FILE * out)
{
    /* Build block buffer */
    uint32_t partition_size = mem_size;
    
    uint32_t block_size = 2048 > mem_size ? mem_size : 2048;
    block_size -= 3; // Ensure that it is not aligned on any 2^n boundary
    unsigned char data[block_size];
    for (unsigned int i=0; i<block_size; i++) {
        data[i] = (i & 0xFF);
    }

    /* Write the entire VMEM partition with a 256 byte repeating pattern */
    uint64_t wt_sum = 0;
    for (uint32_t b=0; b<(partition_size/block_size); b++)
    {
        uint64_t ts = gs_clock_get_nsec();
        gs_vmem_cpy(virtual_start.p + b*block_size, data, block_size);
        uint64_t te = gs_clock_get_nsec();
        wt_sum += te - ts;
    }

    /* Read the entire VMEM partition and validate content */
    uint64_t rt_sum = 0;
    for (uint32_t b=0; b<(partition_size/block_size); b++)
    {
        uint64_t ts = gs_clock_get_nsec();
        gs_vmem_cpy(data, virtual_start.p + b*block_size, block_size);
        uint64_t te = gs_clock_get_nsec();
        rt_sum += (te - ts);

        for (unsigned int i=0; i<block_size; i++) {
            if (data[i] != (i & 0xFF)) {
                fprintf(out, "ERROR: Memory validation failed at address <0x%p> (0x%p)\r\n",
                        (virtual_start.p + b*block_size + i), (virtual_start.p + b*block_size + i));
                return GS_ERROR_DATA;
            }
        }
    }

    double bw_w = (partition_size/block_size)*block_size*8/(wt_sum / 1000.0);
    double bw_r = (partition_size/block_size)*block_size*8/(rt_sum / 1000.0);
    fprintf(out, "Write time [ms]: %"PRIu64"\r\n", wt_sum / 1000000);
    fprintf(out, "Read time [ms]: %"PRIu64"\r\n", rt_sum / 1000000);
    fprintf(out, "Write bw [Mbps]: %2.2f\r\n", bw_w);
    fprintf(out, "Read bw [Mbps]: %2.2f\r\n", bw_r);

    return GS_OK;
}

gs_error_t gs_vmem_test_lock_test(gs_address_t virtual_start, size_t mem_size, const char * vmem_partition_name, FILE * out)
{
    /* Build block buffer */
    uint32_t block_size = 2048 > mem_size ? mem_size : 2048;
    block_size -= 3; // Ensure that it is not aligned on any 2^n boundary
    unsigned char test_data[block_size];
    unsigned char reference_data[block_size];

    gs_vmem_lock_by_name(vmem_partition_name, true);

    /* Copy reference data */
    gs_vmem_cpy(reference_data, virtual_start.p, block_size);

    /* Modify reference data */
    for (unsigned int i=block_size; i<block_size; i--) {
        test_data[i] = (reference_data[i] & 0xA5);
    }

    /* Attempt write of test data, this should not be written to memory as lock is enabled */
    gs_vmem_cpy(virtual_start.p, test_data, block_size);

    /* Read back - should be identical to reference data if locking works */
    gs_vmem_cpy(test_data, virtual_start.p, block_size);

    gs_vmem_lock_by_name(vmem_partition_name, false);
    
    /* Verify that the block has not been changed by the write. */
    for (uint32_t i=0; i<block_size; i++) {
        if (test_data[i] != reference_data[i]) {
            fprintf(out, "ERROR: Memory lock failed at address <0x%p> (0x%p)\r\n",
                    (virtual_start.p + i), (virtual_start.p + i));
            return GS_ERROR_DATA;
        }
    }
    return GS_OK;
}
