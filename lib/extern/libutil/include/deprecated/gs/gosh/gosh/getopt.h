#ifndef GS_GOSH_GOSH_GETOPT_H
#define GS_GOSH_GOSH_GETOPT_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   Legacy header file - use gs/util/gosh/getopt.h
*/

#include <gs/util/gosh/command.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline int gosh_getopt(gs_command_context_t *ctx, const char *opts)
{
    return gs_command_getopt(ctx, opts);
}

#ifdef __cplusplus
}
#endif
#endif
