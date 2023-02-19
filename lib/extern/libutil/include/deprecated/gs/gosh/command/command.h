#ifndef GS_GOSH_COMMAND_COMMAND_H
#define GS_GOSH_COMMAND_COMMAND_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   Legacy header file - use gs/util/gosh/command.h
*/

#include <gs/util/gosh/command.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CMD_ERROR_NONE           GS_OK
#define CMD_ERROR_FAIL           GS_ERROR_UNKNOWN
#define CMD_ERROR_SYNTAX         GS_ERROR_ARG
#define CMD_ERROR_NOMEM          GS_ERROR_ALLOC
#define CMD_ERROR_INVALID        GS_ERROR_DATA
#define CMD_ERROR_NOTFOUND       GS_ERROR_NOT_FOUND

#define CMD_HIDDEN               GS_COMMAND_FLAG_HIDDEN

#define __root_command           GS_COMMAND_ROOT
#define __sub_command            GS_COMMAND_SUB

#define INIT_CHAIN(__list)       GS_COMMAND_INIT_CHAIN(__list)
#define command_register(__cmd)  GS_COMMAND_REGISTER(__cmd)

typedef struct command command_t;

static inline const char * command_args(gs_command_context_t *ctx)
{
    return gs_command_args(ctx);
}

static inline int command_run(char *line)
{
    gs_error_t result = GS_OK;
    gs_error_t error = gs_command_run(line, &result);
    if (error == GS_OK) {
        return result;
    }
    return error;
}

#ifdef __cplusplus
}
#endif
#endif
