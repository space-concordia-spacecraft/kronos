/* Copyright (c) 2013-2019 GomSpace A/S. All rights reserved. */

#include <gs/util/lock.h>
#include <gs/util/mutex.h>

static gs_mutex_t gs_lock;

gs_error_t gs_lock_init(void)
{
    if (gs_lock == NULL) {
        return gs_mutex_create(&gs_lock);
    }
    return GS_OK;
}

gs_error_t gs_lock_lock(void)
{
    if (gs_lock == NULL) {
        gs_error_t error = gs_lock_init();
        if (error) {
            return error;
        }
    }
    return gs_mutex_lock(gs_lock);
}

gs_error_t gs_lock_unlock(void)
{
    return gs_mutex_unlock(gs_lock);
}
