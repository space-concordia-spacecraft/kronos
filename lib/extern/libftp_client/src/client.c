/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#define GS_FTP_INTERNAL_USE 1

#include <gs/ftp/client.h>
#include <gs/ftp/internal/types.h>

#include <unistd.h>
#include <sys/stat.h>

#include <gs/csp/csp.h>
#include <csp/csp_endian.h>

#include <gs/util/crc32.h>
#include <gs/util/log.h>
#include <gs/util/string.h>
#include <gs/util/check.h>

#define DEFAULT_TIMEOUT     30000
#define DEFAULT_CHUNK_SIZE  185

typedef struct {
    FILE       * fp;
    FILE       * fp_map;
    csp_conn_t * conn;
    uint32_t   timeout;
    char       file_name[GS_FTP_PATH_LENGTH];
    uint32_t   file_size;
    uint32_t   chunks;
    int        chunk_size;
    uint32_t   checksum;
    ftp_status_element_t last_status[GS_FTP_STATUS_CHUNKS];
    uint32_t   last_entries;
    gs_ftp_info_callback_t info_callback;
    void       * info_data;
} gs_ftp_state_t;

typedef struct {
    gs_ftp_backend_type_t backend;
    const char * path;
    uint32_t addr;
    uint32_t size;
} gs_ftp_url_t;

/* Chunk status markers */
static const char * const packet_missing = "-";
static const char * const packet_ok = "+";

static gs_error_t ftp_status_request(gs_ftp_state_t * state);
static gs_error_t ftp_status_reply(gs_ftp_state_t * state);
static gs_error_t ftp_data(gs_ftp_state_t * state, int count);
static gs_error_t ftp_crc(gs_ftp_state_t * state);
static void ftp_done(gs_ftp_state_t * state, unsigned int remove_map);

static gs_error_t ftp_error_to_gs_error(gs_ftp_return_t ret)
{
    switch (ret) {
        case GS_FTP_RET_OK:
            return GS_OK;
        case GS_FTP_RET_NOENT:
            return GS_ERROR_NOT_FOUND;
        case GS_FTP_RET_INVAL:
            return GS_ERROR_ARG;
        case GS_FTP_RET_NOSPC:
            return GS_ERROR_FULL;
        case GS_FTP_RET_IO:
            return GS_ERROR_IO;
        case GS_FTP_RET_FBIG:
            return GS_ERROR_OVERFLOW;
        case GS_FTP_RET_EXISTS:
            return GS_ERROR_EXIST;
        case GS_FTP_RET_NOTSUP:
            return GS_ERROR_NOT_SUPPORTED;
        case GS_FTP_RET_BUSY:
            return GS_ERROR_BUSY;
        case GS_FTP_RET_NOMEM:
            return GS_ERROR_ALLOC;
    }
    return GS_ERROR_UNKNOWN;
}


static int ftp_file_crc32(gs_ftp_state_t * state)
{
    size_t bytes;
    char buf[128];
    uint32_t crc = gs_crc32_init();

    fseek(state->fp, 0, SEEK_SET);
    do {
        bytes = fread(buf, 1, sizeof(buf), state->fp);
        crc = gs_crc32_update(crc, buf, bytes);
    } while (bytes > 0);

    state->checksum = crc = gs_crc32_finalize(crc);

    return 0;
}

/* parse URL's of the form:
   -file://<path_to_file>
   -mem://<addr>[++<length>]
   -<path_to_file>
*/
static gs_error_t ftp_parse_url(const char * url, gs_ftp_url_t * parsed_url)
{
    memset(parsed_url, 0, sizeof(*parsed_url));

    if (strstr(url, "mem://")) {
        char temp_url[strlen(url)+1];
        strcpy(temp_url, url);
        char * saveptr;
        char * token = strtok_r(temp_url + strlen("mem://"), "++", &saveptr);
        if (gs_string_empty(token) || gs_string_to_uint32(token, &parsed_url->addr)) {
            return GS_ERROR_ARG;
        }
        token = strtok_r(NULL, "++\r\n", &saveptr);
        if (token) {
            if (gs_string_to_uint32(token, &parsed_url->size)) {
                return GS_ERROR_ARG;
            }
        }
        parsed_url->backend = GS_FTP_BACKEND_RAM;
        parsed_url->path = "";
        return GS_OK;
    }
    parsed_url->backend = GS_FTP_BACKEND_FILE;
    if (strstr(url, "file://")) {
        parsed_url->path = url + strlen("file://");
    } else {
        parsed_url->path = url;
    }
    return GS_OK;
}

static gs_ftp_backend_type_t gs_ftp_get_backend(const gs_ftp_url_t * url, const gs_ftp_settings_t * settings)
{
    if (url) {
        switch (url->backend) {
            case GS_FTP_BACKEND_RAM:
            case GS_FTP_BACKEND_FAT:
            case GS_FTP_BACKEND_UFFS:
                return url->backend;
            case GS_FTP_BACKEND_FILE:
                break;
        }
    }
    if (settings) {
        switch (settings->mode) {
            case GS_FTP_MODE_GATOSS:
                return GS_FTP_BACKEND_UFFS;
            case GS_FTP_MODE_STANDARD:
                break;
        }
    }
    return GS_FTP_BACKEND_FILE;
}

uint32_t gs_ftp_get_timeout(const gs_ftp_settings_t * settings)
{
    return (settings && settings->timeout) ? settings->timeout : DEFAULT_TIMEOUT;
}
    
uint32_t gs_ftp_get_chunk_size(const gs_ftp_settings_t * settings)
{
    return (settings && settings->chunk_size && (settings->chunk_size <= GS_FTP_MAX_CHUNK_SIZE)) ? settings->chunk_size : DEFAULT_CHUNK_SIZE;
}

uint8_t gs_ftp_get_csp_port(const gs_ftp_settings_t * settings)
{
    if (settings) {
        if (settings->port) {
            return settings->port;
        }
        switch (settings->mode) {
            case GS_FTP_MODE_GATOSS:
                return GS_FTP_CSP_PORT_GATOSS;
            case GS_FTP_MODE_STANDARD:
                break;
        }
    }

    return GS_CSP_PORT_FTP;
}

gs_error_t gs_ftp_upload(const gs_ftp_settings_t * settings, const char * local_url, const char * remote_url,
                         gs_ftp_info_callback_t info_callback, void * info_data)
{
    gs_ftp_url_t local_url_info;
    if (ftp_parse_url(local_url, &local_url_info)) {
        log_error("%s: Invalid local URL: [%s]", __FUNCTION__, local_url);
        return GS_ERROR_ARG;
    }
    if (local_url_info.backend != GS_FTP_BACKEND_FILE) {
        log_error("%s: Not supported local URL: [%s]", __FUNCTION__, local_url);
        return GS_ERROR_NOT_SUPPORTED;
    }

    gs_ftp_url_t remote_url_info;
    if (ftp_parse_url(remote_url, &remote_url_info)) {
        log_error("%s: Invalid remote URL: [%s]", __FUNCTION__, remote_url);
        return GS_ERROR_ARG;
    }

    /* Open file and read size */
    gs_ftp_state_t state = {.fp = NULL,};
    state.fp = fopen(local_url_info.path, "rb");
    if (state.fp == NULL) {
        return GS_ERROR_NOT_FOUND;
    }
    struct stat statbuf;
    stat(local_url_info.path, &statbuf);
    /* Calculate CRC32 */
    ftp_file_crc32(&state);

    state.timeout    = gs_ftp_get_timeout(settings);
    state.chunk_size = gs_ftp_get_chunk_size(settings);
    state.file_size = (uint32_t) statbuf.st_size;
    state.chunks = (state.file_size + state.chunk_size - 1) / state.chunk_size;
    GS_STRNCPY(state.file_name, local_url_info.path);
    state.info_callback = info_callback;
    state.info_data     = info_data;

    if (info_callback) {
        gs_ftp_info_t info = {
            .user_data = info_data,
            .type      = GS_FTP_INFO_FILE,
            .u.file = {
                .size = state.file_size,
                .crc  = state.checksum,
            },
        };
        info_callback(&info);
    }

    /* Assemble upload request */
    ftp_packet_t req;
    req.type = FTP_UPLOAD_REQUEST;
    req.up.chunk_size = csp_hton16(state.chunk_size);
    req.up.size = csp_hton32(state.file_size);
    req.up.crc32 = csp_hton32(state.checksum);
    req.up.mem_addr = csp_hton32(remote_url_info.addr);
    req.up.backend = gs_ftp_get_backend(&remote_url_info, settings);
    GS_STRNCPY(req.up.path, remote_url_info.path);
    int req_length = sizeof(req.type) + sizeof(req.up);

    state.conn = csp_connect(CSP_PRIO_NORM, settings->host, gs_ftp_get_csp_port(settings), gs_ftp_get_timeout(settings),
                             CSP_O_RDP | CSP_O_CRC32);
    if (state.conn == NULL) {
        ftp_done(&state, 0);
        return GS_ERROR_IO;
    }

    ftp_packet_t rep;
    int rep_length = sizeof(rep.type) + sizeof(rep.uprep);
    if (csp_transaction_persistent(state.conn, gs_ftp_get_timeout(settings), &req, req_length, &rep, rep_length) != rep_length) {
        ftp_done(&state, 0);
        return GS_ERROR_TIMEOUT;
    }

    if (rep.type != FTP_UPLOAD_REPLY || rep.uprep.ret != GS_FTP_RET_OK) {
        ftp_done(&state, 0);
        return ftp_error_to_gs_error(rep.uprep.ret);
    }

    /* Handle data stage */
    gs_error_t status = ftp_status_request(&state);
    if (status) {
        ftp_done(&state, 0);
        return status;
    }
    status = ftp_data(&state, 0);
    if (status) {
        ftp_done(&state, 0);
        return status;
    }
    status = ftp_crc(&state);
    if (status) {
        ftp_done(&state, 0);
        return status;
    }

    ftp_done(&state, 0);
    return GS_OK;
}


gs_error_t gs_ftp_download(const gs_ftp_settings_t * settings, const char * local_url,  const char * remote_url,
                           gs_ftp_info_callback_t info_callback, void * info_data)
{
    gs_ftp_url_t local_url_info;
    if (ftp_parse_url(local_url, &local_url_info)) {
        log_error("%s: Invalid local URL: [%s]", __FUNCTION__, local_url);
        return GS_ERROR_ARG;
    }
    if (local_url_info.backend != GS_FTP_BACKEND_FILE) {
        log_error("%s: Not supported local URL: [%s]", __FUNCTION__, local_url);
        return GS_ERROR_NOT_SUPPORTED;
    }

    gs_ftp_url_t remote_url_info;
    if (ftp_parse_url(remote_url, &remote_url_info) ||
        ((remote_url_info.backend == GS_FTP_BACKEND_RAM) && (remote_url_info.size == 0))) {
        log_error("%s: Invalid remote URL: [%s]", __FUNCTION__, remote_url);
        return GS_ERROR_ARG;
    }

    gs_ftp_state_t state = {
        .fp = NULL,
        .chunk_size = gs_ftp_get_chunk_size(settings),
        .timeout = gs_ftp_get_timeout(settings),
        .info_callback = info_callback,
        .info_data = info_data,
    };
    GS_STRNCPY(state.file_name, local_url_info.path);

    ftp_packet_t req;
    req.type = FTP_DOWNLOAD_REQUEST;
    req.down.chunk_size = csp_hton16(state.chunk_size);
    req.down.mem_addr = csp_hton32(remote_url_info.addr);
    req.down.mem_size = csp_hton32(remote_url_info.size);
    req.down.backend = gs_ftp_get_backend(&remote_url_info, settings);
    GS_STRNCPY(req.down.path, remote_url_info.path);
    int req_length = sizeof(req.type) + sizeof(req.down);

    state.conn = csp_connect(CSP_PRIO_NORM, settings->host, gs_ftp_get_csp_port(settings), 5000, CSP_O_RDP | CSP_O_CRC32);
    if (state.conn == NULL) {
        return GS_ERROR_IO;
    }

    ftp_packet_t rep;
    int rep_length = sizeof(req.type) + sizeof(req.downrep);
    int res_length = csp_transaction_persistent(state.conn, gs_ftp_get_timeout(settings), &req, req_length, &rep, rep_length);
    if (res_length != rep_length) {
        log_error("Length mismatch. Expected %d, got %d", rep_length, res_length);
        ftp_done(&state, 0);
        return GS_ERROR_TIMEOUT;
    }

    if (rep.type != FTP_DOWNLOAD_REPLY || rep.downrep.ret != GS_FTP_RET_OK) {
        ftp_done(&state, 0);
        return ftp_error_to_gs_error(rep.downrep.ret);
    }
    state.file_size = csp_ntoh32(rep.downrep.size);
    state.checksum = csp_ntoh32(rep.downrep.crc32);
    state.chunks = (state.file_size + state.chunk_size - 1) / state.chunk_size;
    if (info_callback) {
        gs_ftp_info_t info = {
            .user_data = info_data,
            .type      = GS_FTP_INFO_FILE,
            .u.file = {
                .size = state.file_size,
                .crc  = state.checksum,
            },
        };
        info_callback(&info);
    }

    /* Map file name */
    char map[100];

    /* Try to open file */
    bool new_file = false;
    state.fp = fopen(local_url_info.path, "r+");
    if (state.fp) {
        ftp_done(&state, 0);
        return GS_ERROR_EXIST;
    }

    /* Tmp file name */
    char tmp[100];
    snprintf(tmp, sizeof(tmp), "%s.tmp", local_url_info.path);

    state.fp = fopen(tmp, "r+");
    if (state.fp == NULL) {
        /* Create new file */
        new_file = true;
        state.fp = fopen(tmp, "w+");
        if (state.fp == NULL) {
            log_error("Client: Failed to create data file");
            ftp_done(&state, 0);
            return GS_ERROR_IO;
        }
    }

    /* Try to create a new bitmap */
    snprintf(map, sizeof(map), "%s.map", local_url_info.path);

    /* Check if file already exists */
    state.fp_map = fopen(map, "r+");
    if (state.fp_map == NULL) {
        unsigned int i;

        /* Abort if data file exists but map doesn't */
        if (!new_file) {
            ftp_done(&state, 0);
            return GS_ERROR_EXIST;
        }

        /* Create new file */
        state.fp_map = fopen(map, "w+");
        if (state.fp_map == NULL) {
            log_error("Failed to create bitmap");
            ftp_done(&state, 0);
            return GS_ERROR_IO;
        }

        /* Clear contents */
        for (i = 0; i < state.chunks; i++) {
            if (fwrite(packet_missing, 1, 1, state.fp_map) < 1) {
                log_error("Failed to clear bitmap");
                ftp_done(&state, 0);
                return GS_ERROR_IO;
            }
        }

        fflush(state.fp_map);
        fsync(fileno(state.fp_map));
    }

    /* Handle data stage */
    gs_error_t status = ftp_status_reply(&state);
    if (status) {
        ftp_done(&state, 0);
        return GS_ERROR_UNKNOWN;
    }

    status = ftp_crc(&state);
    if (status) {
        ftp_done(&state, 0);
        return status;
    }

    ftp_done(&state, 1);
    return GS_OK;
}

static gs_error_t ftp_status_request(gs_ftp_state_t *state)
{
    /* Request */
    ftp_packet_t req;
    req.type = FTP_STATUS_REQUEST;
    int req_length = sizeof(req.type);

    /* Transaction */
    ftp_packet_t rep;
    if (csp_transaction_persistent(state->conn, state->timeout, &req, req_length, &rep, -1) == 0) {
        log_error("Failed to receive status reply");
        return GS_ERROR_IO;
    }

    if (rep.type != FTP_STATUS_REPLY || rep.statusrep.ret != GS_FTP_RET_OK) {
        log_error("Reply was not STATUS_REPLY");
        return GS_ERROR_DATA;
    }

    rep.statusrep.complete = csp_ntoh32(rep.statusrep.complete);
    rep.statusrep.total = csp_ntoh32(rep.statusrep.total);
    rep.statusrep.entries = csp_ntoh16(rep.statusrep.entries);

    if (state->info_callback) {
        gs_ftp_info_t info = {
            .user_data = state->info_data,
            .type      = GS_FTP_INFO_COMPLETED,
            .u.completed = {
                .completed_chunks = rep.statusrep.complete,
                .total_chunks     = rep.statusrep.total,
            },
        };
        state->info_callback(&info);
    }

    if (rep.statusrep.complete != rep.statusrep.total) {
        int i;
        for (i = 0; i < rep.statusrep.entries; i++) {
            rep.statusrep.entry[i].next = csp_ntoh32(rep.statusrep.entry[i].next);
            rep.statusrep.entry[i].count = csp_ntoh32(rep.statusrep.entry[i].count);
            state->last_status[i].next = rep.statusrep.entry[i].next;
            state->last_status[i].count = rep.statusrep.entry[i].count;
        }

        state->last_entries = rep.statusrep.entries;
    } else {
        state->last_entries = 0;
    }

    return GS_OK;
}


static gs_error_t ftp_status_reply(gs_ftp_state_t *state)
{
    ftp_packet_t ftp_packet_req = { .type = FTP_STATUS_REPLY};
    ftp_status_reply_t * status = (ftp_status_reply_t *) &ftp_packet_req.statusrep;

    /* Build status reply */
    unsigned int i = 0, next = 0, count = 0;

    status->entries = 0;
    status->complete = 0;
    for (i = 0; i < state->chunks; i++) {
        int s;
        char cstat;

        /* Read chunk status */
        if ((unsigned int) ftell(state->fp_map) != i) {
            if (fseek(state->fp_map, i, SEEK_SET) != 0) {
                log_error("fseek failed");
                return GS_ERROR_IO;
            }
        }
        if (fread(&cstat, 1, 1, state->fp_map) != 1) {
            log_error("fread byte %u failed", i);
            return GS_ERROR_IO;
        }

        s = (cstat == *packet_ok);

        /* Increase complete counter if chunk was received */
        if (s) status->complete++;

        /* Add chunk status to packet */
        if (status->entries < GS_FTP_STATUS_CHUNKS) {
            if (!s) {
                if (!count)
                    next = i;
                count++;
            }

            if (count > 0 && (s || i == state->chunks - 1)) {
                status->entry[status->entries].next = csp_hton32(next);
                status->entry[status->entries].count = csp_hton32(count);
                status->entries++;
            }
        }
    }

    if (state->info_callback) {
        gs_ftp_info_t info = {
            .user_data = state->info_data,
            .type      = GS_FTP_INFO_COMPLETED,
            .u.completed = {
                .completed_chunks = status->complete,
                .total_chunks     = state->chunks,
            },
        };
        state->info_callback(&info);
    }

    status->entries = csp_hton16(status->entries);
    status->complete = csp_hton32(status->complete);
    status->total = csp_hton32(state->chunks);
    status->ret = GS_FTP_RET_OK;

    /* Send reply */
    if (csp_transaction_persistent(state->conn, state->timeout,
                                   &ftp_packet_req, sizeof(ftp_packet_req.type) + sizeof(ftp_packet_req.statusrep), NULL, 0) != 1) {
        log_error("Failed to send status reply");
        return GS_ERROR_IO;
    }

    /* File size is 0 */
    if (state->chunks == 0) {
        return GS_OK;
    }

    /* Read data */
    csp_packet_t * packet;
    while (1) {
        packet = csp_read(state->conn, state->timeout);

        if (!packet) {
            log_error("Timeout while waiting for data");
            return GS_ERROR_TIMEOUT;
        }

        ftp_packet_t * ftp_packet = (ftp_packet_t *) &packet->data;

        ftp_packet->data.chunk = csp_ntoh32(ftp_packet->data.chunk);
        unsigned int size;

        if (ftp_packet->type == GS_FTP_RET_IO) {
            log_error("Server failed to read chunk");
            csp_buffer_free(packet);
            return GS_ERROR_DATA;
        }

        if (ftp_packet->data.chunk >= state->chunks) {
            log_error("Bad chunk number %" PRIu32 " > %" PRIu32, ftp_packet->data.chunk, state->chunks);
            csp_buffer_free(packet);
            continue;
        }

        if (ftp_packet->data.chunk == state->chunks - 1) {
            size = state->file_size % state->chunk_size;
            if (size == 0) {
                size = state->chunk_size;
            }
        } else {
            size = state->chunk_size;
        }

        if ((unsigned int) ftell(state->fp) != ftp_packet->data.chunk * state->chunk_size) {
            if (fseek(state->fp, ftp_packet->data.chunk * state->chunk_size, SEEK_SET) != 0) {
                log_error("Seek error");
                csp_buffer_free(packet);
                return GS_ERROR_ACCESS;
            }
        }

        if (fwrite(ftp_packet->data.bytes, 1, size, state->fp) != size) {
            log_error("Write error");
            csp_buffer_free(packet);
            return GS_ERROR_IO;
        }
        fflush(state->fp);

        if ((unsigned int) ftell(state->fp_map) != ftp_packet->data.chunk) {
            if (fseek(state->fp_map, ftp_packet->data.chunk, SEEK_SET) != 0) {
                log_error("Map Seek error");
                csp_buffer_free(packet);
                return GS_ERROR_IO;
            }
        }

        if (fwrite(packet_ok, 1, 1, state->fp_map) != 1) {
            log_error("Map write error");
            csp_buffer_free(packet);
            return GS_ERROR_IO;
        }
        fflush(state->fp_map);

        /* Show progress bar */
        if (state->info_callback) {
            gs_ftp_info_t info = {
                .user_data = state->info_data,
                .type      = GS_FTP_INFO_PROGRESS,
                .u.progress = {
                    .current_chunk = ftp_packet->data.chunk,
                    .total_chunks  = state->chunks,
                    .chunk_size    = state->chunk_size,
                },
            };
            state->info_callback(&info);
        }

        /* Free buffer element */
        csp_buffer_free(packet);

        /* Break if all packets were received */
        if (ftp_packet->data.chunk == state->chunks - 1)
            break;
    }

    /* Sync file to disk */
    fflush(state->fp);
    fsync(fileno(state->fp));

    fflush(state->fp_map);
    fsync(fileno(state->fp_map));

    return 0;
}

static gs_error_t ftp_data(gs_ftp_state_t *state, int count)
{
    unsigned int i, j;
    int ret;

    /* Request */
    ftp_packet_t packet;
    packet.type = FTP_DATA;
    for (i = 0; i < state->last_entries; i++) {
        ftp_status_element_t * n = &state->last_status[i];

        for (j = 0; j < n->count; j++) {
            /* Calculate chunk number */
            packet.data.chunk = n->next + j;

            /* Print progress bar */
            if (state->info_callback) {
                gs_ftp_info_t info = {
                    .user_data = state->info_data,
                    .type      = GS_FTP_INFO_PROGRESS,
                    .u.progress = {
                        .current_chunk = packet.data.chunk,
                        .total_chunks  = state->chunks,
                        .chunk_size    = state->chunk_size,
                    },
                };
                state->info_callback(&info);
            }

            /* Read chunk */
            if ((unsigned int) ftell(state->fp) != packet.data.chunk * state->chunk_size) {
                fseek(state->fp, packet.data.chunk * state->chunk_size, SEEK_SET);
            }
            ret = fread(packet.data.bytes, 1, state->chunk_size, state->fp);

            if (ret < 0) {
                if (!feof(state->fp)) {
                    break;
                }
            }

            // if read is short of 'chunck', clear remaining as full 'chunk' is always sent
            if (ret < state->chunk_size) {
                memset(&packet.data.bytes[ret], 0, state->chunk_size - ret);
            }

            /* Chunk number MUST be little-endian!
             * Note: Yes, this is due to an old mistake, and now we are stuck with it! :( */
            packet.data.chunk = csp_htole32(packet.data.chunk);

            /* Send data */
            int length = sizeof(packet.type) + sizeof(uint32_t) + state->chunk_size;
            if (csp_transaction_persistent(state->conn, state->timeout, &packet, length, NULL, 0) != 1) {
                log_error("Data transaction failed");
                csp_close(state->conn);
                break;
            }
        }
    }

    return GS_OK;

}

static void ftp_done(gs_ftp_state_t *state, unsigned int remove_map)
{
    /* Close file and map */
    if (state->fp_map) {
        fclose(state->fp_map);
    }
    if (state->fp) {
        fclose(state->fp);
    }

    /* Delete map file if it exits */
    if (remove_map) {
        char map[GS_FTP_PATH_LENGTH+4];
        snprintf(map, sizeof(map), "%s.map", state->file_name);
        int status = remove(map);
        if (status != 0) {
            log_error("Failed to remove %s %d", map, status);
        }
        /* Move file to it's real name */
        char tmp[GS_FTP_PATH_LENGTH+4];
        snprintf(tmp, sizeof(tmp), "%s.tmp", state->file_name);
        status = rename(tmp, state->file_name);
        if (status != 0) {
            log_error("Failed to rename %s %s %d", tmp, state->file_name, status);
        }
    }

    /* Request */
    ftp_packet_t packet;
    packet.type = FTP_DONE;

    csp_transaction_persistent(state->conn, state->timeout, &packet, sizeof(packet.type), NULL, 0);
    csp_close(state->conn);
}

static gs_error_t ftp_crc(gs_ftp_state_t *state)
{
    /* Request */
    ftp_packet_t packet;
    packet.type = FTP_CRC_REQUEST;

    /* Reply */
    int repsiz = sizeof(packet.type) + sizeof(packet.crcrep);
    if (csp_transaction_persistent(state->conn, state->timeout, &packet, sizeof(packet.type), &packet, repsiz) != repsiz) {
        return GS_ERROR_IO;
    }
    if (packet.type != FTP_CRC_REPLY) {
        return GS_ERROR_DATA;
    }
    if (packet.crcrep.ret != GS_FTP_RET_OK) {
        return ftp_error_to_gs_error(packet.crcrep.ret);
    }

    /* Calculate CRC32 */
    ftp_file_crc32(state);

    /* Validate checksum */
    packet.crcrep.crc = csp_ntoh32(packet.crcrep.crc);
    if (state->info_callback) {
        gs_ftp_info_t info = {
            .user_data = state->info_data,
            .type      = GS_FTP_INFO_CRC,
            .u.crc = {
                .remote = packet.crcrep.crc,
                .local  = state->checksum,
            },
        };
        state->info_callback(&info);
    }
    if (packet.crcrep.crc != state->checksum) {
        return GS_ERROR_DATA;
    }

    return GS_OK;

}

gs_error_t gs_ftp_move(const gs_ftp_settings_t * settings, const char * from, const char * to)
{
    ftp_packet_t packet;

    GS_CHECK_ARG(from && from[0] && (strnlen(from, sizeof(packet.move.from)) < sizeof(packet.move.from)));
    GS_CHECK_ARG(to && to[0] && (strnlen(to, sizeof(packet.move.to)) < sizeof(packet.move.to)));

    packet.type = FTP_MOVE_REQUEST;
    GS_STRNCPY(packet.move.from, from);
    GS_STRNCPY(packet.move.to, to);
    packet.move.backend = gs_ftp_get_backend(NULL, settings);

    const int reqsiz = sizeof(packet.type) + sizeof(packet.move);
    const int repsiz = sizeof(packet.type) + sizeof(packet.moverep);

    if (csp_transaction(CSP_PRIO_NORM, settings->host, gs_ftp_get_csp_port(settings),
                        gs_ftp_get_timeout(settings), &packet, reqsiz, &packet, repsiz) != repsiz) {
        return GS_ERROR_IO;
    }
    if (packet.type != FTP_MOVE_REPLY) {
        return GS_ERROR_DATA;
    }
    return ftp_error_to_gs_error(packet.moverep.ret);
}

gs_error_t gs_ftp_copy(const gs_ftp_settings_t * settings, const char * from, const char * to)
{
    ftp_packet_t packet;

    GS_CHECK_ARG(from && from[0] && (strnlen(from, sizeof(packet.copy.from)) < sizeof(packet.copy.from)));
    GS_CHECK_ARG(to && to[0] && (strnlen(to, sizeof(packet.copy.to)) < sizeof(packet.copy.to)));

    packet.type = FTP_COPY_REQUEST;
    GS_STRNCPY(packet.copy.from, from);
    GS_STRNCPY(packet.copy.to, to);
    packet.copy.backend = gs_ftp_get_backend(NULL, settings);

    int reqsiz = sizeof(packet.type) + sizeof(packet.copy);
    int repsiz = sizeof(packet.type) + sizeof(packet.copyrep);

    if (csp_transaction(CSP_PRIO_NORM, settings->host, gs_ftp_get_csp_port(settings),
                        gs_ftp_get_timeout(settings), &packet, reqsiz, &packet, repsiz) != repsiz) {
        return GS_ERROR_IO;
    }
    if (packet.type != FTP_COPY_REPLY) {
        return GS_ERROR_DATA;
    }
    return ftp_error_to_gs_error(packet.copyrep.ret);
}

gs_error_t gs_ftp_remove(const gs_ftp_settings_t * settings, const char * path)
{
    ftp_packet_t packet;

    GS_CHECK_ARG(path && path[0] && (strnlen(path, sizeof(packet.remove.path)) < sizeof(packet.remove.path)));

    packet.type = FTP_REMOVE_REQUEST;
    GS_STRNCPY(packet.remove.path, path);
    packet.remove.backend = gs_ftp_get_backend(NULL, settings);

    int reqsiz = sizeof(packet.type) + sizeof(packet.remove);
    int repsiz = sizeof(packet.type) + sizeof(packet.removerep);

    if (csp_transaction(CSP_PRIO_NORM, settings->host, gs_ftp_get_csp_port(settings),
                        gs_ftp_get_timeout(settings), &packet, reqsiz, &packet, repsiz) != repsiz) {
        return GS_ERROR_IO;
    }
    if (packet.type != FTP_REMOVE_REPLY) {
        return GS_ERROR_DATA;
    }
    return ftp_error_to_gs_error(packet.removerep.ret);
}

gs_error_t gs_ftp_mkfs(const gs_ftp_settings_t * settings, const char *path, bool force)
{
    /* Request */
    ftp_packet_t packet;
    packet.type = FTP_MKFS_REQUEST;
    GS_STRNCPY(packet.mkfs.path, path);
    packet.mkfs.backend = gs_ftp_get_backend(NULL, settings);
    packet.mkfs.force = force ? 1 : 0;

    const int reqsiz = sizeof(packet.type) + sizeof(packet.mkfs);
    const int repsiz = sizeof(packet.type) + sizeof(packet.mkfsrep);

    /* The default FTP timeout is not long enough for a full UFFS format.
     * Bump to 120 seconds for this command */
    if (csp_transaction(CSP_PRIO_NORM, settings->host, gs_ftp_get_csp_port(settings),
                        120000, &packet, reqsiz, &packet, repsiz) != repsiz) {
        return GS_ERROR_IO;
    }
    if (packet.type != FTP_MKFS_REPLY) {
        return GS_ERROR_DATA;
    }
    return ftp_error_to_gs_error(packet.mkfsrep.ret);
}

gs_error_t gs_ftp_mkdir(const gs_ftp_settings_t * settings, const char *path, uint32_t mode)
{
    /* Request */
    ftp_packet_t packet;
    packet.type = FTP_MKDIR_REQUEST;
    GS_STRNCPY(packet.mkdir.path, path);
    packet.mkdir.backend = gs_ftp_get_backend(NULL, settings);
    packet.mkdir.mode = mode;

    const int reqsiz = sizeof(packet.type) + sizeof(packet.mkdir);
    const int repsiz = sizeof(packet.type) + sizeof(packet.mkdirrep);

    if (csp_transaction(CSP_PRIO_NORM, settings->host, gs_ftp_get_csp_port(settings),
                        gs_ftp_get_timeout(settings), &packet, reqsiz, &packet, repsiz) != repsiz) {
        return GS_ERROR_IO;
    }
    if (packet.type != FTP_MKDIR_REPLY) {
        return GS_ERROR_DATA;
    }
    return ftp_error_to_gs_error(packet.mkdirrep.ret);
}

gs_error_t gs_ftp_rmdir(const gs_ftp_settings_t * settings, const char *path)
{
    /* Request */
    ftp_packet_t packet;
    packet.type = FTP_RMDIR_REQUEST;
    GS_STRNCPY(packet.rmdir.path, path);
    packet.rmdir.backend = gs_ftp_get_backend(NULL, settings);

    const int reqsiz = sizeof(packet.type) + sizeof(packet.rmdir);
    const int repsiz = sizeof(packet.type) + sizeof(packet.rmdirrep);

    if (csp_transaction(CSP_PRIO_NORM, settings->host, gs_ftp_get_csp_port(settings),
                        gs_ftp_get_timeout(settings), &packet, reqsiz, &packet, repsiz) != repsiz) {
        return GS_ERROR_IO;
    }
    if (packet.type != FTP_RMDIR_REPLY) {
        return GS_ERROR_DATA;
    }
    return ftp_error_to_gs_error(packet.rmdirrep.ret);
}

gs_error_t gs_ftp_list(const gs_ftp_settings_t * settings, const char * path, gs_ftp_list_callback_t callback, void *data)
{
    /* Request */
    int entries;
    csp_conn_t * c;
    csp_packet_t * p;
    ftp_packet_t packet;
    packet.type = FTP_LIST_REQUEST;
    packet.list.backend = gs_ftp_get_backend(NULL, settings);
    GS_STRNCPY(packet.list.path, path);

    const int reqsiz = sizeof(packet.type) + sizeof(packet.list);
    const int repsiz = sizeof(packet.type) + sizeof(packet.listrep);

    c = csp_connect(CSP_PRIO_NORM, settings->host, gs_ftp_get_csp_port(settings), gs_ftp_get_timeout(settings), CSP_O_RDP);
    if (c == NULL) {
        return GS_ERROR_IO;
    }
    if (csp_transaction_persistent(c, gs_ftp_get_timeout(settings), &packet, reqsiz, &packet, repsiz) != repsiz) {
        csp_close(c);
        return GS_ERROR_IO;
    }
    if (packet.type != FTP_LIST_REPLY) {
        csp_close(c);
        return GS_ERROR_DATA;
    }
    if (packet.listrep.ret != GS_FTP_RET_OK) {
        csp_close(c);
        return ftp_error_to_gs_error(packet.listrep.ret);
    }

    entries = csp_ntoh16(packet.listrep.entries);

    /* Read entries */
    ftp_packet_t * f;
    while (entries && ((p = csp_read(c, gs_ftp_get_timeout(settings))) != NULL)) {
        f = (ftp_packet_t *) &(p->data);
        f->listent.entry = csp_ntoh16(f->listent.entry);
        f->listent.size = csp_ntoh32(f->listent.size);

        callback(entries, &f->listent, data);

        uint16_t ent = f->listent.entry;
        csp_buffer_free(p);
        if (ent == entries - 1)
            break;
    }

    csp_close(c);

    return GS_OK;
}

gs_error_t gs_ftp_zip(const gs_ftp_settings_t * settings, const char * src, const char * dest, uint8_t action,
                      uint32_t *comp_sz, uint32_t *decomp_sz)
{
    /* Request */
    ftp_packet_t packet;
    packet.type = FTP_ZIP_REQUEST;
    GS_STRNCPY(packet.zip.src, src);
    GS_STRNCPY(packet.zip.dest, dest);
    packet.zip.action = action;
    packet.zip.backend = gs_ftp_get_backend(NULL, settings);

    const int reqsiz = sizeof(packet.type) + sizeof(packet.zip);
    const int repsiz = sizeof(packet.type) + sizeof(packet.ziprep);

    if (csp_transaction(CSP_PRIO_NORM, settings->host, gs_ftp_get_csp_port(settings),
                        gs_ftp_get_timeout(settings), &packet, reqsiz, &packet, repsiz) != repsiz) {
        return GS_ERROR_IO;
    }
    if (packet.type != FTP_ZIP_REPLY) {
        return GS_ERROR_DATA;
    }
    if (packet.ziprep.ret != GS_FTP_RET_OK) {
        return ftp_error_to_gs_error(packet.ziprep.ret);
    }

    if(comp_sz) {
        *comp_sz = csp_ntoh32(packet.ziprep.comp_sz);
    }
    if (decomp_sz) {
        *decomp_sz = csp_ntoh32(packet.ziprep.decomp_sz);
    }

    return GS_OK;
}

gs_error_t gs_ftp_default_settings(gs_ftp_settings_t *settings)
{
    if (settings) {
        memset(settings, 0, sizeof(*settings));

        settings->timeout = DEFAULT_TIMEOUT;
        settings->chunk_size = DEFAULT_CHUNK_SIZE;
        return GS_OK;
    }
    return GS_ERROR_ARG;
}

#define FTP_MODE_STANDARD "standard"
#define FTP_MODE_GATOSS   "gatoss"

gs_error_t gs_ftp_string_to_mode(const char * str, gs_ftp_mode_t * return_mode)
{
    gs_error_t error = GS_ERROR_ARG;
    gs_ftp_mode_t mode = GS_FTP_MODE_STANDARD;
    if (gs_string_empty(str) == false) {
        if (strcasecmp(str, FTP_MODE_STANDARD) == 0) {
            mode = GS_FTP_MODE_STANDARD;
            error = GS_OK;
        } else if (strcasecmp(str, FTP_MODE_GATOSS) == 0) {
            mode = GS_FTP_MODE_GATOSS;
            error = GS_OK;
        }
    }
    if (return_mode) {
        *return_mode = mode;
    }
    return error;
}

const char * gs_ftp_mode_to_string(gs_ftp_mode_t mode)
{
    switch (mode) {
        case GS_FTP_MODE_STANDARD: return FTP_MODE_STANDARD;
        case GS_FTP_MODE_GATOSS:   return FTP_MODE_GATOSS;
    }
    return "unknown";
}

float gs_ftp_percent_completed(uint32_t completed, uint32_t total)
{
    if (completed >= total) {
        return 100.0f;
    }
    if (completed == 0) {
        return 0.0f;
    }
    return (float) (completed * 100) / (float) total;
}
