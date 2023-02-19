/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/fletcher.h>
#include <gs/util/string.h>
#include <gs/util/pgm.h>

uint16_t gs_fletcher16_memcpy(const void * data_in, size_t count, void * (*memcpyfcn)(void *, const void *, size_t))
{
    if (memcpyfcn == NULL) {
        memcpyfcn = &memcpy;
    }

    uint16_t sum1 = 0;
    uint16_t sum2 = 0;

    if (data_in && count) {
        const uint8_t * data = data_in;
        for (unsigned int idx = 0; idx < count; ++idx) {
            uint8_t byte;
            (*memcpyfcn)(&byte, &data[idx], 1);
            sum1 = (uint16_t)((sum1 + byte) % 255);
            sum2 = (uint16_t)((sum2 + sum1) % 255);
        }
    }
    return (uint16_t)((sum2 << 8) | sum1);
}

uint16_t gs_fletcher16_P(const void * data_in, size_t count)
{
    uint16_t sum1 = 0;
    uint16_t sum2 = 0;

    if (data_in && count) {
        const uint8_t * data = data_in;
        for (unsigned int idx = 0; idx < count; ++idx) {
            sum1 = (uint16_t)((sum1 + GS_PGM_UINT8_BY_PTR(data++)) % 255);
            sum2 = (uint16_t)((sum2 + sum1) % 255);
        }
    }
    return (uint16_t)((sum2 << 8) | sum1);
}

uint16_t gs_fletcher16(const void * data_in, size_t size)
{
    uint16_t sum1 = 0;
    uint16_t sum2 = 0;

    if (data_in && size) {
        const uint8_t * data = data_in;
        for (unsigned int idx = 0; idx < size; ++idx) {
            sum1 = (uint16_t)((sum1 + (*data++)) % 255);
            sum2 = (uint16_t)((sum2 + sum1) % 255);
        }
    }
    return (uint16_t)((sum2 << 8) | sum1);
}

void gs_fletcher16_init(gs_fletcher16_t * f16)
{
    f16->sum1 = f16->sum2 = 0;
}

void gs_fletcher16_update(gs_fletcher16_t * f16, const void * data_in, size_t size)
{
    if (f16 && data_in && size) {
        const uint8_t * data = data_in;
        for (unsigned int idx = 0; idx < size; ++idx) {
            f16->sum1 = (uint16_t)((f16->sum1 + (*data++)) % 255);
            f16->sum2 = (uint16_t)((f16->sum2 + f16->sum1) % 255);
        }
    }
}

uint16_t gs_fletcher16_finalize(gs_fletcher16_t * f16)
{
    return (uint16_t)((f16->sum2 << 8) | f16->sum1);
}
