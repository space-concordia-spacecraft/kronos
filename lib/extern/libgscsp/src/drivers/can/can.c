/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/csp/drivers/can/can.h>
#include <gs/csp/csp.h>
#include <csp/interfaces/csp_if_can.h>
#include <gs/util/check.h>
#include <gs/util/log.h>
#include <gs/embed/drivers/can/can.h>
#include <gs/embed/freertos.h>
#include <stdlib.h>

#define MAX_DEVICES 2

// change default log group
#define LOG_DEFAULT gs_can_log

typedef struct {
    char name[CSP_IFLIST_NAME_MAX + 1];
    csp_iface_t iface;
    csp_can_interface_data_t ifdata;
} gs_csp_can_interface_t;

static gs_csp_can_interface_t * csp_can_interfaces[MAX_DEVICES];

static void gs_csp_can_rxdata_callback_isr(int device,
                                           uint32_t canMsgId,
                                           bool extendedMsgId,
                                           const void * data,
                                           size_t data_size,
                                           uint32_t nowMs,
                                           void * user_data,
                                           gs_context_switch_t * cswitch)
{
    csp_can_rx(&csp_can_interfaces[device]->iface, canMsgId, data, data_size, &cswitch->task_woken);
}

static int csp_can_tx_frame(void * driver_data, uint32_t id, const uint8_t * data, uint8_t dlc)
{
    return gs_can_send_extended(GS_TYPES_PTR2UINT(driver_data), id, data, dlc, 1000);
}

gs_error_t gs_csp_can_init2(uint8_t device, uint8_t csp_addr, uint32_t mtu, const char * name, bool set_default_route, csp_iface_t ** csp_if)
{
    GS_CHECK_HANDLE(device < MAX_DEVICES);
    gs_csp_can_interface_t * interface = csp_can_interfaces[device];
    if (interface == NULL) {
        interface = csp_can_interfaces[device] = malloc(sizeof(*interface));
        if (interface == NULL) {
            return GS_ERROR_ALLOC;
        }
    }

    if (gs_string_empty(name)) {
        name = GS_CSP_CAN_DEFAULT_IF_NAME;
    }

    memset(interface, 0, sizeof(*interface));
    GS_STRNCPY(interface->name, name);
    interface->iface.name = interface->name;
    interface->iface.mtu = mtu;
    interface->iface.interface_data = &interface->ifdata;
    interface->ifdata.tx_func = csp_can_tx_frame;
    interface->iface.driver_data = GS_TYPES_UINT2PTR(device);

    csp_iface_t * iface = &interface->iface;
    int res = csp_can_add_interface(iface);
    if (res != CSP_ERR_NONE) {
        log_error("%s[%u]: csp_can_add_interface() failed, error: %d", __FUNCTION__, device, res);
        return gs_csp_error(res);
    }

    // Register/subscribe to CAN frames for CSP
    const uint32_t can_id = CFP_MAKE_DST(csp_get_address());
    const uint32_t can_mask = CFP_MAKE_DST((1 << CFP_HOST_SIZE) - 1);

    log_debug("%s[%u]: id=0x%" PRIx32 ", mask=0x%" PRIx32, __FUNCTION__, device, can_id, can_mask);

    gs_error_t error = gs_can_set_extended_filter_mask(device, can_id, can_mask, gs_csp_can_rxdata_callback_isr, NULL);
    if (error) {
        log_error("%s[%u]: gs_can_set_extended_filter_mask() failed, error: %s", __FUNCTION__, device, gs_error_string(error));
        return error;
    }

    if (csp_if) {
        *csp_if = iface;
    }

    if (set_default_route) {
        csp_rtable_set(0, 0, iface, CSP_NO_VIA_ADDRESS);
    }

    error = gs_can_start(device);
    if (error) {
        log_error("%s[%u]: gs_can_start() failed, error: %s", __FUNCTION__, device, gs_error_string(error));
        return error;
    }

    return GS_OK;
}

gs_error_t gs_csp_can_init(uint8_t device, uint8_t csp_addr, uint32_t mtu, const char * name, csp_iface_t ** csp_if)
{
    return gs_csp_can_init2(device, csp_addr, mtu, name, true, csp_if);
}
