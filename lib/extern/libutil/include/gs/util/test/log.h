#ifndef GS_UTIL_TEST_LOG_H
#define GS_UTIL_TEST_LOG_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Log Test framework.

   Provides a simple way of veriyfing logs generated during unit-testing.
*/

#include <gs/util/log/log.h>
#include <gs/util/test/cmocka.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Assert log count - internal helper function.
*/
void gs_assert_log_count(int level, unsigned int count, const char * file, int line);

/**
   Assert log messag and count - internal helper function.
*/
void gs_assert_log(unsigned int stack_index, unsigned int count, gs_log_level_t level, const char * pattern, const char * file, int line);

/**
   Initialize framework, by installing a callback for \a print.
   @param[in] verbose of \a true, logs will be printed on stdout.
*/
void gs_test_log_init(bool verbose);
    
/**
   Clear log stats.
*/
void gs_test_log_clear(void);
    
/**
   Assert number of error logs.
*/
#define GS_ASSERT_LOG_ERROR(cnt)   gs_assert_log_count(LOG_ERROR, cnt, __FILE__, __LINE__);

/**
   Assert number of warning logs.
*/
#define GS_ASSERT_LOG_WARNING(cnt) gs_assert_log_count(LOG_WARNING, cnt, __FILE__, __LINE__);

/**
   Assert number of notice logs.
*/
#define GS_ASSERT_LOG_NOTICE(cnt)  gs_assert_log_count(LOG_NOTICE, cnt, __FILE__, __LINE__);

/**
   Assert number of info logs.
*/
#define GS_ASSERT_LOG_INFO(cnt)    gs_assert_log_count(LOG_INFO, cnt, __FILE__, __LINE__);

/**
   Assert number of debug logs.
*/
#define GS_ASSERT_LOG_DEBUG(cnt)   gs_assert_log_count(LOG_DEBUG, cnt, __FILE__, __LINE__);

/**
   Assert number of trace logs.
*/
#define GS_ASSERT_LOG_TRACE(cnt)   gs_assert_log_count(LOG_TRACE, cnt, __FILE__, __LINE__);

/**
   Assert number of all logs.
*/
#define GS_ASSERT_LOG_ALL(cnt)     gs_assert_log_count(-1, cnt, __FILE__, __LINE__);

/**
   Assert/find number of entries matching level and pattern.
*/
#define GS_ASSERT_LOG(count,level,pattern)       gs_assert_log(-1, count, level, pattern, __FILE__, __LINE__)

/**
   Assert log at stack index against matching level and pattern.
*/
#define GS_ASSERT_LOG_AT(stack_index,level,pattern) gs_assert_log(stack_index, 1, level, pattern, __FILE__, __LINE__)

#ifdef __cplusplus
}
#endif
#endif
