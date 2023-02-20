/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/csp/csp.h>
#include <gs/csp/log.h>
#include <gs/util/check.h>
#include <gs/util/minmax.h>
#include <csp/arch/csp_system.h>
#include "local.h"

#define GSCSP_FREERTOS 1

const char *csp_get_hostname(void)
{
    return csp_get_conf()->hostname;
}

const char *csp_get_model(void)
{
    return csp_get_conf()->model;
}

const char *csp_get_revision(void)
{
    return csp_get_conf()->revision;
}

void gs_csp_conf_get_defaults_embedded(gs_csp_conf_t * conf)
{
    static const gs_csp_conf_t defaults = {
        .use_gs_log = true,
        .use_command_line_options = true,
        .csp_buffer_size = 256,  // typical MTU size is 256
        .csp_buffers = 10, // in case of RDP connections, must be > RDP Windows size
        .address = 1,
        .hostname = "hostname",
        .model = "model",
        .revision = "revision",
        .max_connections = 5,
        .rx_queue_length = 20,
        .router_queue_length = 100,
        .max_bind_port = 18,
        .max_rdp_window_size = 20,
        .default_connection_options = CSP_O_CRC32,
    };

    *conf = defaults;

#if GS_CSP_COMMAND_LINE_SUPPORT
    conf->address = gs_csp_command_line_get_address();
#endif
}

void gs_csp_conf_get_defaults_server(gs_csp_conf_t * conf)
{
    gs_csp_conf_get_defaults_embedded(conf);
    conf->csp_buffer_size = 512;
    conf->csp_buffers = 400;
    conf->max_connections = 20;
    conf->rx_queue_length = 100;
    conf->max_bind_port = 31;
}

gs_error_t gs_csp_init(const gs_csp_conf_t * conf)
{
    GS_CHECK_ARG(conf != NULL);

    gs_csp_log_init();

    csp_conf_t csp_conf;
    csp_conf_get_defaults(&csp_conf);
    csp_conf.address = conf->address;
    csp_conf.hostname = conf->hostname;
    csp_conf.model = conf->model;
    csp_conf.revision = conf->revision;
    csp_conf.conn_max = conf->max_connections;
    csp_conf.conn_queue_length = gs_min(conf->rx_queue_length, conf->csp_buffers + 1);
    csp_conf.conn_dfl_so = conf->default_connection_options;
    csp_conf.fifo_length = gs_min(conf->router_queue_length, conf->csp_buffers + 1);
    csp_conf.port_max_bind = conf->max_bind_port;
    csp_conf.rdp_max_window = conf->max_rdp_window_size;
    csp_conf.buffers = conf->csp_buffers;
    csp_conf.buffer_data_size = conf->csp_buffer_size;

#if GS_CSP_COMMAND_LINE_SUPPORT
    if (conf->use_command_line_options && gs_csp_command_line_is_address_set()) {
        csp_conf.address = gs_csp_command_line_get_address();
    }
#endif

    if (csp_conf.conn_dfl_so & CSP_O_NOCRC32) {
        // strip CRC32 flag
        csp_conf.conn_dfl_so &= ~(CSP_O_CRC32);
    }

    int res = csp_init(&csp_conf);
    if (res != CSP_ERR_NONE) {
        log_error("%s: csp_init(address: %u) failed, CSP error: %d, error: %d",
                  __FUNCTION__, csp_conf.address, res, gs_csp_error(res));
        return gs_csp_error(res);
    }

#if GS_CSP_COMMAND_LINE_SUPPORT
    if (conf->use_command_line_options) {
        gs_error_t error = gs_csp_command_line_configure_interfaces();
        if (error) {
            log_error("%s: gs_csp_command_line_configure_interfaces() failed, error: %d -> exit",
                      __FUNCTION__, error);
            // exit vs. return, should probably be handled by an option/argument, but for now we exit()
            exit(1);
        }
    }
#endif

    if (GSCSP_FREERTOS) {
        extern int gs_csp_sys_reboot(void);
        csp_sys_set_reboot(gs_csp_sys_reboot);
    }
    
    return GS_OK;
}

bool gs_csp_is_address_valid(uint8_t address)
{
    if (address < 1) {
        return false;
    }
    if (address >= 33) {
        return false;
    }
    return true;
}

void gs_csp_free_resources(void)
{
    csp_free_resources();
}

