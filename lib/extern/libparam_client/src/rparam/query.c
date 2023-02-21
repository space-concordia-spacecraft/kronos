/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#define GS_PARAM_INTERNAL_USE 1

#include "query.h"
#include <gs/param/internal/rparam.h>
#include <gs/param/internal/types.h>
#include <gs/param/internal/table.h>
#include "../serialize_local.h"
#include <gs/csp/csp.h>
#include <csp/csp_endian.h>

bool gs_rparam_query_is_set(gs_rparam_query_handle_t * handle)
{
    return (handle->length > 0);
}

void gs_rparam_query_reset(gs_rparam_query_handle_t * handle)
{
    handle->action = RPARAM_GET;
    handle->length = 0;
    handle->get_size = 0;
}

void gs_rparam_query_set_quiet(gs_rparam_query_handle_t * handle, bool quiet)
{
    handle->quiet = quiet;
}

gs_error_t gs_rparam_query_get(gs_rparam_query_handle_t * handle, gs_param_table_instance_t * tinst, const char* param_name)
{
    if (tinst->rows == NULL)
        return GS_ERROR_NOT_FOUND;

    /* Ensure correct header */
    if (handle->action != RPARAM_GET) {
        gs_rparam_query_reset(handle);
        handle->action = RPARAM_GET;
    }

    char shortname[GS_PARAM_MAX_NAME + 20];
    uint8_t array_index = 0;
    bool is_array;
    if (gs_param_parse_name_and_array_index(param_name, shortname, sizeof(shortname), &array_index, &is_array)) {
        return GS_ERROR_ARG;
    }

    const gs_param_table_row_t * param = gs_param_row_by_name(shortname, tinst->rows, tinst->row_count);
    if (param == NULL) {
        return GS_ERROR_NOT_FOUND;
    }

    if (array_index >= GS_PARAM_ARRAY_SIZE(param)) {
        return GS_ERROR_RANGE;
    }

    unsigned int start;
    unsigned int end;
    if (is_array) {
        start = array_index;
        end = start + 1;
    } else {
        start = 0;
        end = GS_PARAM_ARRAY_SIZE(param);
    }
    for (unsigned int i = start; i < end; ++i) {

        /* Size check */
        if (handle->get_size + param->size + sizeof(uint16_t) > GS_RPARAM_QUERY_MAX_PAYLOAD) {
            return GS_ERROR_OVERFLOW;
        }

        /* Add to query */
        handle->payload.addr[handle->length/2] = param->addr + (param->size * i);
        handle->length += sizeof(uint16_t);
        handle->get_size += param->size + sizeof(uint16_t);
    }

    return GS_OK;
}

gs_error_t gs_rparam_query_set(gs_rparam_query_handle_t * handle, gs_param_table_instance_t * tinst, const char* param_name, const char * const * values, uint8_t value_count)
{
    /* Ensure correct header */
    if (handle->action != RPARAM_SET) {
        gs_rparam_query_reset(handle);
        handle->action = RPARAM_SET;
    }

    char shortname[GS_PARAM_MAX_NAME + 20];
    uint8_t array_index = 0;
    if (gs_param_parse_name_and_array_index(param_name, shortname, sizeof(shortname), &array_index, NULL)) {
        return GS_ERROR_ARG;
    }

    const gs_param_table_row_t * param = gs_param_row_by_name(shortname, tinst->rows, tinst->row_count);
    if (param == NULL) {
        return GS_ERROR_NOT_FOUND;
    }

    if (array_index >= GS_PARAM_ARRAY_SIZE(param)) {
        return GS_ERROR_RANGE;
    }

    for (unsigned int i = 0; i < value_count; i++) {

        /* Parse input */
        uint8_t value[param->size];
        gs_error_t error = gs_param_from_string(param, values[i], value);
        if (error) {
            return error;
        }

        unsigned int bytes = 0;
        error = gs_param_serialize_item(param, param->addr + (param->size * (array_index + i)), value, F_TO_BIG_ENDIAN,
                                        &handle->payload.packed[handle->length], GS_RPARAM_QUERY_MAX_PAYLOAD - handle->length, &bytes);
        if (error) {
            return error;
        }
        handle->length += bytes;
    }

    return GS_OK;
}

gs_error_t gs_rparam_query_send(gs_rparam_query_handle_t * handle, gs_param_table_instance_t * tinst)
{
    if (tinst->rows == NULL) {
        return GS_ERROR_NOT_FOUND;
    }

    if (handle->length == 0) {
        return GS_ERROR_NO_DATA;
    }

    gs_rparam_query_t * query;

    /* Allocate outgoing buffer */
    csp_packet_t * packet = csp_buffer_get(RPARAM_QUERY_LENGTH(query, GS_RPARAM_QUERY_MAX_PAYLOAD));
    if (packet == NULL) {
        return GS_ERROR_NO_BUFFERS;
    }

    csp_conn_t * conn = csp_connect(CSP_PRIO_HIGH, handle->node, GS_CSP_PORT_RPARAM, handle->timeout_ms, CSP_O_CRC32);
    if (conn == NULL) {
        csp_buffer_free(packet);
        return GS_ERROR_IO;
    }

    query = (gs_rparam_query_t *) packet->data;

    query->action = handle->action;
    query->table_id = handle->table_id;
    query->seq = 0;
    query->total = 0;
    query->length = csp_hton16(handle->length);
    query->checksum = csp_hton16(handle->checksum);

    /* Copy payload to message */
    packet->length = RPARAM_QUERY_LENGTH(query, handle->length);
    memcpy(&query->payload, &handle->payload, handle->length);

    /* Deal with endianness */
    if (handle->action == RPARAM_GET) {
        for (unsigned int i = 0; i < (handle->length/2); i++) {
            query->payload.addr[i] = csp_hton16(query->payload.addr[i]);
        }
    }

    /* Send packet */
    if (!csp_send(conn, packet, 0)) {
        csp_buffer_free(packet);
        csp_close(conn);
        return GS_ERROR_IO;
    }

    /* Read reply */
    packet = csp_read(conn, handle->timeout_ms);
    if (packet == NULL) {
        csp_close(conn);
        return GS_ERROR_TIMEOUT;
    }

    if (packet->length <= 1) {
        gs_error_t error = GS_OK;
        if (packet->length == 1) {
            if (packet->data[0] == RPARAM_ERROR) {
                error = GS_ERROR_DATA;
            }
        } else {
            error = GS_ERROR_NO_DATA;
        }
        csp_buffer_free(packet);
        csp_close(conn);
        return error;
    }

    /* We have a reply */
    gs_rparam_query_t * reply = (gs_rparam_query_t *) packet->data;
    reply->length = csp_ntoh16(reply->length);

    gs_error_t error;
    if (reply->action == RPARAM_REPLY) {
        error = gs_param_deserialize(tinst, reply->payload.packed, reply->length, F_FROM_BIG_ENDIAN);
    } else {
        error = GS_ERROR_TYPE;
    }

    csp_buffer_free(packet);
    csp_close(conn);
    return error;
}
