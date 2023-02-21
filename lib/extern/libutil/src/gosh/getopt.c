/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/gosh/command.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gs_command_getopt(gs_command_context_t *ctx, const char *opts)
{
    int c;
    char *cp;

    if (ctx->optsp == 1) {
        if (ctx->optind >= ctx->argc ||
            ctx->argv[ctx->optind][0] != '-' ||
            ctx->argv[ctx->optind][1] == '\0') {
            return EOF;
        } else if (!strcmp(ctx->argv[ctx->optind], "--")) {
            ctx->optind++;
            return EOF;
        }
    }

    ctx->optopt = c = ctx->argv[ctx->optind][ctx->optsp];
    if (c == ':' || (cp = strchr(opts, c)) == NULL) {
        printf("illegal option -- %c\r\n", c);
        if (ctx->argv[ctx->optind][++ctx->optsp] == '\0') {
            ctx->optind++;
            ctx->optsp = 1;
        }
        return '?';
    }

    if (*++cp == ':') {
        if (ctx->argv[ctx->optind][ctx->optsp+1] != '\0') {
            ctx->optarg = &ctx->argv[ctx->optind++][ctx->optsp+1];
        } else if(++ctx->optind >= ctx->argc) {
            printf("option requires an argument -- %c\r\n", c);
            ctx->optsp = 1;
            return '?';
        } else {
            ctx->optarg = ctx->argv[ctx->optind++];
        }
        ctx->optsp = 1;
    } else {
        if (ctx->argv[ctx->optind][++ctx->optsp] == '\0') {
            ctx->optsp = 1;
            ctx->optind++;
        }
        ctx->optarg = NULL;
    }

    return c;
}
