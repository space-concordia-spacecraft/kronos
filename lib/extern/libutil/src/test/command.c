/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/test/command.h>
#include <gs/util/test/cmocka.h>
#include <gs/util/error.h>
#include <gs/util/time.h>
#include <gs/util/hexdump.h>
#include <fnmatch.h>
#include <stdio.h>

void cm_print_error(const char * const format, ...) CMOCKA_PRINTF_ATTRIBUTE(1, 2);

struct results {
    char group[32];
    char key[32];
    char value[128];
};

static FILE* g_output_file = 0;
static char g_stdout_buf[10000];
static char g_stdin_buf[1000];
static uint16_t g_stdin_idx;

static struct results g_results[100];
static uint32_t g_results_count = 0;


static gs_error_t test_set_result(gs_command_context_t *ctx, const char *group, const char *key, const char *value)
{
    if (g_results_count >= GS_ARRAY_SIZE(g_results)) {
        return GS_ERROR_ALLOC;
    }

    if (group) {
        strncpy(g_results[g_results_count].group, group, sizeof(g_results[0].group));
    }
    if (key) {
        strncpy(g_results[g_results_count].key, key, sizeof(g_results[0].key));
    }
    if (value) {
        strncpy(g_results[g_results_count].value, value, sizeof(g_results[0].value));
    }
    g_results_count++;
    return GS_OK;
}

static gs_error_t test_flush(gs_command_context_t *ctx)
{
    fflush(ctx->out);
    return GS_OK;
}

static gs_error_t test_wait_for_key(gs_command_context_t *ctx, int *ch, int timeout_ms)
{
    if (g_stdin_buf[g_stdin_idx] == 0) {
        gs_time_sleep_ms(timeout_ms);
        return GS_ERROR_TIMEOUT;
    }

    *ch = g_stdin_buf[g_stdin_idx];
    g_stdin_idx++;
    return GS_OK;
}

static gs_command_io_functions_t g_test_io_functions = {
    .set_result = test_set_result,
    .flush = test_flush,
    .wait_for_key = test_wait_for_key,
};


static gs_error_t prepare_validation(const char *input)
{
    g_results_count = 0;
    memset(g_stdout_buf, 0, sizeof(g_stdout_buf));
    memset(g_stdin_buf, 0, sizeof(g_stdin_buf));
    g_stdin_idx = 0;

    if (input != NULL) {
        memcpy(g_stdin_buf, input, strlen(input));
    }

    g_output_file = fmemopen(g_stdout_buf, sizeof(g_stdout_buf) - 1, "w");
    if (!g_output_file) {
        return GS_ERROR_ALLOC;
    }

    return GS_OK;
}

static bool match(const char *first, const char * second)
{
    return (fnmatch(first, second, 0) == 0);
}

static gs_error_t do_validation(const char *expected)
{
    fclose(g_output_file);

    if (expected == NULL)
        return GS_OK;

    if (strcmp(expected, g_stdout_buf) == 0) {
        // exact match - even if they contain wildcard characters
        return GS_OK;
    }

    if (!match(expected, g_stdout_buf)) {
        return GS_ERROR_DATA;
    }

    return GS_OK;
}


void _gs_assert_command_validate(const char *cmd, gs_error_t ret, gs_error_t cmd_ret, const char *std_in, const char *std_out,
                                 const char * const file, const int line)
{
    if (prepare_validation(std_in) != GS_OK)
    {
        cm_print_error("Validation function failed to allocate it's resources\n");
        _fail(file, line);
    }

    gs_error_t _ret;
    gs_error_t _command_ret = GS_OK;
    _ret = gs_command_execute(cmd, &_command_ret, g_output_file, &g_test_io_functions, NULL);

    if (_ret != ret) {
        cm_print_error("Return: %d(%s) != %d(%s)\n", _ret, gs_error_string(_ret), ret, gs_error_string(ret));
        _fail(file, line);
    }

    if (_ret == GS_OK) /* Only check CMD return if command execution succeeded */
    {
        if (_command_ret != cmd_ret) {
            cm_print_error("Command return: %d(%s) != %d(%s)\n", _command_ret, gs_error_string(_command_ret),
                           cmd_ret, gs_error_string(cmd_ret));
            _fail(file, line);
        }
    }

    if (do_validation(std_out) != GS_OK)
    {
        //cm_print_error("Stdout <actual> != <expected>: \n%s\n!=\n%s\n", g_stdout_buf, std_out);
        printf("Stdout <actual> != <expected>: \n%s\n!=\n%s\n", g_stdout_buf, std_out);
        //gs_hexdump_addr(g_stdout_buf, strlen(g_stdout_buf), 0);
        //gs_hexdump_addr(std_out, strlen(std_out), 0);
        _fail(file, line);
    }

    return;
}

void _gs_assert_command_validate_last_result(unsigned int no, const char *group, const char *key, const char *value,
                                             const char * const file, const int line)
{
    if (no >= g_results_count) {
        cm_print_error("Result no: %d not available. Only %d results returned from command\n", no, g_results_count);
        _fail(file, line);
    }

    if (group) {
        if (!match(group, g_results[no].group)) {
            cm_print_error("group: %s != %s\n", group, g_results[no].group);
            _fail(file, line);
        }
    }
    if (key) {
        if (!match(key, g_results[no].key)) {
            cm_print_error("key: %s != %s\n", key, g_results[no].key);
            _fail(file, line);
        }
    }
    if (value) {
        if (!match(value, g_results[no].value)) {
            cm_print_error("value: %s != %s\n", value, g_results[no].value);
            _fail(file, line);
        }
    }

    return;
}

void _gs_assert_command_validate_results(unsigned int no, const char * const file, const int line)
{
    if (no != g_results_count) {
        cm_print_error("Unexpected number of results - expected: %u != actual %u\n", no, g_results_count);
        _fail(file, line);
    }
}
