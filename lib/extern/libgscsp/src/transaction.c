/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/csp/csp.h>
#include <gs/util/check.h>

gs_error_t gs_csp_transaction_persistent(csp_conn_t * conn, uint32_t timeout,  const void * tx_buf,
                                         size_t tx_len, void * rx_buf, size_t rx_max_len, size_t * rx_len)
{
    GS_CHECK_HANDLE(conn != NULL);

    size_t size = (rx_max_len > tx_len) ? rx_max_len : tx_len;
    csp_packet_t * packet = csp_buffer_get(size);
    if (packet == NULL) {
        return GS_ERROR_ALLOC;
    }

    /* Copy the request */
    if (tx_len > 0 && tx_buf != NULL) {
        memcpy(packet->data, tx_buf, tx_len);
    }

    packet->length = tx_len;

    if (!csp_send(conn, packet, timeout)) {
        csp_buffer_free(packet);
        return GS_ERROR_IO;
    }

    /* If no reply is expected, return now */
    if (rx_max_len == 0) {
        return GS_OK;
    }

    packet = csp_read(conn, timeout);
    if (packet == NULL) {
        return GS_ERROR_IO;
    }

    gs_error_t return_val;
    if (rx_max_len >= packet->length) {
        size = packet->length;
        return_val = GS_OK;
    } else {
        csp_log_error("Reply length %u, buffer only %u", packet->length, (unsigned int) rx_max_len);
        size = rx_max_len;
        return_val = GS_ERROR_OVERFLOW;
    }
    memcpy(rx_buf, packet->data, size);
    *rx_len = packet->length;
    csp_buffer_free(packet);
    return return_val;
}

gs_error_t gs_csp_transaction2(uint8_t prio, uint8_t dest, uint8_t port, uint32_t timeout, const void * tx_buf,
                               size_t tx_len, void * rx_buf, size_t rx_max_len, size_t * rx_len, uint32_t opts)
{
    csp_conn_t * conn = csp_connect(prio, dest, port, 0, opts);
    if (conn == NULL) {
        return GS_ERROR_HANDLE;
    }

    gs_error_t res = gs_csp_transaction_persistent(conn, timeout, tx_buf, tx_len, rx_buf, rx_max_len, rx_len);

    csp_close(conn);

    return res;
}
