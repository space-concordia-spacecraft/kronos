/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/csp/drivers/kiss/kiss.h>
//#include <gs/embed/drivers/uart/uart.h> TODO: Replace or remove
#include <gs/csp/csp.h>
#include <gs/util/string.h>
#include <gs/util/check.h>
#include <stdlib.h>

#define MAX_DEVICES 5

typedef struct {
    char name[CSP_IFLIST_NAME_MAX + 1];
    csp_iface_t iface;
    csp_kiss_interface_data_t ifdata;
} gs_csp_kiss_interface_t;

static gs_csp_kiss_interface_t * csp_kiss_interfaces[MAX_DEVICES];

static int csp_kiss_driver_tx(void * driver_data, const uint8_t * data, size_t data_length)
{
    size_t wrote = 0;
//    gs_error_t error = gs_uart_write_buffer(GS_TYPES_PTR2UINT(driver_data), 1000, data, data_length, &wrote);
    return CSP_ERR_DRIVER;
}

static void uart_rx_callback_isr(void * user_data, const uint8_t * data, size_t data_size, gs_context_switch_t * cswitch)
{
    // TODO: Figure out what the fuck
//    csp_kiss_rx(user_data, data, data_size, &cswitch->task_woken);
}

gs_error_t gs_csp_kiss_init2(uint8_t device, uint32_t mtu, const char * name, bool set_default_route, csp_iface_t ** csp_if)
{
    GS_CHECK_HANDLE(device < MAX_DEVICES);
    gs_csp_kiss_interface_t * interface = csp_kiss_interfaces[device];
    if (interface == NULL) {
        interface = csp_kiss_interfaces[device] = malloc(sizeof(*interface));
        if (interface == NULL) {
            return GS_ERROR_ALLOC;
        }
    }

    if (gs_string_empty(name)) {
        name = GS_CSP_KISS_DEFAULT_IF_NAME;
    }

    memset(interface, 0, sizeof(*interface));
    GS_STRNCPY(interface->name, name);
    interface->iface.name = interface->name;
    interface->iface.mtu = mtu;
    interface->iface.interface_data = &interface->ifdata;
    interface->ifdata.tx_func = csp_kiss_driver_tx;
    interface->iface.driver_data = GS_TYPES_UINT2PTR(device);

    csp_iface_t * iface = &interface->iface;
    gs_error_t error = CSP_ERR_DRIVER;//gs_uart_set_rx_callback(device, uart_rx_callback_isr, iface);
    if (error) {
        log_error("%s[%u]: gs_uart_set_rx_callback() failed, error: %s", __FUNCTION__, device, gs_error_string(error));
        return error;
    }

    int res = csp_kiss_add_interface(iface);
    if (res != CSP_ERR_NONE) {
        log_error("%s[%u]: csp_kiss_add_interface() failed, error: %d", __FUNCTION__, device, res);
        return gs_csp_error(res);
    }

    if (csp_if) {
        *csp_if = iface;
    }

    if (set_default_route) {
        csp_rtable_set(0, 0, iface, CSP_NO_VIA_ADDRESS);
    }

    return GS_OK;
}

gs_error_t gs_csp_kiss_init(uint8_t device)
{
    return gs_csp_kiss_init2(device, 0, CSP_IF_KISS_DEFAULT_NAME, true, NULL);
}
