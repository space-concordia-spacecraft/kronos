/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#define GS_PARAM_INTERNAL_USE 1

#include <gs/param/internal/rparam.h>
#include <gs/param/internal/types.h>
#include <gs/param/internal/table.h>
#include "../serialize_local.h"
#include <stdio.h>
#include <alloca.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <gs/csp/csp.h>
#include <csp/csp_endian.h>
#include <gs/util/log.h>
#include <gs/util/fletcher.h>
#include <gs/util/check.h>

static gs_error_t gs_rparam_command(uint8_t node, uint32_t timeout_ms, uint8_t action, uint8_t table_id,
                                    const void * payload, size_t payload_size, char reply_ok)
{
    gs_rparam_query_t * query = alloca(RPARAM_QUERY_LENGTH(query, payload_size));
    query->action = action;
    query->table_id = table_id;
    query->length = csp_hton16(payload_size);
    query->checksum = csp_hton16(GS_RPARAM_MAGIC_CHECKSUM); // Ignore checksum
    query->seq = 0;
    query->total = 0;
    memcpy(&query->payload, payload, payload_size);

    /* Run single packet transaction */
    char reply = 0;
    if (csp_transaction2(CSP_PRIO_HIGH, node, GS_CSP_PORT_RPARAM, timeout_ms, query, RPARAM_QUERY_LENGTH(query, payload_size),
                         &reply, 1, CSP_O_CRC32) <= 0) {
        return GS_ERROR_IO;
    }

    return (reply == reply_ok) ? GS_OK : GS_ERROR_UNKNOWN;
}

gs_error_t gs_rparam_copy(uint8_t node, uint32_t timeout_ms, uint8_t from, uint8_t to)
{
    gs_rparam_query_payload_t payload;
    payload.copy.from = from;
    payload.copy.to = to;
    return gs_rparam_command(node, timeout_ms, RPARAM_COPY, from, &payload, sizeof(payload.copy), RPARAM_COPY_OK);
}

static gs_error_t gs_rparam_store(uint8_t node, uint32_t timeout_ms, uint8_t action, char reply_ok, uint8_t table_id,
                                  const char * table, const char * store, const char * slot)
{
    gs_rparam_query_payload_store_t payload;
    memset(&payload, 0, sizeof(payload));
    if (table && (gs_string_empty(table) == false)) {
        GS_STRNCPY(payload.table, table);
    }
    if (store && (gs_string_empty(store) == false)) {
        GS_STRNCPY(payload.store, store);
    }
    if (slot && (gs_string_empty(slot) == false)) {
        GS_STRNCPY(payload.slot, slot);
    }
    return gs_rparam_command(node, timeout_ms, action, table_id, &payload, sizeof(payload), reply_ok);
}

gs_error_t gs_rparam_save_to_store(uint8_t node, uint32_t timeout_ms, uint8_t table_id,
                                   const char * store, const char * slot)
{
    return gs_rparam_store(node, timeout_ms, RPARAM_SAVE_TO_STORE, RPARAM_SAVE_OK, table_id, NULL, store, slot);
}

gs_error_t gs_rparam_load_from_store(uint8_t node, uint32_t timeout_ms, uint8_t table_id,
                                     const char * store, const char * slot)
{
    return gs_rparam_store(node, timeout_ms, RPARAM_LOAD_FROM_STORE, RPARAM_LOAD_OK, table_id, NULL, store, slot);
}

gs_error_t gs_rparam_save(uint8_t node, uint32_t timeout_ms, uint8_t from, uint8_t to)
{
    gs_rparam_query_payload_t payload;
    payload.copy.from = from;
    payload.copy.to = to;
    return gs_rparam_command(node, timeout_ms, RPARAM_SAVE, from, &payload, sizeof(payload.copy), RPARAM_SAVE_OK);
}

gs_error_t gs_rparam_load(uint8_t node, uint32_t timeout_ms, uint8_t from, uint8_t to)
{
    gs_rparam_query_payload_t payload;
    payload.copy.from = from;
    payload.copy.to = to;
    return gs_rparam_command(node, timeout_ms, RPARAM_LOAD, to, &payload, sizeof(payload.copy), RPARAM_LOAD_OK);
}

static gs_error_t update_table(const char * func,
                               gs_param_table_instance_t * tinst,
                               gs_param_table_row_t * rows, unsigned int row_count,
                               uint16_t checksum)
{
    size_t memory_size = gs_param_calc_table_size(rows, row_count);
    if ((tinst->memory == NULL) || (tinst->memory_size < memory_size)) {
        // (re)allocate memory
        if (memory_size == 0) {
            return GS_ERROR_NOT_SUPPORTED;
        }
        memory_size = gs_max(1000U, memory_size);
        void * memory = calloc(1, memory_size);
        if (memory == NULL) {
            return GS_ERROR_ALLOC;
        }

        free(tinst->memory);
        tinst->memory = memory;
        tinst->memory_size = memory_size;
        tinst->flags |= GS_PARAM_TABLE_F_ALLOC_MEMORY;
    } else {
        memset(tinst->memory, 0, tinst->memory_size);
    }

    free((void*)tinst->rows);
    tinst->rows = rows;
    tinst->row_count = row_count;
    tinst->checksum_be = tinst->checksum_le = 0;
    tinst->flags |= GS_PARAM_TABLE_F_ALLOC_ROWS;

    if ((checksum != gs_param_table_checksum_le(tinst)) && (checksum != gs_param_table_checksum_be(tinst))) {
        log_error("%s: table specification has invalid checksum: %u - different from LE: %u and BE: %u",
                  func, checksum, gs_param_table_checksum_le(tinst), gs_param_table_checksum_be(tinst));
        return GS_ERROR_DATA;
    }

    return GS_OK;
}

gs_error_t gs_rparam_load_table_spec(gs_param_table_instance_t * tinst, const char* fname, uint16_t * return_checksum)
{
    GS_CHECK_HANDLE(tinst != NULL);
    GS_CHECK_ARG(fname != NULL);

    FILE * fd = fopen(fname, "r");
    if (fd == NULL) {
        return GS_ERROR_NOT_FOUND;
    }

    struct stat file_stat;
    if (fstat(fileno(fd), &file_stat) != 0) {
        log_error("%s: failed to stat file [%s]", __FUNCTION__, fname);
        fclose(fd);
        return GS_ERROR_IO;
    }

    void * rows = calloc(file_stat.st_size, 1);
    if (rows == NULL) {
        fclose(fd);
        return GS_ERROR_ALLOC;
    }

    uint16_t checksum = 0;
    size_t rs1 = fread(&checksum, 1, sizeof(checksum), fd);
    size_t rs2 = fread(rows, 1, file_stat.st_size, fd);
    fclose(fd);

    const unsigned int single_row_size = sizeof(*tinst->rows);
    const unsigned int all_row_size = (file_stat.st_size - sizeof(checksum));
    const unsigned int row_count = (all_row_size) / single_row_size;
    if ((rs1 != sizeof(checksum)) || (rs2 != all_row_size) || (rs2 != (single_row_size * row_count))) {
        log_error("%s: incomplete/invalid read, expected %u + %u - read %u + %u, single row size: %u", __FUNCTION__,
                  (unsigned int) sizeof(checksum), row_count,
                  (unsigned int) rs1, (unsigned int) rs2, single_row_size);
        free(rows);
        return GS_ERROR_IO;
    }

    gs_error_t error = update_table(__FUNCTION__, tinst, rows, row_count, checksum);

    if (error == GS_OK) {
        if (return_checksum) {
            *return_checksum = checksum;
        }
    }

    return error;
}

gs_error_t gs_rparam_download_table_spec(gs_param_table_instance_t * tinst,
                                         const char * fname,
                                         uint8_t node,
                                         gs_param_table_id_t table_id,
                                         uint32_t timeout_ms,
                                         uint16_t * return_checksum)
{
    csp_conn_t * conn = csp_connect(CSP_PRIO_HIGH, node, GS_CSP_PORT_RPARAM, timeout_ms, CSP_O_CRC32);
    if (conn == NULL) {
        return GS_ERROR_IO;
    }

    /* Allocate outgoing buffer */
    gs_rparam_query_t * query;
    csp_packet_t * packet = csp_buffer_get(RPARAM_QUERY_LENGTH(query, 0));
    if (packet == NULL) {
        csp_close(conn);
        return GS_ERROR_NO_BUFFERS;
    }

    // setup request
    query = (gs_rparam_query_t *) packet->data;
    query->action = RPARAM_TABLE;
    query->table_id = table_id;
    query->length = 0;
    query->checksum = csp_hton16(GS_RPARAM_MAGIC_CHECKSUM); // Ignore checksum
    query->seq = 0;
    query->total = 0;

    packet->length = RPARAM_QUERY_LENGTH(query, 0);
    if (!csp_send(conn, packet, 0)) {
        csp_buffer_free(packet);
        csp_close(conn);
        return GS_ERROR_IO;
    }

    /* Receive remote parameter table, in host byte order
     * Note: This is necessary, because the SFP functions does not know the dataformat
     * and hence cannot be converted server-side. */
    void * dataout = NULL;
    int totalsize = 0;
    int result = csp_sfp_recv(conn, &dataout, &totalsize, timeout_ms);
    csp_close(conn);

    if (result < 0) {
        free(dataout);
        return GS_ERROR_IO;
    }

    gs_param_table_row_t * rows = dataout;
    const uint8_t row_count = totalsize / sizeof(*rows);

    /* Calculate checksum on table (before converting endians!) */
    const uint16_t checksum = gs_fletcher16(rows, totalsize);

    /* Autodetect Endians */
    int sum_first = 0;
    int sum_last = 0;
    for (unsigned int i = 0; i < row_count; i++) {
        sum_first += (rows[i].addr & 0xFF00) >> 8;
        sum_last += rows[i].addr & 0xFF;
    }

    /* Correct endians */
    if (sum_first > sum_last) {
        for (unsigned int i = 0; i < row_count; i++) {
            rows[i].addr = (((rows[i].addr & 0xff00) >> 8) | ((rows[i].addr & 0x00ff) << 8));
        }
    }

    gs_error_t error = update_table(__FUNCTION__, tinst, rows, row_count, checksum);
    if (error == GS_OK) {

        if (return_checksum) {
            *return_checksum = checksum;
        }

        // If filename provided, store table specification to file.
        if (gs_string_empty(fname) == false) {
            FILE * fd = fopen(fname, "w");
            if (fd == NULL) {
                log_error("%s: failed to open/create file: [%s]", __FUNCTION__, fname);
                return GS_ERROR_IO;
            }
            const size_t ws1_size = sizeof(checksum);
            const size_t ws1 = fwrite(&checksum, 1, ws1_size, fd);
            const size_t ws2 = fwrite(rows, 1, totalsize, fd);
            fclose(fd);
            if ((ws1 != ws1_size) || (ws2 != (size_t) totalsize)) {
                log_error("%s: failed to write %u + %d - wrote %u + %u", __FUNCTION__,
                          (unsigned int) sizeof(checksum), totalsize, (unsigned int) ws1, (unsigned int) ws2);
                return GS_ERROR_IO;
            }
        }
    }

    return error;
}

gs_error_t gs_rparam_get_full_table(gs_param_table_instance_t * tinst,
                                    uint8_t node,
                                    gs_param_table_id_t table_id,
                                    uint16_t checksum,
                                    uint32_t timeout_ms)
{
    GS_CHECK_HANDLE(tinst != NULL);
    GS_CHECK_HANDLE(tinst->rows != NULL);
    GS_CHECK_HANDLE(tinst->memory != NULL);

    unsigned int expected_bytes = 0;
    {
        unsigned int param_pos = 0;
        gs_error_t error = gs_param_serialize_full_table(tinst, &param_pos, GS_PARAM_SF_DRY_RUN, NULL, 10000, &expected_bytes);
        if (error) {
            return error;
        }
    }

    gs_rparam_query_t * query;
    csp_packet_t * request = csp_buffer_get(RPARAM_QUERY_LENGTH(query, 0));
    if (request == NULL) {
        return GS_ERROR_NO_BUFFERS;
    }

    csp_conn_t * conn = csp_connect(CSP_PRIO_HIGH, node, GS_CSP_PORT_RPARAM, timeout_ms, CSP_O_CRC32);
    if (!conn) {
        csp_buffer_free(request);
        return GS_ERROR_IO;
    }

    query = (gs_rparam_query_t *) request->data;
    query->action = RPARAM_GET;
    query->table_id = table_id;
    query->length = 0; // == get full table
    query->checksum = csp_hton16(checksum);
    query->seq = 0;
    query->total = 0;

    request->length = RPARAM_QUERY_LENGTH(query, 0);
    if (!csp_send(conn, request, timeout_ms)) {
        csp_buffer_free(request);
        csp_close(conn);
        return GS_ERROR_IO;
    }

    csp_packet_t * reply;
    gs_error_t error = GS_OK;
    unsigned int total_bytes = 0;
    while ((reply = csp_read(conn, timeout_ms)) != NULL) {

        /* We have a reply */
        query = (void *) reply->data;
        const uint16_t qlength = csp_ntoh16(query->length);
        total_bytes += qlength;
        const uint16_t seq = csp_ntoh16(query->seq);
        const uint16_t total = csp_ntoh16(query->total);

        if (query->action == RPARAM_REPLY) {
            error = gs_param_deserialize(tinst, query->payload.packed, qlength, F_FROM_BIG_ENDIAN);
        }
        csp_buffer_free(reply);

        if (error || (seq >= total)) {
            break;
        }
    }

    if (reply == NULL) {
        error = GS_ERROR_TIMEOUT;
    }

    if ((error == GS_OK) && (expected_bytes != total_bytes)) {
        log_warning("%s: expected %u != received %u bytes", __FUNCTION__, expected_bytes, total_bytes);
        error = GS_ERROR_DATA;
    }

    csp_close(conn);

    return error;
}

gs_error_t gs_rparam_get_array(uint8_t node,
                               gs_param_table_id_t table_id,
                               uint16_t addr,
                               gs_param_type_t type,
                               uint16_t checksum,
                               uint32_t timeout_ms,
                               void * value,
                               size_t value_element_size,
                               size_t array_size)
{
    /* Calculate length */
    gs_rparam_query_t * query;
    const size_t query_payload_size = sizeof(query->payload.addr[0]) * array_size;
    const size_t query_size = RPARAM_QUERY_LENGTH(query, query_payload_size);
    const size_t reply_payload_element_size = value_element_size + sizeof(query->payload.addr[0]);
    const size_t reply_payload_size = reply_payload_element_size * array_size;
    const size_t reply_size = RPARAM_QUERY_LENGTH(query, reply_payload_size);

    query = alloca(reply_size);
    query->action = RPARAM_GET;
    query->table_id = table_id;
    query->checksum = csp_hton16(checksum);
    query->seq = 0;
    query->total = 0;
    for(unsigned int i = 0; i < array_size; i++) {
        query->payload.addr[i] = csp_hton16(addr + (value_element_size * i));
    }
    query->length = csp_hton16(query_payload_size);

    /* Run single packet transaction */
    if (csp_transaction2(CSP_PRIO_HIGH, node, GS_CSP_PORT_RPARAM, timeout_ms, query, query_size, query, reply_size, CSP_O_CRC32) <= 0) {
        return GS_ERROR_IO;
    }

    /* We have a reply */
    query->length = csp_ntoh16(query->length);

    if (query->length != reply_payload_size) {
        log_warning("%s: Invalid reply size %u - expected %u", __FUNCTION__, query->length, (unsigned int) reply_payload_size);
        return GS_ERROR_DATA;
    }

    /* Read address */
    query->payload.addr[0] = csp_betoh16(query->payload.addr[0]);
    if (query->payload.addr[0] != addr) {
        log_warning("%s: Invalid address in reply %u", __FUNCTION__, query->payload.addr[0]);
        return GS_ERROR_DATA;
    }

    /* Read value */
    for(unsigned int i = 0; i < array_size; i++) {
        memcpy(((uint8_t *)value + (i * value_element_size)),
                &query->payload.packed[sizeof(query->payload.addr[0]) + (i * reply_payload_element_size)],
                value_element_size);
        gs_param_betoh(type, (uint8_t *)value + (value_element_size * i));
    }

    return GS_OK;
}

gs_error_t gs_rparam_get(uint8_t node,
                         gs_param_table_id_t table_id,
                         uint16_t addr,
                         gs_param_type_t type,
                         uint16_t checksum,
                         uint32_t timeout_ms,
                         void * value,
                         size_t value_element_size)
{
    return gs_rparam_get_array(node, table_id, addr, type, checksum, timeout_ms, value, value_element_size, 1);
}

gs_error_t gs_rparam_set(uint8_t node,
                         gs_param_table_id_t table_id,
                         uint16_t addr,
                         gs_param_type_t type,
                         uint16_t checksum,
                         uint32_t timeout_ms,
                         const void * value,
                         size_t value_size)
{
    /* Calculate length */
    gs_rparam_query_t * query;
    const size_t payload_size = (value_size + sizeof(query->payload.addr[0]));
    const size_t query_size = RPARAM_QUERY_LENGTH(query, payload_size);

    query = alloca(query_size);
    query->action = RPARAM_SET;
    query->table_id = table_id;
    query->seq = 0;
    query->total = 0;
    query->checksum = csp_hton16(checksum);

    /* Actual set query */
    unsigned int bytes = 0;
    gs_error_t error = gs_param_serialize_item_direct(type, value_size, addr, value, F_TO_BIG_ENDIAN, query->payload.packed, payload_size, &bytes);
    if (error) {
        return error;
    }

    /* Add to query */
    query->length = csp_hton16(bytes);

    /* Run single packet transaction */
    if (csp_transaction2(CSP_PRIO_HIGH, node, GS_CSP_PORT_RPARAM, timeout_ms, query, query_size, query, 1, CSP_O_CRC32) <= 0) {
        return GS_ERROR_IO;
    }

    /* We have a reply */
    query->length = csp_ntoh16(query->length);

    if ((query->action != RPARAM_SET_OK) || (query->length != bytes)) {
        log_warning("%s: Invalid reply: size %u - expected %u, action %u - expected %u",
                    __FUNCTION__, query->length, bytes, query->action, RPARAM_SET_OK);
        return GS_ERROR_DATA;
    }

    return GS_OK;
}

gs_error_t gs_rparam_query_get_value(gs_param_table_instance_t * tinst, const char* param_name, uint16_t param_no, void* val_p, size_t val_size)
{
    const gs_param_table_row_t * t = gs_param_row_by_name(param_name, tinst->rows, tinst->row_count);
    if (t == NULL) {
        return GS_ERROR_NOT_FOUND;
    }

    if (val_size < t->size) {
        return GS_ERROR_OVERFLOW;
    }

    return gs_param_get(tinst, t->addr + (param_no * t->size), t->type, val_p, t->size, 0);
}
