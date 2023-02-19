/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/test/cmocka.h>
#include <gs/util/log.h>
#include <math.h>

void cm_print_error(const char * const format, ...) CMOCKA_PRINTF_ATTRIBUTE(1, 2);

#define EQUAL_TO_STRING(equal)  (equal ? "!=" : "==")

void _gs_assert_int_equal(const intptr_t left, const intptr_t right, bool equal, const char * const file, const int line)
{
    const bool cmp_equal = left == right;
    if (cmp_equal != equal) {
        cm_print_error("%" PRIdPTR " %s %" PRIdPTR "\n", left, EQUAL_TO_STRING(equal), right);

        _fail(file, line);
    }
}

void _gs_assert_uint_equal(const uintptr_t left, const uintptr_t right, bool equal, bool hex, const char * const file, const int line)
{
    const bool cmp_equal = left == right;
    if (cmp_equal != equal) {
        if(hex) {
            cm_print_error("0x%" PRIxPTR " %s 0x%" PRIxPTR "\n", left, EQUAL_TO_STRING(equal), right);
        } else {
            cm_print_error("%" PRIuPTR " %s %" PRIuPTR "\n", left, EQUAL_TO_STRING(equal), right);
        }

        _fail(file, line);
    }
}

void _gs_assert_error_equal(const int left, const int right, bool equal, const char * const file, const int line)
{
    const bool cmp_equal = left == right;
    if (cmp_equal != equal) {
        cm_print_error("%d(%s) %s %d(%s)\n", left, gs_error_string(left), EQUAL_TO_STRING(equal), right, gs_error_string(right));

        _fail(file, line);
    }
}

void _gs_assert_float_equal(const float left, const float right, const float diff, bool equal, const char * const file, const int line)
{
    const bool cmp_equal = (fabsf(left - right) < diff);
    if (cmp_equal != equal) {
        cm_print_error("%e %s %e\n", left, EQUAL_TO_STRING(equal), right);

        _fail(file, line);
    }
}

void _gs_assert_double_equal(const double left, const double right, const double diff, bool equal, const char * const file, const int line)
{
    const bool cmp_equal = (fabsf(left - right) < diff);
    if (cmp_equal != equal) {
        cm_print_error("%e %s %e\n", left, EQUAL_TO_STRING(equal), right);

        _fail(file, line);
    }
}
