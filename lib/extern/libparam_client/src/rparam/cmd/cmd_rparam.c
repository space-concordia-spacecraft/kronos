/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#define GS_PARAM_INTERNAL_USE 1

#include <gs/param/internal/rparam.h>
#include <gs/param/internal/table.h>
#include <gs/param/internal/serialize.h>
#include <gs/param/internal/types.h>
#include "../query.h"
#include <gs/util/log.h>
#include <gs/util/gosh/command.h>
#include <gs/util/stdio.h>
#include <gs/util/unistd.h>
#include <gs/csp/csp.h>
#include <stdlib.h>

/** Remote param system setup */
static gs_param_table_instance_t rparam_tinst;
static gs_rparam_query_handle_t rquery = {.timeout_ms = 10000};
static char * rparam_wd = NULL;

#define CHECK_TABLE() \
    if (rparam_tinst.rows == NULL) {                                    \
        fprintf(ctx->out, "Run download or init to setup table\n");     \
        return GS_ERROR_NOT_FOUND;                                      \
    }

static int cmd_rparam_list(gs_command_context_t *ctx)
{
    CHECK_TABLE();
    return gs_param_list_to_stream(&rparam_tinst, true, 0, ctx->out);
}

static void make_filename(char * fname, size_t fname_size, bool add_filename)
{
    char cwd[GS_PATH_MAX + 1];
    const char * wd;
    if (gs_string_empty(rparam_wd) == false) {
        wd = rparam_wd;
    } else if (gs_getcwd(cwd, sizeof(cwd)) == GS_OK) {
        wd = cwd;
    } else {
        wd = NULL;
    }
    if (gs_string_empty(wd) == false) {
        if (add_filename) {
            snprintf(fname, fname_size, "%s/param-%d-%u.bin", wd, rquery.node, rquery.table_id);
        } else {
            snprintf(fname, fname_size, "%s", wd);
        }
    } else {
        fname[0] = 0;
    }
}

static int cmd_rparam_init_from_local_file(gs_command_context_t *ctx)
{
    gs_rparam_query_reset(&rquery);

    if (gs_string_to_uint8(ctx->argv[1], &rquery.node)) {
        return GS_ERROR_ARG;
    }
    if (gs_string_to_uint8(ctx->argv[2], &rquery.table_id)) {
        return GS_ERROR_ARG;
    }

    char fname[GS_PATH_MAX + 1];
    make_filename(fname, sizeof(fname), true);
    gs_error_t error = gs_rparam_load_table_spec(&rparam_tinst, fname, &rquery.checksum);
    if ((error != GS_OK) && (ctx->argc > 3)) {
        bool fallback_download;
        if (gs_string_to_bool(ctx->argv[3], &fallback_download)) {
            return GS_ERROR_ARG;
        }
        if (fallback_download) {
            error = gs_rparam_download_table_spec(&rparam_tinst, fname, rquery.node, rquery.table_id, rquery.timeout_ms, 
                                                  &rquery.checksum);
        }
    }
    return error;
}

static int cmd_rparam_init_from_remote_node(gs_command_context_t *ctx)
{
    gs_rparam_query_reset(&rquery);

    if (gs_string_to_uint8(ctx->argv[1], &rquery.node)) {
        return GS_ERROR_ARG;
    }
    if (gs_string_to_uint8(ctx->argv[2], &rquery.table_id)) {
        return GS_ERROR_ARG;
    }

    char fname[GS_PATH_MAX + 1];
    make_filename(fname, sizeof(fname), true);
    return gs_rparam_download_table_spec(&rparam_tinst, fname, rquery.node, rquery.table_id, rquery.timeout_ms, &rquery.checksum);
}

static int cmd_rparam_send(gs_command_context_t *ctx)
{
    CHECK_TABLE();

    gs_error_t error = gs_rparam_query_send(&rquery, &rparam_tinst);
    if (error == GS_OK) {
        if (rquery.action == RPARAM_GET) {
            const gs_param_table_row_t * last_print = NULL;
            for (unsigned int i = 0; i < rquery.length / 2; ++i) {
                const gs_param_table_row_t * row = gs_param_row_by_address(rquery.payload.addr[i], rparam_tinst.rows, rparam_tinst.row_count);
                if (row != last_print) { // work-around to avoid duplicate lines for elements within same array
                    gs_param_list_single_to_stream(&rparam_tinst, row, true, 0, ctx->out);
                    last_print = row;
                }
            }
        }
        gs_rparam_query_reset(&rquery);
    }
    return error;
}

static int cmd_rparam_get(gs_command_context_t *ctx)
{
    CHECK_TABLE();

    gs_rparam_query_set_quiet(&rquery, false);
    gs_error_t error = gs_rparam_query_get(&rquery, &rparam_tinst, ctx->argv[1]);
    if ((error == GS_OK) && rquery.auto_send) {
        error = cmd_rparam_send(ctx);
    }

    return error;
}

static int cmd_rparam_getall(gs_command_context_t *ctx)
{
    CHECK_TABLE();

    fprintf(ctx->out, "Downloading table content for table %i from server %u\n", rquery.table_id, rquery.node);
    gs_error_t error = gs_rparam_get_full_table(&rparam_tinst, rquery.node, rquery.table_id, rquery.checksum, rquery.timeout_ms);
    if (error == GS_OK) {
        gs_param_list_to_stream(&rparam_tinst, true, 0, ctx->out);
    }
    return error;
}

static int cmd_rparam_set(gs_command_context_t *ctx)
{
    CHECK_TABLE();

    gs_error_t error = gs_rparam_query_set(&rquery, &rparam_tinst, ctx->argv[1], &ctx->argv[2], ctx->argc - 2);
    if ((error == GS_OK) && rquery.auto_send) {
        error = cmd_rparam_send(ctx);
    }

    return error;
}

static int cmd_rparam_copy(gs_command_context_t *ctx)
{
    uint8_t from;
    if (gs_string_to_uint8(ctx->argv[1], &from)) {
        return GS_ERROR_ARG;
    }

    uint8_t to;
    if (gs_string_to_uint8(ctx->argv[2], &to)) {
        return GS_ERROR_ARG;
    }

    return gs_rparam_copy(rquery.node, rquery.timeout_ms, from, to);
}

static int cmd_rparam_load(gs_command_context_t *ctx)
{
    uint8_t table_id;
    if (gs_string_to_uint8(ctx->argv[2], &table_id)) {
        return GS_ERROR_ARG;
    }
    uint8_t file_id;
    if (gs_string_to_uint8(ctx->argv[1], &file_id)) {
        // This may be a store - no way of validating
        return gs_rparam_load_from_store(rquery.node, rquery.timeout_ms, table_id, ctx->argv[1], NULL);
    }

    return gs_rparam_load(rquery.node, rquery.timeout_ms, file_id, table_id);
}

static int cmd_rparam_save(gs_command_context_t *ctx)
{
    uint8_t table_id;
    if (gs_string_to_uint8(ctx->argv[1], &table_id)) {
        return GS_ERROR_ARG;
    }
    uint8_t file_id;
    if (gs_string_to_uint8(ctx->argv[2], &file_id)) {
        // This may be a store - no way of validating
        return gs_rparam_save_to_store(rquery.node, rquery.timeout_ms, table_id, ctx->argv[2], NULL);
    }

    return gs_rparam_save(rquery.node, rquery.timeout_ms, table_id, file_id);
}

static int cmd_rparam_reset(gs_command_context_t *ctx)
{
    gs_rparam_query_reset(&rquery);
    return GS_OK;
}

static int cmd_rparam_set_autosend(gs_command_context_t *ctx)
{
    if (ctx->argc > 1) {
        if (gs_string_to_bool(ctx->argv[1], &rquery.auto_send)) {
            return GS_ERROR_ARG;
        }
    }
    fprintf(ctx->out, "auto send: %d\r\n", rquery.auto_send);
    return GS_OK;
}

static int cmd_set_wd(gs_command_context_t *ctx)
{
    if (ctx->argc > 1) {
        if (strnlen(ctx->argv[1], GS_PATH_MAX) >= GS_PATH_MAX) {
            return GS_ERROR_ARG;
        }
        if (rparam_wd == NULL) {
            rparam_wd = malloc(GS_PATH_MAX + 1);
            if (rparam_wd == NULL) {
                return GS_ERROR_ALLOC;
            }
        }
        strcpy(rparam_wd, ctx->argv[1]);
    }
    char fname[GS_PATH_MAX + 1];
    make_filename(fname, sizeof(fname), false);
    fprintf(ctx->out, "working directory: %s\r\n", gs_string_empty(fname) ? "not set" : fname);
    return GS_OK;
}

static int cmd_rparam_set_timeout(gs_command_context_t *ctx)
{
    if (ctx->argc > 1) {
        if (gs_string_to_uint32(ctx->argv[1], &rquery.timeout_ms)) {
            return GS_ERROR_ARG;
        }
    }
    fprintf(ctx->out, "timeout: %"PRIu32" mS\r\n", rquery.timeout_ms);
    return GS_OK;
}

static int cmd_rparam_set_checksum(gs_command_context_t *ctx)
{
    if (ctx->argc > 1) {
        if (strcasecmp(ctx->argv[1], "magic") == 0) {
            rquery.checksum = GS_RPARAM_MAGIC_CHECKSUM;
        } else if (gs_string_to_uint16(ctx->argv[1], &rquery.checksum)) {
            return GS_ERROR_ARG;
        }
    }
    fprintf(ctx->out, "checksum: 0x%04x/%u (magic: 0x%04x)\r\n", rquery.checksum, rquery.checksum, GS_RPARAM_MAGIC_CHECKSUM);
    return GS_OK;
}

static const gs_command_t rparam_commands[] = {
    {
        .name = "init",
        .help = "Set server and load table specification from file",
        .usage = "<server> <table-id> [fallback download: 0|1]",
        .handler = cmd_rparam_init_from_local_file,
        .mandatory_args = 2,
        .optional_args = 1,
    },{
        .name = "download",
        .help = "Set server and download table specification from server",
        .usage = "<server> <table-id>",
        .handler = cmd_rparam_init_from_remote_node,
        .mandatory_args = 2,
    },{
        .name = "getall",
        .help = "Download all table values from server",
        .handler = cmd_rparam_getall,
        .mandatory_args = GS_COMMAND_NO_ARGS,
    },{
        .name = "list",
        .help = "List cached table values (use getall to download values)",
        .handler = cmd_rparam_list,
        .mandatory_args = GS_COMMAND_NO_ARGS,
    },{
        .name = "copy",
        .usage = "<from table-id> <to table-id>",
        .help = "Copy table on remote server (version <= 3 only)",
        .handler = cmd_rparam_copy,
        .mandatory_args = 2,
    },{
        .name = "load",
        .usage = "<file-id|store> <table-id>",
        .help = "Load table on remote server",
        .handler = cmd_rparam_load,
        .mandatory_args = 2,
    },{
        .name = "save",
        .usage = "<table-id> <file-id|store>",
        .help = "Save table on remote server",
        .handler = cmd_rparam_save,
        .mandatory_args = 2,
    },{
        .name = "get",
        .help = "query: add 'get' to current transactions",
        .usage = "<name>",
        .handler = cmd_rparam_get,
        .mandatory_args = 1,
    },{
        .name = "set",
        .help = "query: add 'set' to current transactions",
        .usage = "<parameter> <value> [value] ...",
        .handler = cmd_rparam_set,
        .mandatory_args = 2,
        .optional_args = 100,
    /* },{ */
    /*     .name = "print", */
    /*     .help = "query: print the current transactions", */
    /*     .handler = cmd_rparam_print, */
    /*     .mandatory_args = GS_COMMAND_NO_ARGS, */
    },{
        .name = "reset",
        .help = "query: reset current transactions",
        .handler = cmd_rparam_reset,
        .mandatory_args = GS_COMMAND_NO_ARGS,
    },{
        .name = "send",
        .help = "query: send current transactions",
        .handler = cmd_rparam_send,
        .mandatory_args = GS_COMMAND_NO_ARGS,
    },{
        .name = "autosend",
        .usage = "[bool]",
        .help = "query: Enable/disable autosend for set & get",
        .handler = cmd_rparam_set_autosend,
        .optional_args = 1,
    },{
        .name = "wd",
        .help = "Set working directory for init/download",
        .usage = "[path]",
        .handler = cmd_set_wd,
        .optional_args = 1,
    },{
        .name = "timeout",
        .help = "Set timeout",
        .usage = "[timeout mS]",
        .handler = cmd_rparam_set_timeout,
        .optional_args = 1,
    },{
        .name = "checksum",
        .help = "Set checksum",
        .usage = "[magic|<checksum>]",
        .handler = cmd_rparam_set_checksum,
        .optional_args = 1,
    }
};

static const gs_command_t GS_COMMAND_ROOT rparam_root_command[] = {
    {
        .name = "rparam",
        .help = "Parameter System client",
        .chain = GS_COMMAND_INIT_CHAIN(rparam_commands),
    },
};

gs_error_t gs_rparam_register_commands(void)
{
    return GS_COMMAND_REGISTER(rparam_root_command);
}
