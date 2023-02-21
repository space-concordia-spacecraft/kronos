/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
 * GomSpace Parameter System.
 */
#ifndef PARAM_PARAM_LOCK_H_
#define PARAM_PARAM_LOCK_H_

#include <gs/param/table.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline void param_lock(param_index_t * mem)
{
    gs_param_table_lock((gs_param_table_instance_t *) mem);
}

static inline void param_unlock(param_index_t * mem)
{
    gs_param_table_unlock((gs_param_table_instance_t *) mem);
}

#ifdef __cplusplus
}
#endif
#endif
