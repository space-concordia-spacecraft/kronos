/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/csp/drivers/i2c/i2c.h>
#include <gs/csp/csp.h>
#include <csp/interfaces/csp_if_i2c.h>
#include <gs/util/drivers/i2c/master.h>
#include <gs/util/drivers/i2c/slave.h>
#include <gs/util/string.h>
#include <gs/util/check.h>
#if !defined(__linux__)
#include <gs/embed/freertos.h>
#endif
#include <stdlib.h>

#define MAX_DEVICES          3
#define I2C_FRAME_OVERHEAD   (offsetof(csp_i2c_frame_t, data))

typedef struct {
    char name[CSP_IFLIST_NAME_MAX + 1];
    csp_iface_t iface;
    csp_i2c_interface_data_t ifdata;
} gs_csp_i2c_interface_t;

static gs_csp_i2c_interface_t * csp_i2c_interfaces[MAX_DEVICES];

static void gs_csp_i2c_rxdata_callback_isr(uint8_t device, const uint8_t * rx, size_t rx_length, gs_context_switch_t * cswitch)
{
    csp_i2c_frame_t * frame = (csp_i2c_frame_t *) (void *) (rx - I2C_FRAME_OVERHEAD);
    frame->len = rx_length;
#if (__linux__)
    csp_i2c_rx(&csp_i2c_interfaces[device]->iface, frame, NULL);
#else
    csp_i2c_rx(&csp_i2c_interfaces[device]->iface, frame, &cswitch->task_woken);
#endif
}

static void * gs_csp_i2c_get_buffer_isr(uint8_t device)
{
    uint8_t * buff = csp_buffer_get_isr(0);
    if (buff) {
        buff = buff + I2C_FRAME_OVERHEAD;
    }
    return buff;
}

static int csp_i2c_driver_tx(void * driver_data, csp_i2c_frame_t * frame)
{
    int res_tx = gs_i2c_master_transaction(GS_TYPES_PTR2UINT(driver_data), frame->dest, frame->data, frame->len, 0, 0, 1000);
    if (res_tx == GS_OK) {
        csp_buffer_free(frame);
        return CSP_ERR_NONE;
    }
    return CSP_ERR_TX;
}

gs_error_t gs_csp_i2c_init2(uint8_t device, uint32_t mtu, const char * name, bool set_default_route, csp_iface_t ** csp_if)
{
    GS_CHECK_HANDLE(device < MAX_DEVICES);
    gs_csp_i2c_interface_t * interface = csp_i2c_interfaces[device];
    if (interface == NULL) {
        interface = csp_i2c_interfaces[device] = malloc(sizeof(*interface));
        if (interface == NULL) {
            return GS_ERROR_ALLOC;
        }
    }

    gs_error_t error = gs_i2c_slave_set_rx(device, gs_csp_i2c_rxdata_callback_isr);
    if (error) {
        return error;
    }
    error = gs_i2c_slave_set_get_rx_buf(device, gs_csp_i2c_get_buffer_isr, csp_buffer_data_size() + sizeof(csp_id_t)); // == what's on the wire
    if (error) {
        return error;
    }

    if (gs_string_empty(name)) {
        name = CSP_IF_I2C_DEFAULT_NAME;
    }

#if defined(__AVR32__)
    if (mtu == 0) {
        mtu = (csp_buffer_data_size() - 1); // compensate for A3200 driver weirdness to receive 1 byte less, than specified.
    }
#endif

    memset(interface, 0, sizeof(*interface));
    GS_STRNCPY(interface->name, name);
    interface->iface.name = interface->name;
    interface->iface.mtu = mtu;
    interface->iface.interface_data = &interface->ifdata;
    interface->ifdata.tx_func = csp_i2c_driver_tx;
    interface->iface.driver_data = GS_TYPES_UINT2PTR(device);

    csp_iface_t * iface = &interface->iface;
    int res = csp_i2c_add_interface(iface);
    if (res != CSP_ERR_NONE) {
        log_error("%s[%u]: csp_i2c_add_interface() failed, error: %d", __FUNCTION__, device, res);
        return gs_csp_error(res);
    }

    if (csp_if) {
        *csp_if = iface;
    }

    if (set_default_route) {
        csp_rtable_set(0, 0, iface, CSP_NO_VIA_ADDRESS);
    }

    if ((error = gs_i2c_slave_start(device)) != GS_OK) {
        return error;
    }

    return GS_OK;
}

gs_error_t gs_csp_i2c_init(uint8_t device, uint8_t csp_addr)
{
    return gs_csp_i2c_init2(device, 0, NULL, false, NULL);
}
