/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Overwrite weak definitions in libcsp.
*/

#include <csp/arch/csp_clock.h>
#include <gs/util/clock.h>

CSP_STATIC_ASSERT(sizeof(gs_timestamp_t) == sizeof(csp_timestamp_t), csp_timestamp_t_size);
CSP_STATIC_ASSERT(offsetof(gs_timestamp_t, tv_sec) == offsetof(csp_timestamp_t, tv_sec), csp_timestamp_t_tv_sec);
CSP_STATIC_ASSERT(offsetof(gs_timestamp_t, tv_nsec) == offsetof(csp_timestamp_t, tv_nsec), csp_timestamp_t_tv_nsec);

void csp_clock_get_time(csp_timestamp_t * time)
{
    gs_clock_get_time((gs_timestamp_t*)time);
}

int csp_clock_set_time(const csp_timestamp_t * time)
{
    return (gs_clock_set_time((const gs_timestamp_t*)time) == GS_OK) ? CSP_ERR_NONE : CSP_ERR_NOTSUP;
}
