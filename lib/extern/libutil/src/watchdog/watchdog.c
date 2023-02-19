/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include "local.h"
#include <gs/util/check.h>
#include <gs/util/log.h>
#include <gs/util/time.h>
#include <gs/util/mutex.h>
#include <stdlib.h>

#define GS_SWWD_DEFAULT_TIMEOUT     30      /* 30 second timeout */

// define log group and make it default
GS_LOG_GROUP(gs_swwd_log, "swwd", GS_LOG_CAT_DEFAULT, GS_LOG_DEFAULT_MASK);

// watchdog state
typedef enum {
    GS_SWWD_STATE_FREE = 0,
    GS_SWWD_STATE_ACTIVE = 1,
    GS_SWWD_STATE_EXPIRED = 2,
} gs_swwd_state_t;

// watchdog client instance
struct gs_swwd_hdl {
    // State
    gs_swwd_state_t state;
    // Last 'user' touch value - used for detecting if watchdog has been touched (avoid race condition)
    uint32_t last_touch;
    // Last detected touch time
    uint32_t last_touch_ms;

    // User 'set' attributes
    struct {
        // Name
        const char* name;
        // Timeout - converted from seconds (given to API) to mS
        uint32_t timeout_ms;
        // Action when/if timeout occurs
        gs_swwd_timeout_action_t action;
        // callback
        gs_swwd_callback_function_t cb;
        // user data (for callback)
        void * cb_userdata;
        // Touch - incremented on each touch
        uint32_t touch;
    } user;
};

typedef struct gs_swwd {
    gs_watchdog_device_t *wdev;
    gs_mutex_t lock;
    gs_swwd_monitor_task_t monitor;
    uint32_t num_clients;
    gs_swwd_hdl_t clients[0];
} gs_swwd_t;

static gs_swwd_t* g_swwd = NULL;

gs_swwd_monitor_task_t * gs_swwd_get_monitor_task(void)
{
    if (g_swwd) {
        return &g_swwd->monitor;
    }
    return NULL;
}

gs_error_t gs_swwd_create(uint32_t max_clients, gs_watchdog_device_t * wdev)
{
    GS_CHECK_SUPPORTED(g_swwd == NULL); /* SWWD must not be initialized more than once */
    GS_CHECK_ARG(max_clients > 0);
    if (wdev) {
        // ping is the only mandatory
        GS_CHECK_ARG((wdev->ops != NULL) && (wdev->ops->ping != NULL));
    }

    gs_log_group_register(LOG_DEFAULT);

    gs_swwd_t *swwd_obj = calloc(1, sizeof(*swwd_obj) + (sizeof(swwd_obj->clients[0]) * max_clients));
    if (swwd_obj == NULL) {
        return GS_ERROR_ALLOC;
    }

    if (gs_mutex_create(&(swwd_obj->lock))) {
        free(swwd_obj);
        return GS_ERROR_ALLOC;
    }

    swwd_obj->num_clients = max_clients;
    swwd_obj->wdev = wdev;

    if (wdev) {
        if (wdev->ops->set_timeout) {
            wdev->ops->set_timeout(wdev, (wdev->timeout > 0) ? wdev->timeout : GS_SWWD_DEFAULT_TIMEOUT);
        }
        if (wdev->ops->set_pretimeout) {
            wdev->ops->set_pretimeout(wdev, (wdev->pretimeout > 0) ? wdev->pretimeout : (GS_SWWD_DEFAULT_TIMEOUT - (GS_SWWD_DEFAULT_TIMEOUT/10)));
        }
        if (wdev->ops->start) {
            wdev->ops->start(wdev);
        }
        wdev->ops->ping(wdev);

    } else {
        log_warning("%s: no watchdog device specifed - cannot reset system!", __FUNCTION__);
    }

    g_swwd = swwd_obj; /* Set the task handle as the last operation */
    return GS_OK;
}

gs_error_t gs_swwd_destroy(uint32_t timeout_s)
{
    GS_CHECK_SUPPORTED(g_swwd != NULL);

    if (gs_swwd_monitor_task_stop(timeout_s) != GS_OK) {
        return GS_ERROR_BUSY;
    }

    if (g_swwd->wdev && g_swwd->wdev->ops->stop) {
        g_swwd->wdev->ops->stop(g_swwd->wdev);
    }

    gs_mutex_destroy(g_swwd->lock);
    free(g_swwd);
    g_swwd = NULL;

    return GS_OK;
}

static const char * get_action(gs_swwd_timeout_action_t action)
{
    switch (action) {
        case GS_SWWD_TIMEOUT_ACTION_RESET:
            return "reset";
        case GS_SWWD_TIMEOUT_ACTION_LOG:
            return "log";
    }
    return "unknown";
}

gs_error_t gs_swwd_check_expired_clients(uint32_t *num_expired)
{
    GS_CHECK_SUPPORTED(g_swwd != NULL); /* SWWD must be initialized */

    uint32_t expired_clients_reset = 0;
    uint32_t expired_clients_log = 0;

    gs_mutex_lock(g_swwd->lock);
    for (uint32_t idx = 0; idx < g_swwd->num_clients; idx++) {
        gs_swwd_hdl_t * wd = &g_swwd->clients[idx];

        uint32_t now_ms = gs_time_rel_ms();
        if (wd->state == GS_SWWD_STATE_ACTIVE) {
            if (wd->last_touch != wd->user.touch) {
                // watchdog has been touched since last we checked - update touch time
                wd->last_touch = wd->user.touch;
                wd->last_touch_ms = now_ms;
            } else {
                const uint32_t elapsed_ms = gs_time_diff_ms(wd->last_touch_ms, now_ms);
                if (elapsed_ms >= wd->user.timeout_ms) {
                    wd->state = GS_SWWD_STATE_EXPIRED;

                    char logbuf[100];
                    snprintf(logbuf, sizeof(logbuf),
                             "[%s] expired -> %s (elapsed %"PRIu32" mS, timeout %"PRIu32" mS)",
                             wd->user.name,
                             get_action(wd->user.action),
                             elapsed_ms,
                             wd->user.timeout_ms);

                    gs_swwd_callback_function_t cb = wd->user.cb;
                    void * cb_userdata = wd->user.cb_userdata;

                    // Unlock while doing log and callback
                    // - we accept the tiny risk, that client has deregistered and will be called with invalid data
                    gs_mutex_unlock(g_swwd->lock);
                    {
                        log_error("%s", logbuf);
                        if (cb) {
                            (cb)(cb_userdata);
                        }
                    }
                    gs_mutex_lock(g_swwd->lock);
                }
            }
        }
        if (wd->state == GS_SWWD_STATE_EXPIRED) {
            switch (wd->user.action) {
                case GS_SWWD_TIMEOUT_ACTION_RESET:
                    ++expired_clients_reset;
                    break;
                case GS_SWWD_TIMEOUT_ACTION_LOG:
                    if (wd->last_touch != wd->user.touch) {
                        // its alive - reactive watchdog
                        wd->state = GS_SWWD_STATE_ACTIVE;
                        wd->last_touch = wd->user.touch;
                        wd->last_touch_ms = now_ms;
                    } else {
                        ++expired_clients_log;
                    }
                    break;
            }
        }
    }
    gs_mutex_unlock(g_swwd->lock);

    if ((expired_clients_reset == 0) && g_swwd->wdev) {
        g_swwd->wdev->ops->ping(g_swwd->wdev);
    }

    if (num_expired) {
        *num_expired = (expired_clients_reset + expired_clients_log);
    }

    return GS_OK;
}

gs_error_t gs_swwd_register_with_action(gs_swwd_hdl_t ** user_wd,
                                        uint32_t timeout_seconds,
                                        gs_swwd_callback_function_t callback, void * userdata,
                                        const char * name,
                                        gs_swwd_timeout_action_t action)
{
    GS_CHECK_ARG(gs_string_empty(name) == false);
    GS_CHECK_ARG(timeout_seconds > 0);
    GS_CHECK_ARG(user_wd != NULL);
    GS_CHECK_SUPPORTED(g_swwd != NULL); /* SWWD must be initialized */

    gs_swwd_hdl_t * wd = NULL;
    gs_mutex_lock(g_swwd->lock);
    {
        for (unsigned int idx = 0; idx < g_swwd->num_clients; idx++) {
            if (g_swwd->clients[idx].state == GS_SWWD_STATE_FREE) {
                wd = &g_swwd->clients[idx];
                memset(wd, 0, sizeof(*wd));

                // set user stuff
                wd->user.name = name;
                wd->user.timeout_ms = (timeout_seconds * 1000);
                wd->user.cb = callback;
                wd->user.cb_userdata = userdata;
                wd->user.action = action;

                // set internal stuff
                wd->state = GS_SWWD_STATE_ACTIVE;
                wd->last_touch_ms = gs_time_rel_ms();
                break;
            }
        }
    }
    gs_mutex_unlock(g_swwd->lock);

    *user_wd = wd;

    if (wd == NULL) {
        log_error("[%s] cannot create instance due to no available handles", name);
        return GS_ERROR_FULL;
    }

    return GS_OK;
}

gs_error_t gs_swwd_deregister(gs_swwd_hdl_t ** wd)
{
    GS_CHECK_SUPPORTED(g_swwd != NULL); /* SWWD must be initialized */
    GS_CHECK_ARG(wd != NULL);
    GS_CHECK_HANDLE(*wd != NULL);

    gs_mutex_lock(g_swwd->lock);
    memset((*wd), 0, sizeof(**wd));
    gs_mutex_unlock(g_swwd->lock);
    *wd = NULL;

    return GS_OK;
}

gs_error_t gs_swwd_touch(gs_swwd_hdl_t * wd)
{
    GS_CHECK_HANDLE(wd != NULL);

    ++wd->user.touch;
    return GS_OK;
}

gs_error_t gs_swwd_set_timeout(gs_swwd_hdl_t * wd, uint32_t timeout_seconds)
{
    GS_CHECK_ARG(timeout_seconds > 0);
    GS_CHECK_HANDLE(wd != NULL);

    ++wd->user.touch;
    wd->user.timeout_ms = (timeout_seconds * 1000);
    return GS_OK;
}
