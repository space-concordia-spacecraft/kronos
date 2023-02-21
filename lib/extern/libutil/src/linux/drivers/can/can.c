/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/drivers/can/can.h>
#include <gs/util/thread.h>
#include <gs/util/log.h>
#include <gs/util/string.h>
#include <gs/util/time.h>

#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

typedef struct {
    // true if handle is in use
    bool inuse;

    // opened socket
    int can_socket;

    // receiver thread
    gs_thread_t rxthread;

    // received data callback
    gs_can_rxdata_callback_t rx_callback;
    void * user_data;

} gs_can_handle_t;

#define MAX_CAN_HANDLES   10
static gs_can_handle_t can_handles[MAX_CAN_HANDLES];

static int gs_can_alloc_handle(void)
{
    int handle_id;
    for (handle_id = 0; (handle_id < MAX_CAN_HANDLES) && (can_handles[handle_id].inuse == true); ++handle_id);

    if (handle_id < MAX_CAN_HANDLES) {
        gs_can_handle_t * handle = &can_handles[handle_id];
        memset(handle, 0, sizeof(*handle));
        handle->inuse = true;
        handle->can_socket = -1;
    }

    return handle_id;
}

static inline gs_can_handle_t * gs_can_handle(uint8_t hdl)
{
    if (hdl >= MAX_CAN_HANDLES) {
        return NULL;
    }
    if (can_handles[hdl].inuse == false) {
        return NULL;
    }
    return &can_handles[hdl];
}

static void * gs_can_rx_thread(void * parameter)
{
    int hdl = (int) GS_TYPES_PTR2INT(parameter);

    log_debug("%s: running, hdl: %d", __FUNCTION__, hdl);

    gs_can_handle_t * handle = gs_can_handle(hdl);
    if (handle == NULL) {
        log_error("%s: CAN handle: %d is invalid or not opened", __FUNCTION__, hdl);
        gs_thread_exit(NULL);
    }

    while (1) {
        /* Read CAN frame */
        struct can_frame frame;
        ssize_t nbytes = read(handle->can_socket, &frame, sizeof(frame));
        if (nbytes < 0) {
            log_error("%s: read() on socket failed, error: %s", __FUNCTION__, strerror(errno));
            continue;
        }

        if (nbytes != sizeof(frame)) {
            log_warning("%s: read() returned incomplete CAN frame of %d bytes - ignoring frame", __FUNCTION__, (int) nbytes);
            continue;
        }

        /* Frame type */
        if (frame.can_id & (CAN_ERR_FLAG | CAN_RTR_FLAG)) {
            /* Drop error and remote frames */
            log_warning("%s: discarding ERR/RTR frame, can_id: 0x%x", __FUNCTION__, frame.can_id);
            continue;
        }

        const bool extId = (frame.can_id & CAN_EFF_FLAG) ? true : false;
        if (extId) {
            frame.can_id &= CAN_EFF_MASK;
        } else {
            frame.can_id &= CAN_SFF_MASK;
        }
        handle->rx_callback(hdl, frame.can_id, extId, frame.data, frame.can_dlc, gs_time_rel_ms(), handle->user_data, false);
    }

    /* We should never reach this point */
    return NULL;
}

static gs_error_t gs_can_send(uint8_t hdl, uint32_t canMsgId, bool extended, const void * data, size_t data_size, int timeout_ms)
{
    gs_can_handle_t * handle = gs_can_handle(hdl);
    if (handle == NULL) {
        return GS_ERROR_HANDLE;
    }

    if ((data == NULL) || (data_size > 8)) {
        log_error("%s: invalid data: %p, data_size: %u", __FUNCTION__, (void*) data, (unsigned int) data_size);
        return GS_ERROR_ARG;
    }

    struct can_frame frame;
    memset(&frame, 0, sizeof(frame));
    frame.can_id = canMsgId;
    if (extended) {
        frame.can_id |= CAN_EFF_FLAG;
    }

    memcpy(frame.data, data, data_size);

    frame.can_dlc = (uint8_t) data_size;

    const int DELAY_MS = 10;
    while (write(handle->can_socket, &frame, sizeof(frame)) != sizeof(frame)) {
        if ((timeout_ms > 0) && (errno == ENOBUFS)) {
            // Wait a bit and try again
            gs_thread_sleep_ms(DELAY_MS);
            timeout_ms -= DELAY_MS;
        } else {
            gs_error_t gserror = gs_error(errno);
            log_error("%s: write() failed, error: %s", __FUNCTION__, gs_error_string(gserror));
            return gserror;
        }
    }

    return GS_OK;
}

gs_error_t gs_can_send_standard(uint8_t hdl, uint32_t canMsgId, const void * data, size_t data_size, int timeout_ms)
{
    return gs_can_send(hdl, canMsgId, false, data, data_size, timeout_ms);
}

gs_error_t gs_can_send_extended(uint8_t hdl, uint32_t canExtMsgId, const void * data, size_t data_size, int timeout_ms)
{
    return gs_can_send(hdl, canExtMsgId, true, data, data_size, timeout_ms);
}

static void gs_can_close(gs_can_handle_t * handle)
{
    if (handle->can_socket >= 0) {
        close(handle->can_socket);
    }

    // free instance - must be the last thing done, no lock needed
    handle->inuse = false;
}

gs_error_t gs_can_open(const char * ifname, int * return_handle)
{
    if ((ifname == NULL) || (ifname[0] == 0) || (return_handle == NULL)) {
        log_error("%s: invalid CAN interface name", __FUNCTION__);
        return GS_ERROR_ARG;
    }

    int handle_id = gs_can_alloc_handle();
    if (handle_id >= MAX_CAN_HANDLES) {
        log_error("%s: no free handles", __FUNCTION__);
        return GS_ERROR_FULL;
    }
    gs_can_handle_t * handle = &can_handles[handle_id];

    /* Create socket */
    if ((handle->can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        gs_error_t gserror = gs_error(errno);
        log_error("%s: socket() failed, error: %s", __FUNCTION__, gs_error_string(gserror));
        gs_can_close(handle);
        return gserror;
    }

    /* Locate interface */
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name) - 1);
    if (ioctl(handle->can_socket, SIOCGIFINDEX, &ifr) < 0) {
        gs_error_t gserror = gs_error(errno);
        log_error("%s: ioctl(ifname: [%s]) failed, error: %s", __FUNCTION__, ifr.ifr_name, gs_error_string(gserror));
        gs_can_close(handle);
        return gserror;
    }

    /* Bind the socket to CAN interface */
    struct sockaddr_can addr;
    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(handle->can_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        gs_error_t gserror = gs_error(errno);
        log_error("%s: bind() failed, error: %s", __FUNCTION__, gs_error_string(gserror));
        gs_can_close(handle);
        return gserror;
    }

    *return_handle = handle_id;

    return GS_OK;
}

static gs_error_t gs_can_set_filter_mask(uint8_t hdl, bool extended, uint32_t canMsgId, uint32_t mask, gs_can_rxdata_callback_t rx_callback, void * user_data)
{
    gs_can_handle_t * handle = gs_can_handle(hdl);
    if (handle == NULL) {
        return GS_ERROR_HANDLE;
    }

    if (extended) {
        if ((canMsgId > CAN_EFF_MASK) || (mask > CAN_EFF_MASK)) {
            return GS_ERROR_ARG;
        }
    } else {
        if ((canMsgId > CAN_SFF_MASK) || (mask > CAN_SFF_MASK)) {
            return GS_ERROR_ARG;
        }
    }

    handle->rx_callback = rx_callback;
    handle->user_data = user_data;
    
    struct can_filter filter;
    filter.can_id = canMsgId;
    filter.can_mask = mask;
    if (extended == false) {
        filter.can_mask |= (CAN_EFF_MASK & ~CAN_SFF_MASK);
    }

    if (setsockopt(handle->can_socket, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter)) < 0) {
        gs_error_t gserror = gs_error(errno);
        log_error("%s: setsockopt(id: 0x%x, mask: 0x%x) failed, error: %s", __FUNCTION__, canMsgId, mask, gs_error_string(gserror));
        return gserror;
    }

    return GS_OK;
}

gs_error_t gs_can_set_standard_filter_mask(uint8_t hdl, uint32_t canMsgId, uint32_t mask, gs_can_rxdata_callback_t rx_callback, void * user_data)
{
    return gs_can_set_filter_mask(hdl, false, canMsgId, mask, rx_callback, user_data);
}

gs_error_t gs_can_set_extended_filter_mask(uint8_t hdl, uint32_t canExtMsgId, uint32_t mask, gs_can_rxdata_callback_t rx_callback, void * user_data)
{
    return gs_can_set_filter_mask(hdl, true, canExtMsgId, mask, rx_callback, user_data);
}

gs_error_t gs_can_start(uint8_t hdl)
{
    gs_can_handle_t * handle = gs_can_handle(hdl);
    if (handle == NULL) {
        return GS_ERROR_HANDLE;
    }

    if (handle->rxthread) {
        return GS_OK;
    }

    /* Create receiver thread */
    gs_error_t gserror = gs_thread_create("rxcan", gs_can_rx_thread, GS_TYPES_INT2PTR(hdl), 0, GS_THREAD_PRIORITY_HIGH, 0, &handle->rxthread);
    if (gserror) {
        log_error("s: gs_thread_create() failed, error: %s", gs_error_string(gserror));
        return gserror;
    }

    return GS_OK;
}

gs_error_t gs_can_stop(uint8_t hdl)
{
    gs_can_handle_t * handle = gs_can_handle(hdl);
    if (handle == NULL) {
        return GS_ERROR_HANDLE;
    }

    return GS_ERROR_NOT_IMPLEMENTED;
}

gs_error_t gs_can_error_state(uint8_t hdl, bool * restart_required)
{
    gs_can_handle_t * handle = gs_can_handle(hdl);
    if (handle == NULL) {
        return GS_ERROR_HANDLE;
    }

    if (restart_required) {
        *restart_required = false;
    }

    // missing error state check on CAN layer

    return GS_OK;
}
