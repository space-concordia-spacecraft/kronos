/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <math.h>
#include <stdlib.h>
#include <limits.h>

#include <gs/csp/error.h>

#include <csp/csp.h>
#include <csp/csp_cmp.h>
#include <csp/csp_endian.h>

#include "../lib/libcsp/src/csp_conn.h"

#include <gs/util/string.h>
#include <gs/util/gosh/command.h>
#include <gs/util/hexdump.h>
#include <gs/util/base16.h>
#include <gs/util/log.h>
#include <gs/util/clock.h>
#include <gs/util/time.h>

static gs_error_t parse_node_timeout(gs_command_context_t *ctx, int node_index, uint8_t * node, int timeout_index, uint32_t * timeout)
{
    gs_error_t error = GS_OK;
    if (node) {
        *node = csp_get_address();

        if (ctx->argc > node_index) {
            error = gs_string_to_uint8(ctx->argv[node_index], node);
        }
    }

    if (timeout && (error == GS_OK)) {
        *timeout = 1000;

        if (ctx->argc > timeout_index) {
            error = gs_string_to_uint32(ctx->argv[timeout_index], timeout);
        }
    }

    return error;
}

static int cmd_ping(gs_command_context_t *ctx)
{
    uint8_t node;
    uint32_t timeout;
    if (parse_node_timeout(ctx, 1, &node, 2, &timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }

    uint32_t size = 1;
    if ((ctx->argc > 3) && (gs_string_to_uint32(ctx->argv[3], &size) != GS_OK)) {
        return GS_ERROR_ARG;
    }

    uint32_t options = CSP_O_NONE;
    if (ctx->argc > 4) {
        const char * features = ctx->argv[4];
        if (strchr(features, 'r')) {
            options |= CSP_O_RDP;
        }
        if (strchr(features, 'x')) {
            options |= CSP_O_XTEA;
        }
        if (strchr(features, 'h')) {
            options |= CSP_O_HMAC;
        }
        if (strchr(features, 'c')) {
            options |= CSP_O_CRC32;
        }
        if (strchr(features, 'C')) {
            options |= CSP_O_NOCRC32;
        }
    }

    uint32_t max_count = 1;
    if ((ctx->argc > 5) && (gs_string_to_uint32(ctx->argv[5], &max_count) != GS_OK)) {
        return GS_ERROR_ARG;
    }
    if (max_count < 1) {
        return GS_ERROR_RANGE;
    }

    fprintf(ctx->out, "Ping node %u, timeout %" PRIu32 ", size %" PRIu32 ": options: 0x%" PRIx32 " ... ", node, timeout, size, options);

    bool ping_timeout = false;
    uint32_t count = 0;
    const uint64_t start = gs_clock_get_nsec();
    while (count < max_count) {
        ++count;
        if (csp_ping(node, timeout, size, options) < 0) {
            ping_timeout = true;
            break;
        }
        if (gs_command_wait_any_key(ctx, 0)) {
            break;
        }
    }
    const uint64_t stop = gs_clock_get_nsec();
    float elapsed = (((float)(stop - start)) / 1E6);

    if (ping_timeout) {
        fprintf(ctx->out, "timeout after %.03f ms\r\n", elapsed);
        return GS_ERROR_TIMEOUT;
    }

    const char * avg_str = "";
    if (count > 1) {
        elapsed /= ((float)count);
        avg_str = "avg. ";
    }

    fprintf(ctx->out, "%sreply in %.03f ms\r\n", avg_str, elapsed);

    return GS_OK;
}

static int cmd_ps(gs_command_context_t *ctx)
{
    uint8_t node;
    uint32_t timeout;
    if (parse_node_timeout(ctx, 1, &node, 2, &timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }

    csp_ps(node, timeout);

    return GS_OK;
}

static int cmd_memfree(gs_command_context_t *ctx)
{
    uint8_t node;
    uint32_t timeout;
    if (parse_node_timeout(ctx, 1, &node, 2, &timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }

    uint32_t size;
    int res = gs_csp_error(csp_get_memfree(node, timeout, &size));
    if (res == GS_OK) {
        fprintf(ctx->out, "Free Memory at node %u is %"PRIu32" bytes\r\n", node, size);
    }
    return res;
}

static int cmd_reboot(gs_command_context_t *ctx)
{
    uint8_t node;
    if (parse_node_timeout(ctx, 1, &node, 0, NULL) != GS_OK) {
        return GS_ERROR_ARG;
    }

    csp_reboot(node);

    return GS_OK;
}

static int cmd_shutdown(gs_command_context_t *ctx)
{
    uint8_t node;
    if (parse_node_timeout(ctx, 1, &node, 0, NULL) != GS_OK) {
        return GS_ERROR_ARG;
    }

    csp_shutdown(node);

    return GS_OK;
}

static int cmd_buf_free(gs_command_context_t *ctx)
{
    uint8_t node;
    uint32_t timeout;
    if (parse_node_timeout(ctx, 1, &node, 2, &timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }

    uint32_t size;
    int res = gs_csp_error(csp_get_buf_free(node, timeout, &size));
    if (res == GS_OK) {
        fprintf(ctx->out, "Free buffers at node %u is %"PRIu32"\r\n", node, size);
    }
    return res;
}

static int cmd_uptime(gs_command_context_t *ctx)
{
    uint8_t node;
    uint32_t timeout;
    if (parse_node_timeout(ctx, 1, &node, 2, &timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }

    uint32_t uptime;
    int res = gs_csp_error(csp_get_uptime(node, timeout, &uptime));
    if (res == GS_OK) {
        char buf[50];
        gs_time_to_string(uptime, buf, sizeof(buf));
        fprintf(ctx->out, "Uptime of node %u is %s\r\n", node, buf);
    }
    return res;
}

static bool csp_rtable_print_route(void * ctx, uint8_t address, uint8_t mask, const csp_route_t * route)
{
    if (route->via == CSP_NO_VIA_ADDRESS) {
        fprintf((FILE*) ctx, "%2u/%u %s\r\n", address, mask, route->iface->name);
    } else {
        fprintf((FILE*) ctx, "%2u/%u %s %u\r\n", address, mask, route->iface->name, route->via);
    }
    return true;
}

static int cmd_csp_rtable_print_table(gs_command_context_t *ctx)
{
    csp_rtable_iterate(csp_rtable_print_route, ctx->out);
    return GS_OK;
}

static int cmd_csp_iflist_print_interfaces(gs_command_context_t *ctx)
{
    csp_iface_t * i = csp_iflist_get();
    char txbuf[25], rxbuf[25];

    while (i) {
        csp_bytesize(txbuf, sizeof(txbuf), i->txbytes);
        csp_bytesize(rxbuf, sizeof(rxbuf), i->rxbytes);
        fprintf(ctx->out,
                "%-10s tx: %05"PRIu32" rx: %05"PRIu32" txe: %05"PRIu32" rxe: %05"PRIu32"\r\n"
                "           drop: %05"PRIu32" autherr: %05"PRIu32 " frame: %05"PRIu32"\r\n"
                "           txb: %"PRIu32" (%s) rxb: %"PRIu32" (%s) MTU: %u\r\n\r\n",
                i->name, i->tx, i->rx, i->tx_error, i->rx_error, i->drop,
               i->autherr, i->frame, i->txbytes, txbuf, i->rxbytes, rxbuf, i->mtu);
        i = i->next;
    }
    return GS_OK;
}

static int cmd_csp_conn_print_table(gs_command_context_t *ctx)
{
    size_t size;
    const csp_conn_t * conn = csp_conn_get_array(&size);
    for (unsigned int i = 0; i < size; ++i, ++conn) {
        fprintf(ctx->out, "[%02u %p] S:%u, %u:%u -> %u:%u, sock: %p\r\n",
                i, conn, conn->state,
                conn->idin.src, conn->idin.sport,
                conn->idin.dst, conn->idin.dport,
                conn->socket);
#if (CSP_USE_RDP)
        if (conn->idin.flags & CSP_FRDP) {
            fprintf(ctx->out, "\tRDP: S:%d (closed by 0x%x), rcv %"PRIu16", snd %"PRIu16", win %"PRIu32", timeout %"PRIu32"\r\n",
                    conn->rdp.state, conn->rdp.closed_by, conn->rdp.rcv_cur, conn->rdp.snd_una, conn->rdp.window_size, conn->rdp.conn_timeout);
}
#endif
    }

    return GS_OK;
}

#if (CSP_USE_RDP)
static int cmd_csp_rdp_set_opt(gs_command_context_t *ctx)
{
    uint32_t window_size;
    if (gs_string_to_uint32(ctx->argv[1], &window_size) != GS_OK) {
        return GS_ERROR_ARG;
    }
    uint32_t conn_timeout;
    if (gs_string_to_uint32(ctx->argv[2], &conn_timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }
    uint32_t packet_timeout;
    if (gs_string_to_uint32(ctx->argv[3], &packet_timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }
    uint32_t delayed_acks;
    if (gs_string_to_uint32(ctx->argv[4], &delayed_acks) != GS_OK) {
        return GS_ERROR_ARG;
    }
    uint32_t ack_timeout;
    if (gs_string_to_uint32(ctx->argv[5], &ack_timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }
    uint32_t ack_delay_count;
    if (gs_string_to_uint32(ctx->argv[6], &ack_delay_count) != GS_OK) {
        return GS_ERROR_ARG;
    }

    fprintf(ctx->out, "Setting arguments to: window size %" PRIu32 ", conn timeout %" PRIu32 ", packet timeout %" PRIu32 ", delayed acks %" PRIu32 ", ack timeout %" PRIu32 ", ack delay count %" PRIu32 "\r\n",
           window_size, conn_timeout, packet_timeout, delayed_acks, ack_timeout, ack_delay_count);

    csp_rdp_set_opt(window_size, conn_timeout, packet_timeout, delayed_acks, ack_timeout, ack_delay_count);

    return GS_OK;
}
#endif

static int cmd_cmp_ident(gs_command_context_t *ctx)
{
    uint8_t node;
    uint32_t timeout;
    if (parse_node_timeout(ctx, 1, &node, 2, &timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }

    struct csp_cmp_message msg;
    memset(&msg.ident, 0, sizeof(msg.ident)); // valgrind

    int res = csp_cmp_ident(node, timeout, &msg);
    if (res != CSP_ERR_NONE) {
        return gs_csp_error(res);
    }

    fprintf(ctx->out, "Hostname: %s\r\n", msg.ident.hostname);
    fprintf(ctx->out, "Model:    %s\r\n", msg.ident.model);
    fprintf(ctx->out, "Revision: %s\r\n", msg.ident.revision);
    fprintf(ctx->out, "Date:     %s\r\n", msg.ident.date);
    fprintf(ctx->out, "Time:     %s\r\n", msg.ident.time);

    return GS_OK;
}

static int cmd_cmp_route_set(gs_command_context_t *ctx)
{
    uint8_t node;
    uint32_t timeout;
    if (parse_node_timeout(ctx, 1, &node, 2, &timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }

    struct csp_cmp_message msg;
    memset(&msg.route_set, 0, sizeof(msg.route_set)); // valgrind

    if (gs_string_to_uint8(ctx->argv[3], &msg.route_set.dest_node) != GS_OK) {
        return GS_ERROR_ARG;
    }

    if (gs_string_to_uint8(ctx->argv[4], &msg.route_set.next_hop_via) != GS_OK) {
        return GS_ERROR_ARG;
    }

    if (strnlen(ctx->argv[5], sizeof(msg.route_set.interface)) >= sizeof(msg.route_set.interface)) {
        return GS_ERROR_ARG;
    }
    strcpy(msg.route_set.interface, ctx->argv[5]);

    fprintf(ctx->out, "Set route on %u: dest_node: %u, via: %u, interface %s\r\n",
            node, msg.route_set.dest_node, msg.route_set.next_hop_via, msg.route_set.interface);

    return gs_csp_error(csp_cmp_route_set(node, timeout, &msg));
}

static int cmd_cmp_ifc(gs_command_context_t *ctx)
{
    uint8_t node;
    uint32_t timeout;
    if (parse_node_timeout(ctx, 1, &node, 3, &timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }

    struct csp_cmp_message msg;
    memset(&msg.if_stats, 0, sizeof(msg.if_stats)); // valgrind

    if (strnlen(ctx->argv[2], sizeof(msg.if_stats.interface)) >= sizeof(msg.if_stats.interface)) {
        return GS_ERROR_ARG;
    }
    strcpy(msg.if_stats.interface, ctx->argv[2]);

    int ret = csp_cmp_if_stats(node, timeout, &msg);
    if (ret != CSP_ERR_NONE) {
        return gs_csp_error(ret);
    }

    msg.if_stats.tx = csp_ntoh32(msg.if_stats.tx);
    msg.if_stats.rx = csp_ntoh32(msg.if_stats.rx);
    msg.if_stats.tx_error = csp_ntoh32(msg.if_stats.tx_error);
    msg.if_stats.rx_error = csp_ntoh32(msg.if_stats.rx_error);
    msg.if_stats.drop = csp_ntoh32(msg.if_stats.drop);
    msg.if_stats.autherr = csp_ntoh32(msg.if_stats.autherr);
    msg.if_stats.frame = csp_ntoh32(msg.if_stats.frame);
    msg.if_stats.txbytes = csp_ntoh32(msg.if_stats.txbytes);
    msg.if_stats.rxbytes = csp_ntoh32(msg.if_stats.rxbytes);
    msg.if_stats.irq = csp_ntoh32(msg.if_stats.irq);

    fprintf(ctx->out, "%-5s   tx: %05"PRIu32" rx: %05"PRIu32" txe: %05"PRIu32" rxe: %05"PRIu32"\r\n"
           "        drop: %05"PRIu32" autherr: %05"PRIu32 " frame: %05"PRIu32"\r\n"
           "        txb: %"PRIu32" rxb: %"PRIu32"\r\n\r\n",
           msg.if_stats.interface, msg.if_stats.tx, msg.if_stats.rx, msg.if_stats.tx_error, msg.if_stats.rx_error, msg.if_stats.drop,
           msg.if_stats.autherr, msg.if_stats.frame, msg.if_stats.txbytes, msg.if_stats.rxbytes);

    return GS_OK;
}

static int cmd_cmp_peek(gs_command_context_t *ctx)
{
    uint8_t node;
    uint32_t timeout;
    if (parse_node_timeout(ctx, 1, &node, 4, &timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }

    uint32_t addr;
    if (gs_string_to_uint32(ctx->argv[2], &addr)) {
        return GS_ERROR_ARG;
    }

    struct csp_cmp_message msg;
    memset(&msg.peek, 0, sizeof(msg.peek));
    msg.peek.addr = csp_hton32(addr);

    if (gs_string_to_uint8(ctx->argv[3], &msg.peek.len)) {
        return GS_ERROR_ARG;
    }
    if (msg.peek.len > sizeof(msg.peek.data)) {
        return GS_ERROR_RANGE;
    }

    int ret = csp_cmp_peek(node, timeout, &msg);
    if (ret != CSP_ERR_NONE) {
        return gs_csp_error(ret);
    }

    gs_hexdump_to_stream(msg.peek.data, msg.peek.len, GS_TYPES_UINT2PTR(addr), ctx->out);

    return GS_OK;
}

static int cmd_cmp_poke(gs_command_context_t *ctx)
{
    uint8_t node;
    uint32_t timeout;
    if (parse_node_timeout(ctx, 1, &node, 4, &timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }

    uint32_t addr;
    if (gs_string_to_uint32(ctx->argv[2], &addr)) {
        return GS_ERROR_ARG;
    }
    struct csp_cmp_message msg;
    memset(&msg.poke, 0, sizeof(msg.poke));
    msg.poke.addr = csp_hton32(addr);

    if (base16_decoded_max_len(ctx->argv[3]) > sizeof(msg.poke.data)) {
        return GS_ERROR_RANGE;
    }

    int len = base16_decode(ctx->argv[3], (uint8_t*) msg.poke.data);
    if (len <= 0) {
        return GS_ERROR_ARG;
    }
    msg.poke.len = len;

    return gs_csp_error(csp_cmp_poke(node, timeout, &msg));
}

static int cmd_cmp_clock(gs_command_context_t *ctx, uint32_t node, uint32_t timeout, const gs_timestamp_t * set)
{
    char tbuf[GS_CLOCK_ISO8601_BUFFER_LENGTH];
    struct csp_cmp_message msg;
    memset(&msg, 0, sizeof(msg));

    if (set) {
        gs_clock_to_iso8601_string(set, tbuf, sizeof(tbuf));
        fprintf(ctx->out, "Set time: %s (%" PRIu32 ".%09" PRIu32 " sec)\r\n", tbuf, set->tv_sec, set->tv_nsec);
        msg.clock.tv_sec = csp_hton32(set->tv_sec);
        msg.clock.tv_nsec = csp_hton32(set->tv_nsec);
    }

    gs_timestamp_t t1, t2;
    gs_clock_get_time(&t1);
    int ret = csp_cmp_clock(node, timeout, &msg);
    if (ret != CSP_ERR_NONE) {
        return gs_csp_error(ret);
    }
    gs_clock_get_time(&t2);

    /* Calculate round-trip time */
    const int64_t rtt = ((uint64_t)t2.tv_sec * 1000000000 + t2.tv_nsec) - ((uint64_t)t1.tv_sec * 1000000000 + t1.tv_nsec);

    gs_timestamp_t timestamp;
    timestamp.tv_sec = csp_ntoh32(msg.clock.tv_sec);
    timestamp.tv_nsec = csp_ntoh32(msg.clock.tv_nsec);

    gs_clock_to_iso8601_string(&timestamp, tbuf, sizeof(tbuf));
    fprintf(ctx->out, "Get time: %s (%" PRIu32 ".%09" PRIu32 " sec)\r\n", tbuf, timestamp.tv_sec, timestamp.tv_nsec);

    /* Calculate time difference to local clock. This takes the round-trip
     * into account, but assumes a symmetrical link */
    const int64_t remote = (uint64_t)timestamp.tv_sec * 1000000000 + timestamp.tv_nsec;
    const int64_t local = (uint64_t)t1.tv_sec * 1000000000 + t1.tv_nsec + rtt / 2;

    const double diff = (remote - local) / 1000000.0;
    fprintf(ctx->out, "Remote is %f ms %s local time\r\n", fabs(diff), diff > 0 ? "ahead of" : "behind");

    return GS_OK;
}

static int cmd_cmp_clock_get(gs_command_context_t *ctx)
{
    uint8_t node;
    uint32_t timeout;
    if (parse_node_timeout(ctx, 1, &node, 2, &timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }

    return cmd_cmp_clock(ctx, node, timeout, NULL);
}

static int cmd_cmp_clock_set(gs_command_context_t *ctx)
{
    uint8_t node;
    uint32_t timeout;
    if (parse_node_timeout(ctx, 1, &node, 3, &timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }

    gs_timestamp_t ts;
    if (gs_clock_from_string(ctx->argv[2], &ts) != GS_OK) {
        return GS_ERROR_ARG;
    }

    return cmd_cmp_clock(ctx, node, timeout, &ts);
}

static int cmd_cmp_clock_sync(gs_command_context_t *ctx)
{
    uint8_t node;
    uint32_t timeout;
    if (parse_node_timeout(ctx, 1, &node, 2, &timeout) != GS_OK) {
        return GS_ERROR_ARG;
    }

    gs_timestamp_t ts;
    gs_clock_get_time(&ts);

    return cmd_cmp_clock(ctx, node, timeout, &ts);
}

static const gs_command_t GS_COMMAND_SUB cmp_clock_commands[] = {
    {
        .name = "get",
        .help = "Get clock on <node>",
        .usage = "<node> [timeout]",
        .handler = cmd_cmp_clock_get,
        .mandatory_args = 1,
        .optional_args = 1,
    },
    {
        .name = "set",
        .help = "Set time of <node>",
        .usage = "<node> <sec.nsec|YYYY-MM-DDTHH:MM:SSZ> [timeout]",
        .handler = cmd_cmp_clock_set,
        .mandatory_args = 2,
        .optional_args = 1,
    },
    {
        .name = "sync",
        .help = "Sync/set time of <node> to time of this node",
        .usage = "<node> [timeout]",
        .handler = cmd_cmp_clock_sync,
        .mandatory_args = 1,
        .optional_args = 1,
    }
};

static const gs_command_t GS_COMMAND_SUB cmp_commands[] = {
    {
        .name = "ident",
        .help = "Node id",
        .usage = "[node] [timeout]",
        .handler = cmd_cmp_ident,
        .optional_args = 2,
    },{
        .name = "route_set",
        .help = "Update table",
        .usage = "<node> <timeout> <addr> <via> <ifstr>",
        .handler = cmd_cmp_route_set,
        .mandatory_args = 5,
    },{
        .name = "ifc",
        .help = "Remote IFC",
        .usage = "<node> <interface> [timeout]",
        .handler = cmd_cmp_ifc,
        .mandatory_args = 2,
        .optional_args = 1,
    },{
        .name = "peek",
        .help = "Show remote memory",
        .usage = "<node> <addr> <len> [timeout]",
        .handler = cmd_cmp_peek,
        .mandatory_args = 3,
        .optional_args = 1,
    },{
        .name = "poke",
        .help = "Modify remote memory",
        .usage = "<node> <addr> <base16_data> [timeout]",
        .handler = cmd_cmp_poke,
        .mandatory_args = 3,
        .optional_args = 1,
    },{
        .name = "clock",
        .help = "Get/set clock",
        .chain = GS_COMMAND_INIT_CHAIN(cmp_clock_commands),
    }
};

static const gs_command_t GS_COMMAND_ROOT csp_commands[] = {
    {
        .name = "ping",
        .help = "csp: Ping",
        .usage = "[node] [timeout] [size] [r|x|h|c] [count]",
        .handler = cmd_ping,
        .optional_args = 5,
    },{
        .name = "rps",
        .help = "csp: Remote ps",
        .usage = "[node] [timeout]",
        .handler = cmd_ps,
        .optional_args = 2,
    },{
        .name = "memfree",
        .help = "csp: Memory free",
        .usage = "[node] [timeout]",
        .handler = cmd_memfree,
        .optional_args = 2,
    },{
        .name = "buffree",
        .help = "csp: Buffer free",
        .usage = "[node] [timeout]",
        .handler = cmd_buf_free,
        .optional_args = 2,
    },{
        .name = "reboot",
        .help = "csp: Reboot",
        .usage = "<node>",
        .handler = cmd_reboot,
        .mandatory_args = 1,
    },{
        .name = "shutdown",
        .help = "csp: Shutdown",
        .usage = "<node>",
        .handler = cmd_shutdown,
        .mandatory_args = 1,
    },{
        .name = "uptime",
        .help = "csp: Uptime",
        .usage = "[node] [timeout]",
        .handler = cmd_uptime,
        .optional_args = 2,
    },{
        .name = "cmp",
        .help = "csp: Management",
        .chain = GS_COMMAND_INIT_CHAIN(cmp_commands),
    },
#if (CSP_DEBUG)
    {
        .name = "route",
        .help = "csp: Show routing table",
        .handler = cmd_csp_rtable_print_table,  
        .mandatory_args = GS_COMMAND_NO_ARGS,
    },{
        .name = "ifc",
        .help = "csp: Show interfaces",
        .handler = cmd_csp_iflist_print_interfaces,
        .mandatory_args = GS_COMMAND_NO_ARGS,
    },{
        .name = "conn",
        .help = "csp: Show connection table",
        .handler = cmd_csp_conn_print_table,
        .mandatory_args = GS_COMMAND_NO_ARGS,
    },
#endif
#if (CSP_USE_RDP)
    {
        .name = "rdpopt",
        .help = "csp: Set RDP options",
        .handler = cmd_csp_rdp_set_opt,
        .usage = "<window size> <conn timeout> <packet timeout> <delayed ACKs> <ACK timeout> <ACK delay count>",
        .mandatory_args = 6,
    },
#endif
};

gs_error_t gs_csp_register_commands(void)
{
    return GS_COMMAND_REGISTER(csp_commands);
}
