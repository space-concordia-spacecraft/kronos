#ifndef GS_UTIL_TEST_CMOCKA_H
#define GS_UTIL_TEST_CMOCKA_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Cmocka extensions.

   Official site for cmocka https://cmocka.org.
*/

#include <gs/util/string.h>

// cmocka
#include <stdio.h>
#include <setjmp.h>
#include <cmocka.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !(__DOXYGEN__)
// internal helpers - use macros
void _gs_assert_int_equal(const intptr_t a, const intptr_t b, bool equal, const char * const file, const int line);
void _gs_assert_uint_equal(const uintptr_t a, const uintptr_t b, bool equal, bool hex, const char * const file, const int line);
void _gs_assert_error_equal(const int a, const int b, bool equal, const char * const file, const int line);
void _gs_assert_float_equal(const float a, const float b, const float diff, bool equal, const char * const file, const int line);
void _gs_assert_double_equal(const double a, const double b, const double diff, bool equal, const char * const file, const int line);
#endif
    
/**
   Assert int (print value as signed).
*/
#define GS_ASSERT_INT_EQUAL(a,b)             _gs_assert_int_equal((intptr_t) (a), (intptr_t) (b), true, __FILE__, __LINE__)
/**
   Assert unsigned int (print value as unsigned).
*/
#define GS_ASSERT_UINT_EQUAL(a,b)            _gs_assert_uint_equal((uintptr_t) (a), (uintptr_t) (b), true, false, __FILE__, __LINE__)
/**
   Assert int (print value as hex).
*/
#define GS_ASSERT_XINT_EQUAL(a,b)            _gs_assert_uint_equal((uintptr_t) (a), (uintptr_t) (b), true, true, __FILE__, __LINE__)
/**
   Assert #gs_error_t (print value and error text).
*/
#define GS_ASSERT_ERROR_EQUAL(a,b)           _gs_assert_error_equal(a, b, true, __FILE__, __LINE__)
/**
   Assert #GS_OK (print value and error text).
*/
#define GS_ASSERT_ERROR_OK(a)                _gs_assert_error_equal(a, GS_OK, true, __FILE__, __LINE__)
/**
   Assert float (print value as signed).
*/
#define GS_ASSERT_FLOAT_EQUAL(a,b,diff)      _gs_assert_float_equal(a, b, diff, true, __FILE__, __LINE__)
/**
   Assert double (print value as signed).
*/
#define GS_ASSERT_DOUBLE_EQUAL(a,b,diff)     _gs_assert_double_equal(a, b, diff, true, __FILE__, __LINE__)

/**
   Assert int (print value as signed).
*/
#define GS_ASSERT_INT_NOT_EQUAL(a,b)         _gs_assert_int_equal((intptr_t) (a), (intptr_t) (b), false, __FILE__, __LINE__)
/**
   Assert unsigned int (print value as unsigned).
*/
#define GS_ASSERT_UINT_NOT_EQUAL(a,b)        _gs_assert_uint_equal((uintptr_t) (a), (uintptr_t) (b), false, false, __FILE__, __LINE__)
/**
   Assert int (print value as hex).
*/
#define GS_ASSERT_XINT_NOT_EQUAL(a,b)        _gs_assert_uint_equal((uintptr_t) (a), (uintptr_t) (b), false, true, __FILE__, __LINE__)
/**
   Assert #GS_OK (print value and error text).
*/
#define GS_ASSERT_ERROR_NOT_EQUAL(a,b)       _gs_assert_error_equal(a, b, false, __FILE__, __LINE__)

/**
   Code reference.
*/
#define GS_REF()                                 __FILE__,__LINE__
/**
   Assert int with code reference (print value as signed).
*/
#define GS_ASSERT_INT_EQUAL_REF(a,b,file,line)   _gs_assert_int_equal((intptr_t) (a), (intptr_t) (b), true, file, file, line)
/**
   Assert unsigned int with code reference (print value as unsigned).
*/
#define GS_ASSERT_UINT_EQUAL_REF(a,b,file,line)  _gs_assert_uint_equal((uintptr_t) (a), (uintptr_t) (b), true, false, file, line)
/**
   Assert int with code reference (print value as hex).
*/
#define GS_ASSERT_XINT_EQUAL_REF(a,b,file,line)  _gs_assert_uint_equal((uintptr_t) (a), (uintptr_t) (b), true, true, file, line)
/**
   Assert #gs_error_t with code reference (print value and error text).
*/
#define GS_ASSERT_ERROR_EQUAL_REF(a,b,file,line) _gs_assert_error_equal(a, b, true, file, line)
/**
   Assert #GS_OK with code reference (print value and error text).
*/
#define GS_ASSERT_ERROR_OK_REF(a,file,line)      _gs_assert_error_equal(a, GS_OK, true, file, line)

/**
   Run \a cmocka test group.

   @param[in] name name of test. If name is \a tests and GS_TEST_NAME is set, GS_TEST_NAME will be used instead.
   @param[in] tests array of tests.
   @param[in] num_tests number of tests.
   @param[in] setup setup function, can be NULL.
   @param[in] teardown teardown function, can be NULL.
   @return 0 on success.
*/
static inline int gs_cmocka_run_group_tests(const char *name,
                                            const struct CMUnitTest * const tests,
                                            const size_t num_tests,
                                            CMFixtureFunction setup,
                                            CMFixtureFunction teardown)
{
#ifdef GS_TEST_NAME // set by buildtools::gs_test_cmocka.py
    if (strcasecmp(name, "tests") == 0) {
        name = GS_DEF2STRING(GS_TEST_NAME);
    }
#endif
    return _cmocka_run_group_tests(name, tests, num_tests, setup, teardown);
}

#ifdef GS_TEST_NAME
// hi-jack cmocka's macro
#undef cmocka_run_group_tests
#define cmocka_run_group_tests(tests, setup, teardown) gs_cmocka_run_group_tests(GS_DEF2STRING(tests), tests, GS_ARRAY_SIZE(tests), setup, teardown)
#endif

#ifdef __cplusplus
}
#endif
#endif
