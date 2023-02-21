/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/csp/service_handler.h>
#include <gs/util/time.h>
#include <gs/util/byteorder.h>
#include <gs/util/drivers/sys/memory.h>

// callback for processing packets on a connection.
typedef void (*csp_service_packet_handler_t)(csp_conn_t * conn, csp_packet_t * packet);

// Process all packets on the connectio and close it when done.
static gs_error_t call_csp_packet_handler(csp_conn_t * conn, csp_service_packet_handler_t handler)
{
    csp_packet_t *packet;
    while ((packet = csp_read(conn, 0))) {
        (handler)(conn, packet);
    }
    csp_close(conn);
    return GS_OK;
}

gs_error_t gs_csp_cmp_service_handler(csp_conn_t * conn)
{
    return call_csp_packet_handler(conn, csp_service_handler);
}

gs_error_t gs_csp_ping_service_handler(csp_conn_t * conn)
{
    return call_csp_packet_handler(conn, csp_service_handler);
}

gs_error_t gs_csp_ps_service_handler(csp_conn_t * conn)
{
    return call_csp_packet_handler(conn, csp_service_handler);
}

static void memfree(csp_conn_t * conn, csp_packet_t * packet)
{
    uint32_t mem_free = 0;

    gs_mem_ram_type_t ram_type = gs_mem_get_ram_default();
    gs_mem_ram_stat_t ram_stat;
    if(gs_mem_get_ram_stat(ram_type, &ram_stat) == GS_OK) {
        mem_free = ram_stat.available;
    }

    mem_free = util_hton32(mem_free);
    memcpy(packet->data, &mem_free, sizeof(mem_free));
    packet->length = sizeof(mem_free);

    if (!csp_send(conn, packet, 0)) {
        csp_buffer_free(packet);
    }
}

gs_error_t gs_csp_mem_free_service_handler(csp_conn_t * conn)
{
    return call_csp_packet_handler(conn, memfree);
}

gs_error_t gs_csp_reboot_service_handler(csp_conn_t * conn)
{
    return call_csp_packet_handler(conn, csp_service_handler);
}

gs_error_t gs_csp_buf_free_service_handler(csp_conn_t * conn)
{
    return call_csp_packet_handler(conn, csp_service_handler);
}

static void uptime(csp_conn_t * conn, csp_packet_t * packet)
{
    uint32_t time = gs_time_uptime();
    time = util_hton32(time);
    memcpy(packet->data, &time, sizeof(time));
    packet->length = sizeof(time);

    if (!csp_send(conn, packet, 0)) {
        csp_buffer_free(packet);
    }
}

gs_error_t gs_csp_uptime_service_handler(csp_conn_t * conn)
{
    return call_csp_packet_handler(conn, uptime);
}
