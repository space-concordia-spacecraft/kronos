/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/csp/service_dispatcher.h>
#include <gs/util/watchdog/watchdog.h>
#include <gs/util/check.h>
#include <gs/util/log.h>
#include <stdlib.h>
#include "../lib/libcsp/src/csp_conn.h"  // internal libcsp header
#include "local.h"

static GS_LOG_GROUP(gs_cspdispatcher_log, "cspdispatcher", GS_LOG_CAT_CSP, LOG_DEFAULT_MASK);
#undef  LOG_DEFAULT
#define LOG_DEFAULT gs_cspdispatcher_log

#define WD_TIMEOUT_SECOUNDS  30
#define DEFAULT_TIMEOUT_MS   (((WD_TIMEOUT_SECOUNDS * 1000) / 3) * 2)

struct gs_csp_service_dispatcher {
    // Configuration
    const gs_csp_service_dispatcher_conf_t * conf;
    // Run or stop/exit.
    bool run;
    // Server socket
    csp_socket_t * socket;
    // Software watchdog
    gs_swwd_hdl_t * wd;
    // Thread handle.
    gs_thread_t thread;
};

static void * service_dispatcher_task(void * parameter)
{
    gs_csp_service_dispatcher_t handle = parameter;

    unsigned int timeout_ms = (handle->conf->callback) ? 0 : DEFAULT_TIMEOUT_MS;

    log_debug("[%s] entering connection loop, timeout: %u mS", handle->conf->name, timeout_ms);

    while (handle->run) {
        if (handle->wd) {
            gs_swwd_touch(handle->wd);
        }

        /* Wait for incoming connection, or timeout */
        csp_conn_t * conn = csp_accept(handle->socket, timeout_ms);

        if (conn) {
            unsigned int in_port = csp_conn_dport(conn);

            log_debug("[%s] new connection %p on port: %u, source: %d:%d, flags: 0x%x",
                      handle->conf->name, conn, in_port, csp_conn_src(conn), csp_conn_sport(conn), csp_conn_flags(conn));

            gs_csp_service_handler_t handler;
            if (in_port < handle->conf->handler_array_size) {
                handler = handle->conf->handler_array[in_port];
            } else {
                handler = NULL;
            }

            if (handler) {
                gs_error_t error = (handler)(conn);
                log_debug("[%s] connection on port: %u processed by %p, error: %s",
                          handle->conf->name, in_port, handler, gs_error_string(error));
            } else {
                log_warning("[%s] no handler on port: %u - closing connection: source: %d:%d, flags: 0x%x",
                            handle->conf->name, in_port, csp_conn_src(conn), csp_conn_sport(conn), csp_conn_flags(conn));
                csp_close(conn);
            }
        }

        if (handle->conf->callback) {
            timeout_ms = handle->conf->callback();
            if (timeout_ms > DEFAULT_TIMEOUT_MS) {
                timeout_ms = DEFAULT_TIMEOUT_MS;
            }
        }
    }

    log_debug("[%s] terminating ...", handle->conf->name);
    gs_thread_exit(NULL);
}

static gs_error_t service_dispatcher_create(const gs_csp_service_dispatcher_conf_t * conf,
                                            size_t stack_size,
                                            gs_thread_priority_t priority,
                                            gs_csp_service_dispatcher_t * return_handle)
{
    gs_csp_service_dispatcher_t handle = calloc(1, sizeof(*handle));
    if (handle == 0) {
        return GS_ERROR_ALLOC;
    }

    *return_handle = handle;

    handle->conf = conf;

    // Create watchdog
    if (conf->disable_watchdog == false) {
        gs_error_t error = gs_swwd_register(&handle->wd, WD_TIMEOUT_SECOUNDS, NULL, NULL, conf->name);
        if (error) {
            log_error("[%s] gs_swwd_register(%s, %u) failed, error: %d",
                      conf->name, conf->name, WD_TIMEOUT_SECOUNDS, error);
            handle->wd = NULL;
            return error;
        }
    }

    // Open "server" socket
    handle->socket = csp_socket(conf->socket_options);
    if (handle->socket == NULL) {
        log_error("[%s] csp_socket(0) failed",
                  conf->name);
        return GS_ERROR_ALLOC;
    }

    // Bind to port(s) to socket
    for (unsigned int i = 0; i < conf->handler_array_size; ++i) {
        if (conf->handler_array[i]) {
            int res = csp_bind(handle->socket, i);
            if (res) {
                log_error("[%s] csp_bind(socket: %p, port: %u) failed, result: %d",
                          conf->name, handle->socket, i, res);
                return GS_ERROR_IN_USE;
            }
        }
    }

    // Bind on "any" port?
    if (conf->bind_any) {
        int res = csp_bind(handle->socket, CSP_ANY);
        if (res) {
            log_error("[%s] csp_bind(socket: %p, port: %u) failed, result: %d",
                      conf->name, handle->socket, CSP_ANY, res);
            return GS_ERROR_IN_USE;
        }
    }

    // Create listen backlog
    {
        size_t backlog = conf->listen_backlog ? conf->listen_backlog : 10;
        int res = csp_listen(handle->socket, backlog);
        if (res) {
            log_error("[%s] csp_listen(%p, %zu) failed, result: %d",
                      conf->name, handle->socket, backlog, res);
            return GS_ERROR_UNKNOWN;
        }
    }

    // Launch thread
    handle->run = true;
    gs_error_t error = gs_thread_create(handle->conf->name, service_dispatcher_task, handle, stack_size, priority,
                                        GS_THREAD_CREATE_JOINABLE, &handle->thread);
    if (error) {
        handle->thread = 0;
    }

    return error;
}

gs_error_t gs_csp_service_dispatcher_create(const gs_csp_service_dispatcher_conf_t * conf,
                                            size_t stack_size,
                                            gs_thread_priority_t priority,
                                            gs_csp_service_dispatcher_t * return_handle)
{
    GS_CHECK_ARG(conf != NULL);

    gs_log_group_register(gs_cspdispatcher_log);

    gs_csp_service_dispatcher_t handle;
    gs_error_t error = service_dispatcher_create(conf, stack_size, priority, &handle);
    if (error) {
        //gs_csp_service_dispatcher_destroy(handle);
        handle = NULL;
    }

    if (return_handle) {
        *return_handle = handle;
    }

    return error;
}

gs_error_t gs_csp_service_dispatcher_wake_up(gs_csp_service_dispatcher_t handle)
{
    GS_CHECK_HANDLE(handle && handle->socket && handle->socket->socket);
    csp_packet_t * packet = NULL;
    int res = csp_queue_enqueue(handle->socket->socket, &packet, 0);
    return (res == CSP_QUEUE_OK) ? GS_OK : GS_ERROR_FULL;
}

gs_error_t gs_csp_service_dispatcher_destroy(gs_csp_service_dispatcher_t handle)
{
    if (handle == NULL) {
        return GS_OK;
    }

    GS_CHECK_HANDLE(handle->conf);

    log_debug("[%s] stopping dispatcher ...", handle->conf->name);

    handle->run = false;
    if (handle->thread) {
        gs_csp_service_dispatcher_wake_up(handle);
        gs_thread_join(handle->thread, NULL);
    }

    csp_close(handle->socket);

    if (handle->wd) {
        gs_swwd_deregister(&handle->wd);
    }

    memset(handle, 0, sizeof(*handle));
    free(handle);

    return GS_OK;
}
