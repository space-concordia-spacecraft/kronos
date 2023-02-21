/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/hexdump.h>
#include <gs/util/string.h>
#include <gs/util/stdio.h>
#include <gs/util/log.h>

static void print_disp_addr02(FILE * out, uintptr_t disp_addr)
{
    fprintf(out, "0x%02"PRIx32" : ", (uint32_t) disp_addr);
}
static void print_disp_addr04(FILE * out, uintptr_t disp_addr)
{
    fprintf(out, "0x%04"PRIx32" : ", (uint32_t) disp_addr);
}
static void print_disp_addrxx(FILE * out, uintptr_t disp_addr)
{
#if defined(PRIx64)
    fprintf(out, "0x%08"PRIx64" : ", (uint64_t) disp_addr);
#else
    fprintf(out, "0x%08"PRIx32" : ", (uint32_t) disp_addr);
#endif
}

void gs_hexdump_to_stream(const void * in_src, size_t len, const void * in_disp_addr, FILE* out)
{
    volatile const uint8_t * src = in_src;
    uintptr_t disp_addr = GS_TYPES_PTR2UINT(in_disp_addr);
    const uintptr_t end_disp_addr = disp_addr + len;

    // work-rounds for not printing NIL (if address 0), align addresses, not supporting %zx, %*x or %08p on all platforms
    void (*print_addr)(FILE * out, uintptr_t disp_addr);
    if (end_disp_addr <= 0xff) {
        print_addr = print_disp_addr02;
    } else if (end_disp_addr <= 0xffff) {
        print_addr = print_disp_addr04;
    } else {
        print_addr = print_disp_addrxx;
    }

    print_addr(out, disp_addr);

    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    char text[17];
    for(; i < len; ++i) {
        const uint8_t ch = *src++;
        ++disp_addr;

        // hex
        fprintf(out, "%02x ", ch);
        ++j;
        if (j == 8) {
            fprintf(out, " ");
        }

        // printable
        if ((ch < 32) || (ch > 126)) {
            text[k] = '.';
        } else {
            text[k] = (char) ch;
        }
        ++k;
        text[k] = 0;

        // newline?
        if(j >= 16) {
            fprintf(out, "|%-16.16s|\r\n", text);
            j = 0;
            k = 0;
            text[k] = 0;

            if (i < (len - 1)) {
                print_addr(out, disp_addr);
            }
        }
    }
    if ((i == 0) || (i % 16)) {
        if (j) {
            // something was printed - show textual
            for (; j < 16; j++) {
                if (j == 7) {
                    fprintf(out, " ");
                }
                fprintf(out, "   ");
            }
            fprintf(out, "|%-16.16s|", text);
        }
        fprintf(out, "\r\n");
    }
}
