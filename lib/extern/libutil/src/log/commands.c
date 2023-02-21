/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include "../cmd/log.h"
#include <gs/util/bytebuffer.h>
#include <gs/util/gosh/command.h>
#include <gs/util/log/log.h>
#include <gs/util/log/appender/appender.h>
#include "local.h"
#include <stdlib.h>


// iterator context
typedef struct {
    gs_command_context_t * ctx;
    gs_log_group_t * first;
    gs_log_appender_t * first_appender;
    bool completer;
    bool detailed;
    gs_error_t error;
} iter_group_t;

#define FORMAT_BUF_SIZE 10
static const char * format_mask(uint8_t mask, char * buf)
{
    snprintf(buf, FORMAT_BUF_SIZE, "%c%c%c%c%c%c",
             (mask & LOG_ERROR_MASK)    ? 'E' : '.',
             (mask & LOG_WARNING_MASK)  ? 'W' : '.',
             (mask & LOG_NOTICE_MASK)   ? 'N' : '.',
             (mask & LOG_INFO_MASK)     ? 'I' : '.',
             (mask & LOG_DEBUG_MASK)    ? 'D' : '.',
             (mask & LOG_TRACE_MASK)    ? 'T' : '.');
    return buf;
}

static bool iter_print_group_appenders(void * ctx_in, gs_log_appender_t * appender)
{
    gs_bytebuffer_t *bb = ctx_in;
    gs_bytebuffer_printf(bb, "%s,", appender->name);
    return true;
}

static bool iter_print_group(void * ctx_in, gs_log_group_t * group)
{
    iter_group_t * ctx = ctx_in;
    char level_mask[FORMAT_BUF_SIZE];
    if (!ctx->completer) {
        char appender_str[128] = "\0";
        gs_bytebuffer_t bb;
        gs_bytebuffer_init(&bb, appender_str, sizeof(appender_str));
        gs_log_group_appender_iterate(group, &bb, iter_print_group_appenders);
        if (ctx->detailed) {
            gs_command_set_output_printf(ctx->ctx, group->name, "category", "0x%08x", group->category);
            gs_command_set_output_printf(ctx->ctx, group->name, "mask", "%-6s (0x%02x)", format_mask(group->mask, level_mask), group->mask);
            gs_command_set_output_printf(ctx->ctx, group->name, "appenders", appender_str);
        } else {
            gs_command_set_output_printf(ctx->ctx, NULL, NULL, "%-15s %-6s  %s", group->name, format_mask(group->mask, level_mask), appender_str);
        }
    } else {
        fprintf(ctx->ctx->out, "  %-15s %-6s\r\n",
                group->name,
                format_mask(group->mask, level_mask));
    }
    return true;
}

int gs_log_cmd_log_group_list_handler(gs_command_context_t * ctx)
{
    iter_group_t iter = {.ctx = ctx, .completer = false};

    if (ctx->argc > 1) {
        iter.detailed = true;
        gs_log_group_iterate(ctx->argv[1], &iter, iter_print_group);
    } else {
        fprintf(ctx->out, "Group           Mask    Appenders\r\n");
        gs_log_group_iterate("*", &iter, iter_print_group);
    }
    return GS_OK;
}

static bool iter_print_appender(void * ctx_in, gs_log_appender_t * appender)
{
    iter_group_t * ctx = ctx_in;
    char level_mask[FORMAT_BUF_SIZE];
    if (!ctx->completer) {
        if (ctx->detailed) {
            gs_command_set_output_printf(ctx->ctx, appender->name, "mask", "%-6s (0x%02x)", format_mask(appender->mask, level_mask), appender->mask);

            if (appender->drv->info) {
                char info_str[100];
                appender->drv->info(appender, info_str, sizeof(info_str));
                gs_command_set_output(ctx->ctx, appender->name, "info", info_str);
            }
        } else {
            gs_command_set_output_printf(ctx->ctx, NULL, NULL, "%-15s %-6s", appender->name, format_mask(appender->mask, level_mask));
        }
    } else {
        fprintf(ctx->ctx->out, "  %-15s %-6s\r\n",
                appender->name,
                format_mask(appender->mask, level_mask));
    }
    return true;
}

int gs_log_cmd_log_appender_list_handler(gs_command_context_t * ctx)
{
    iter_group_t iter = {.ctx = ctx, .completer = false};

    if (ctx->argc > 1) {
        iter.detailed = true;
        gs_log_appender_iterate(ctx->argv[1], &iter, iter_print_appender);
    } else {
        fprintf(ctx->out, "Appender        Mask\r\n");
        gs_log_appender_iterate("*", &iter, iter_print_appender);
    }
    return GS_OK;
}

typedef gs_error_t (*log_get_mask_t)(const char *name, uint8_t* mask);
typedef gs_error_t (*log_set_mask_t)(const char *name, uint8_t mask);

static int cmd_log_mask_handler(gs_command_context_t * ctx, log_get_mask_t get_mask, log_set_mask_t set_mask)
{
    /* strtok writes to the string, so we need to duplicate it to avoid writing to read-only memory */
    char strbuf[100];
    GS_STRNCPY(strbuf, ctx->argv[1]);

    char * saveptr = NULL;
    char * token = strtok_r(strbuf, ",", &saveptr);
    gs_error_t error = GS_OK;
    while (token && (error == GS_OK)) {

        uint8_t old_mask = 0;
        if (gs_log_is_group_all(token) == false) {
            error = get_mask(token, &old_mask);
        }
        if (error == GS_OK) {
            uint8_t new_mask = 0;
            error = gs_log_string_to_mask(ctx->argv[2], old_mask, &new_mask);
            if (error == GS_OK) {
                error = set_mask(token, new_mask);
            }
        }

        token = strtok_r(NULL, ",", &saveptr);
    }

    return error;
}

int gs_log_cmd_log_group_mask_handler(gs_command_context_t * ctx)
{
    return cmd_log_mask_handler(ctx, gs_log_group_get_level_mask, gs_log_group_set_level_mask);
}

int gs_log_cmd_debug_handler(gs_command_context_t * ctx)
{
    return cmd_log_mask_handler(ctx, gs_log_group_get_level_mask, gs_log_group_set_level_mask);
}

int gs_log_cmd_log_appender_mask_handler(gs_command_context_t * ctx)
{
    return cmd_log_mask_handler(ctx, gs_log_appender_get_level_mask, gs_log_appender_set_level_mask);
}

#ifndef __AVR__
static bool iter_log_completer(void *ctx_in, gs_log_group_t * group)
{
    iter_group_t * ctx = ctx_in;
    unsigned int hits = gs_command_completer_add_token(ctx->ctx, group->name, false);
    if (hits == 1) {
        ctx->first = group;
    } else {
        if (hits == 2) {
            fprintf(ctx->ctx->out, "\r\n");
            iter_print_group(ctx, ctx->first);
        }
        iter_print_group(ctx, group);
    }
    return true;
}

static gs_error_t cmd_log_group_completer(gs_command_context_t * ctx, int arg_to_complete)
{
    if (arg_to_complete == 1) {
        iter_group_t iter = {.ctx = ctx, .completer = true};
        char name[50];
        snprintf(name, sizeof(name), "%s*", (ctx->argc > 1) ? ctx->argv[1] : "");
        gs_log_group_iterate(name, &iter, iter_log_completer);
        return GS_OK;
    }
    return GS_ERROR_AMBIGUOUS;
}

gs_error_t gs_log_cmd_log_group_list_completer(gs_command_context_t* ctx, int arg_to_complete)
{
    return cmd_log_group_completer(ctx, arg_to_complete);
}

gs_error_t gs_log_cmd_log_group_mask_completer(gs_command_context_t* ctx, int arg_to_complete)
{
    return cmd_log_group_completer(ctx, arg_to_complete);
}

gs_error_t gs_log_cmd_log_group_insert_completer(gs_command_context_t* ctx, int arg_to_complete)
{
    return cmd_log_group_completer(ctx, arg_to_complete);
}

gs_error_t gs_log_cmd_debug_completer(gs_command_context_t* ctx, int arg_to_complete)
{
    return cmd_log_group_completer(ctx, arg_to_complete);
}

static bool iter_log_appender_completer(void *ctx_in, gs_log_appender_t * appender)
{
    iter_group_t * ctx = ctx_in;
    unsigned int hits = gs_command_completer_add_token(ctx->ctx, appender->name, false);
    if (hits == 1) {
        ctx->first_appender = appender;
    } else {
        if (hits == 2) {
            fprintf(ctx->ctx->out, "\r\n");
            iter_print_appender(ctx, ctx->first_appender);
        }
        iter_print_appender(ctx, appender);
    }
    return true;
}

static gs_error_t cmd_log_appender_completer(gs_command_context_t * ctx, int arg_to_complete)
{
    if (arg_to_complete == 1) {
        iter_group_t iter = {.ctx = ctx, .completer = true};
        char name[50];
        snprintf(name, sizeof(name), "%s*", (ctx->argc > 1) ? ctx->argv[1] : "");
        gs_log_appender_iterate(name, &iter, iter_log_appender_completer);
        return GS_OK;
    }
    return GS_ERROR_AMBIGUOUS;
}

gs_error_t gs_log_cmd_log_appender_list_completer(gs_command_context_t* ctx, int arg_to_complete)
{
    return cmd_log_appender_completer(ctx, arg_to_complete);
}

gs_error_t gs_log_cmd_log_appender_mask_completer(gs_command_context_t* ctx, int arg_to_complete)
{
    return cmd_log_appender_completer(ctx, arg_to_complete);
}

gs_error_t gs_log_cmd_log_appender_hist_completer(gs_command_context_t* ctx, int arg_to_complete)
{
    return cmd_log_appender_completer(ctx, arg_to_complete);
}

gs_error_t gs_log_cmd_log_appender_clear_completer(gs_command_context_t* ctx, int arg_to_complete)
{
    return cmd_log_appender_completer(ctx, arg_to_complete);
}
#endif

typedef struct {
    gs_command_context_t *cmd_ctx;
    uint32_t count;
    uint32_t found;
    const char * pattern;
} hist_ctx_t;

static bool appender_history_iter(void *ctx, gs_log_level_t level, const gs_timestamp_t *ts, const char *group, const char *msg)
{
    hist_ctx_t * hist_ctx = ctx;

    if (hist_ctx->pattern &&
        (gs_string_match(hist_ctx->pattern, group) == false) &&
        (gs_string_match(hist_ctx->pattern, msg) == false)) {
        return true;
    }

    gs_command_set_output_printf(hist_ctx->cmd_ctx, NULL, NULL,
                                 "%s%04"PRIu32".%06"PRIu32" %c %s: %s%s",
                                 gs_log_level_to_color_begin(level),
                                 ts->tv_sec, ts->tv_nsec/1000,
                                 gs_log_level_to_char(level),
                                 group,
                                 msg,
                                 gs_log_level_to_color_end());

    if (++hist_ctx->found >= hist_ctx->count) {
        return false;
    }
    return true;
}

int gs_log_cmd_log_appender_hist_handler(gs_command_context_t * ctx)
{
    hist_ctx_t hist_ctx = {.cmd_ctx = ctx, .count = 20};

    if ((ctx->argc > 2) && (gs_string_to_uint32(ctx->argv[2], &hist_ctx.count) != GS_OK)) {
        return GS_ERROR_ARG;
    }
    if (ctx->argc > 3) {
        hist_ctx.pattern = ctx->argv[3];
    }

    gs_error_t error = gs_log_appender_history_iterate(ctx->argv[1], &hist_ctx, appender_history_iter);
    if ((error == GS_OK) && (hist_ctx.found == 0)) {
        error = GS_ERROR_NOT_FOUND;
    }
    return error;
}

static bool iter_log_appender_clear(void *ctx_in, gs_log_appender_t * appender)
{
    iter_group_t * iter = ctx_in;
    if (appender && appender->drv && appender->drv->clear) {
        iter->error = (appender->drv->clear)(appender);
    } else {
        iter->error = GS_ERROR_NOT_SUPPORTED;
    }
    return true;
}

int gs_log_cmd_log_appender_clear_handler(gs_command_context_t * ctx)
{
    iter_group_t iter = {.ctx = ctx};
    gs_error_t error = gs_log_appender_iterate(ctx->argv[1], &iter, iter_log_appender_clear);
    if (error == GS_OK) {
        if (gs_string_has_wildcards(ctx->argv[1]) == false) {
            error = iter.error;
        }
    }
    return error;
}

static bool iter_log_group_find(void* ctx_in, gs_log_group_t *group)
{
    gs_log_group_t **grp = ctx_in;
    *grp = group;
    return false;
}

int gs_log_cmd_log_group_insert_handler(gs_command_context_t * ctx)
{
    gs_log_group_t *log_group = NULL;
    gs_error_t error = gs_log_group_iterate(ctx->argv[1], &log_group, iter_log_group_find);
    if (error != GS_OK) {
        return error;
    }

    gs_log_level_t level;
    error = gs_log_string_to_level(ctx->argv[2], &level);
    if (error == GS_OK) {
        gs_log(level, log_group, GS_PGM_STR("%s"), ctx->argv[3]);
    }

    return error;
}

int gs_log_cmd_log_group_color_handler(gs_command_context_t * ctx)
{
    bool color;
    gs_error_t error = gs_string_to_bool(ctx->argv[1], &color);
    if (error == GS_OK) {
        gs_log_set_print_color(color);
    }

    return error;
}
