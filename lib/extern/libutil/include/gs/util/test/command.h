#ifndef GS_UTIL_TEST_COMMAND_H
#define GS_UTIL_TEST_COMMAND_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Command Test framework.

   Provides a simple way of unit-testing/validating commands.
*/

#include <gs/util/gosh/command.h>
#include <gs/util/test/cmocka.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Validate command execution.

   Runs a commands and validates the output/results agains the inputs.
   Asserts if the results does not match.

   @param[in] cmd command (including arguments) to execute.
   @param[in] ret expected return code from the command execution framework.
   @param[in] cmd_ret expected return code from the commands handler. This is only validated if (ret = GS_OK).
   @param[in] std_in string with expected command input.
   @param[in] std_out string with expected command output. Wildcards (*\/?) are supported.
   @param[in] file string with file name.
   @param[in] line string with line no.
*/
void _gs_assert_command_validate(const char *cmd, gs_error_t ret, gs_error_t cmd_ret, const char *std_in, const char *std_out, const char * const file, const int line);

/**
   Validate command results returned from last command execution.
   Asserts if the results does not match.

   @param[in] no the result no to verify.
   @param[in] group string with expected group id. Wildcards (*\/?) are supported.
   @param[in] key string with expected key. Wildcards (*\/?) are supported.
   @param[in] value string with expected value. Wildcards (*\/?) are supported.
   @param[in] file string with file name.
   @param[in] line string with line no.
*/
void _gs_assert_command_validate_last_result(unsigned int no, const char *group, const char *key, const char *value, const char * const file, const int line);

/**
   Validate number of command results.
   Asserts if the results does not match.

   @param[in] no number of expected results
   @param[in] file string with file name.
   @param[in] line string with line no.
*/
void _gs_assert_command_validate_results(unsigned int no, const char * const file, const int line);
    
/**
   Validate command execution.

   Runs a commands and validates the output/results agains the inputs.
   Asserts if the results does not match.

   @param[in] cmd command (including arguments) to execute.
   @param[in] ret expected return code from the command execution framework.
   @param[in] cmd_ret expected return code from the commands handler. This is only validated if (ret = GS_OK).
   @param[in] std_in string with expected command input.
   @param[in] std_out string with expected command output. Wildcards (*\/?) are supported.
*/
#define GS_ASSERT_COMMAND(cmd,ret,cmd_ret,std_in,std_out)   _gs_assert_command_validate(cmd,ret,cmd_ret,std_in,std_out, __FILE__, __LINE__);

/**
   Validate command results returned from last command execution.
   Asserts if the results does not match.

   @param[in] no the result no to verify.
   @param[in] group string with expected group id. Wildcards (*\/?) are supported.
   @param[in] key string with expected key. Wildcards (*\/?) are supported.
   @param[in] value string with expected value. Wildcards (*\/?) are supported.
*/
#define GS_ASSERT_COMMAND_RESULT(no,group,key,value)        _gs_assert_command_validate_last_result(no,group,key,value, __FILE__, __LINE__);

/**
   Validate command results.
   Asserts if the results does not match.

   @param[in] no number of expected results
*/
#define GS_ASSERT_COMMAND_RESULTS(no)                       _gs_assert_command_validate_results(no, __FILE__, __LINE__);

#ifdef __cplusplus
}
#endif
#endif
