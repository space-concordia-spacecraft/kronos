/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/ftp/client.h>

#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <gs/util/gosh/command.h>

#include <gs/util/log.h>
#include <gs/util/string.h>
#include <gs/util/clock.h>
#include <gs/util/time.h>
#include <gs/csp/port.h>
#include <gs/util/zip/zip.h>

/* Settings */
static gs_ftp_settings_t ftp_settings;

#define PROGRESS_WIDTH	40

typedef struct {
    uint32_t last_chunk;
    float bps;
    timestamp_t last;
    timestamp_t last_speed;
    gs_command_context_t *ctx;
} progress_data_t;

typedef struct {
    progress_data_t       progress;
    gs_command_context_t *ctx;
} ftp_user_info_t;

static float timespec_diff(timestamp_t * start, timestamp_t * end)
{
    struct timespec temp;
    if (((float) end->tv_nsec - (float) start->tv_nsec) < 0) {
        temp.tv_sec = end->tv_sec-start->tv_sec - 1;
        temp.tv_nsec = 1000000000 + end->tv_nsec - start->tv_nsec;
    } else {
        temp.tv_sec = end->tv_sec - start->tv_sec;
        temp.tv_nsec = end->tv_nsec - start->tv_nsec;
    }
    return (float)(temp.tv_sec + (float)temp.tv_nsec/1000000000);
}

static void progress_bar(uint32_t current_chunk, uint32_t total_chunks, uint32_t chunk_size, ftp_user_info_t * info)
{
    float sec, speed_sec, alpha = 0.25;
    char buf[200] = "";
    int idx = 0;
    uint32_t eta_sec=0;
    timestamp_t now;

    /* Only print progress to TTY */
    if (!isatty(fileno(info->ctx->out))) {
        return;
    }
    gs_clock_get_monotonic(&now);
    sec = timespec_diff(&info->progress.last, &now);
    speed_sec = timespec_diff(&info->progress.last_speed, &now);

    /* Update progress bar every second and for last chunk */
    if (sec > 1.0 || current_chunk == total_chunks - 1) {

        const float percent = gs_ftp_percent_completed(current_chunk+1, total_chunks);
        const int l = (int)(PROGRESS_WIDTH * (percent / 100.0));

        /* Clear line */
        idx += snprintf(&buf[idx], sizeof(buf) - idx, "\033[1K\r");

        idx += snprintf(&buf[idx], sizeof(buf) - idx, "%5.1f%% [", percent);
        int k;
        for (k = 0; k < l; k++)
            idx += snprintf(&buf[idx], sizeof(buf) - idx, "#");
        for (; k < PROGRESS_WIDTH; k++)
            idx += snprintf(&buf[idx], sizeof(buf) - idx, " ");
        idx += snprintf(&buf[idx], sizeof(buf) - idx, "] ");

        /* Show received chunks */
        idx += snprintf(&buf[idx], sizeof(buf) - idx, "(%d/%d) ", (int)current_chunk + 1, (int)total_chunks);

        /* Update speed and ETA */
        if (speed_sec >= 1.0) {
            /* Exponentially smoothed bytes per second */
            float this_bps = (((current_chunk + 1 - info->progress.last_chunk) * chunk_size) / speed_sec);
            info->progress.bps = info->progress.bps > 0 ? this_bps * alpha + info->progress.bps * (1 - alpha) : this_bps;

            /* Remaining time */
            eta_sec = ((total_chunks - current_chunk + 1) * chunk_size) / info->progress.bps;

            /* Update last time */
            info->progress.last_chunk = current_chunk;
            info->progress.last_speed = now;
        }

        /* Show speed and ETA if not last chunk */
        if (info->progress.last_chunk > 0 && current_chunk != total_chunks - 1) {
            /* Show speed */
            char tbuf[30];
            gs_time_to_string(eta_sec, tbuf, sizeof(tbuf));
            idx += snprintf(&buf[idx], sizeof(buf) - idx,
                            "%4.1f kB/s eta %s", (info->progress.bps / 1024), tbuf);
        }
        /* Insert newline after last chunk */
        if(current_chunk == total_chunks - 1) {
            idx += snprintf(&buf[idx], sizeof(buf) - idx, "\r\n");
        }
        buf[sizeof(buf) - 1] = '\0';
        fprintf(info->ctx->out, "%s", buf);
        fflush(info->ctx->out);

        /* Update last time */
        info->progress.last = now;
    }
}

static void ftp_info_print_callback(const gs_ftp_info_t * info)
{
    ftp_user_info_t *user = info->user_data;

    switch (info->type) {
        case GS_FTP_INFO_COMPLETED:
            {
                const uint32_t complete = info->u.completed.completed_chunks;
                const uint32_t total    = info->u.completed.total_chunks;
                fprintf(user->ctx->out, "Transfer Status: %" PRIu32 " of %" PRIu32 " (%.1f%%)\r\n",
                        complete, total, gs_ftp_percent_completed(complete, total));
            }
            break;
        case GS_FTP_INFO_FILE:
            {
                fprintf(user->ctx->out, "File size: %" PRIu32 ", checksum: 0x%" PRIx32 "\r\n",
                        info->u.file.size, info->u.file.crc);
            }
            break;
        case GS_FTP_INFO_CRC:
            {
                fprintf(user->ctx->out, "Checksum %s remote: 0x%" PRIx32 ", local: 0x%" PRIx32 "\r\n",
                        (info->u.crc.remote == info->u.crc.local) ? "OK" : "ERROR",
                        info->u.crc.remote, info->u.crc.local);
            }
            break;
        case GS_FTP_INFO_PROGRESS:
            progress_bar(info->u.progress.current_chunk, info->u.progress.total_chunks, info->u.progress.chunk_size,
                         info->user_data);
            break;
    }
}

static int cmd_ftp_set_server(gs_command_context_t *ctx)
{
    if (ctx->argc > 1) {
        uint32_t timeout = ftp_settings.timeout;
        gs_ftp_default_settings(&ftp_settings);
        ftp_settings.timeout = timeout;

        if (gs_string_to_uint8(ctx->argv[1], &ftp_settings.host)) {
            return GS_ERROR_ARG;
        }

        if (ctx->argc > 2) {
            if (gs_string_to_uint32(ctx->argv[2], &ftp_settings.chunk_size)) {
                return GS_ERROR_ARG;
            }
        }

        if (ctx->argc > 3) {
            if (gs_ftp_string_to_mode(ctx->argv[3], &ftp_settings.mode)) {
                return GS_ERROR_ARG;
            }
        }
    }

    fprintf(ctx->out, "server %u (port %u), chunk size %" PRIu32 " bytes",
            ftp_settings.host, gs_ftp_get_csp_port(&ftp_settings), gs_ftp_get_chunk_size(&ftp_settings));
    if (ftp_settings.mode != GS_FTP_MODE_STANDARD) {
        fprintf(ctx->out, ", mode %s", gs_ftp_mode_to_string(ftp_settings.mode));
    }
    fprintf(ctx->out, "\r\n");

    return GS_OK;
}

/* Some versions of newlib do not have basename
 * (looking at you, Atmel)
 */
static const char *ftp_basename(const char *path)
{
    char *base = strrchr(path, '/');
    return base ? base+1 : path;
}

static int cmd_ftp_download(gs_command_context_t *ctx)
{
    if ((ctx->argc < 2) || (ctx->argc > 3)) {
        return GS_ERROR_ARG;
    }

    const char * remote_path = ctx->argv[1];

    if (strlen(remote_path) >= GS_FTP_PATH_LENGTH) {
        return GS_ERROR_ARG;
    }

    const char * local_path;
    if (ctx->argc == 3) {
        local_path = ctx->argv[2];
    } else {
        local_path = ftp_basename(remote_path);
    }
    ftp_user_info_t info_data = {.ctx = ctx};
    gs_error_t status = gs_ftp_download(&ftp_settings, local_path, remote_path, ftp_info_print_callback, &info_data);
    return status;
}

static int cmd_ftp_upload(gs_command_context_t *ctx)
{
    if (ctx->argc != 3) {
        return GS_ERROR_ARG;
    }
    const char * remote_path = ctx->argv[2];
    const char * local_path = ctx->argv[1];

    if (strlen(remote_path) >= GS_FTP_PATH_LENGTH || strlen(remote_path) < 3) {
        return GS_ERROR_ARG;
    }
    if (strlen(local_path) < 3) {
        return GS_ERROR_ARG;
    }

    ftp_user_info_t info_data = {.ctx = ctx};
    gs_error_t status = gs_ftp_upload(&ftp_settings, local_path, remote_path, ftp_info_print_callback, &info_data);
    return status;
}

static int cmd_ftp_download_mem(gs_command_context_t *ctx)
{
    if (ctx->argc != 4) {
        return GS_ERROR_ARG;
    }

    const char * local_url = ctx->argv[1];
    if (strlen(local_url) >= GS_FTP_PATH_LENGTH || strlen(local_url) < 3) {
        return GS_ERROR_ARG;
    }

    uint32_t memaddr;
    if (gs_string_to_uint32(ctx->argv[2], &memaddr)) {
        return GS_ERROR_ARG;
    }

    uint32_t memsize;
    if (gs_string_to_uint32(ctx->argv[3], &memsize)) {
        return GS_ERROR_ARG;
    }

    fprintf(ctx->out, "Downloading from addr 0x%"PRIx32" size %"PRIu32" to file %s\r\n", memaddr, memsize, local_url);
    char remote_url[32];

    sprintf(remote_url, "mem://%"PRIu32"++%"PRIu32, memaddr, memsize);
    ftp_user_info_t info_data = {.ctx = ctx};
    gs_error_t status = gs_ftp_download(&ftp_settings, local_url, remote_url, ftp_info_print_callback, &info_data);
    return status;
}

static int cmd_ftp_upload_mem(gs_command_context_t *ctx)
{
    if (ctx->argc != 3) {
        return GS_ERROR_ARG;
    }

    const char * local_url = ctx->argv[1];
    if (strlen(local_url) >= GS_FTP_PATH_LENGTH || strlen(local_url) < 3) {
        return GS_ERROR_ARG;
    }

    uint32_t memaddr;
    if (gs_string_to_uint32(ctx->argv[2], &memaddr)) {
        return GS_ERROR_ARG;
    }

    fprintf(ctx->out, "Uploading from %s to 0x%"PRIx32"\r\n", local_url, memaddr);
    char remote_url[32];

    ftp_user_info_t info_data = {.ctx = ctx};
    sprintf(remote_url, "mem://%"PRIu32, memaddr);
    gs_error_t status = gs_ftp_upload(&ftp_settings, local_url, remote_url, ftp_info_print_callback, &info_data);
    return status;
}

static int cmd_ftp_zip(gs_command_context_t *ctx)
{
    if (ctx->argc != 3) {
        return GS_ERROR_ARG;
    }

    uint8_t action;
    if (!strcmp(ctx->argv[0], "zip")) {
        action = GS_FTP_ZIP;
    } else {
        action = GS_FTP_UNZIP;
    }
    const char * src_path = ctx->argv[1];
    if (strlen(src_path) >= GS_FTP_PATH_LENGTH || strlen(src_path) < 3) {
        return GS_ERROR_ARG;
    }

    const char * dest_path = ctx->argv[2];
    if (strlen(dest_path) >= GS_FTP_PATH_LENGTH || strlen(dest_path) < 3) {
        return GS_ERROR_ARG;
    }

    uint32_t comp_sz;
    uint32_t decomp_sz;
    gs_error_t status = gs_ftp_zip(&ftp_settings, src_path, dest_path, action, &comp_sz, &decomp_sz);
    if (status == GS_OK) {
        if (action == GS_FTP_ZIP) {
            fprintf(ctx->out, "Zip %s to %s, unzipped: %" PRIu32 ", zipped: %" PRIu32 "\r\n",
                    src_path, dest_path, decomp_sz, comp_sz);
        } else {
            fprintf(ctx->out, "Unzip %s to %s, zipped: %" PRIu32 ", unzipped: %" PRIu32 "\r\n",
                    src_path, dest_path, comp_sz, decomp_sz);
        }
    }

    return status;
}

static int cmd_ftp_zip_local(gs_command_context_t *ctx)
{
    const char * src = ctx->argv[1];
    const char * dest = ctx->argv[2];
    if(!strcmp(src, dest))
    {
        //src and dest must be different
        log_error("zip: output file cannot be the same as input");
        return GS_ERROR_ARG;
    }

    // Make sure the source exist
    struct stat statbuf;
    if(stat(src, &statbuf) != 0)
    {
        log_error("zip: input file not found");
        return GS_ERROR_NOT_FOUND;
    }

    if (!strcmp(ctx->argv[0], "local_zip")) {
        if(gs_zip_compress_file(src, dest) != GS_OK) {
            log_error("zip: Compression failed");
            return GS_ERROR_IO;
        }
    } else {
        if(gs_zip_decompress_file(src, dest) != GS_OK) {
            log_error("zip: Decompression failed");
            return GS_ERROR_IO;
        }
    }

    if(stat(dest, &statbuf) != 0)
    {
        //Check that output file exist
        log_error("zip: Failed to create output file");
        return GS_ERROR_IO;
    }
    return GS_OK;
}

static int ftp_ls_print_callback(uint16_t entries, const gs_ftp_list_entry_t *entry, void *user_data)
{
    gs_command_context_t *ctx = user_data;
    char bytebuf[25];

    gs_string_bytesize(entry->size, bytebuf, sizeof(bytebuf));

    fprintf(ctx->out, "%6s %s%s\r\n", bytebuf, entry->path, entry->type ? "/" : "");

    return GS_OK;
}

static int cmd_ftp_ls(gs_command_context_t *ctx)
{
    gs_error_t status =  gs_ftp_list(&ftp_settings, ctx->argv[1], ftp_ls_print_callback, ctx);
    return status;
}

static int cmd_ftp_remove(gs_command_context_t *ctx)
{
    gs_error_t status = gs_ftp_remove(&ftp_settings, ctx->argv[1]);
    return status;
}

static int cmd_ftp_mkfs(gs_command_context_t *ctx)
{
    bool force = false;

    if (ctx->argc < 2)
        return GS_ERROR_ARG;

    if (ctx->argc > 2) {
        if (!strcmp(ctx->argv[2], "1") || !strcmp(ctx->argv[2], "yes") || !strcmp(ctx->argv[2], "true")) {
            force = true;
        }
        else if (!strcmp(ctx->argv[2], "0") || !strcmp(ctx->argv[2], "no") || !strcmp(ctx->argv[2], "false")) {
            force = false;
        }
        else {
            return GS_ERROR_ARG;
        }
    }

    const char *path = ctx->argv[1];

    gs_error_t status = gs_ftp_mkfs(&ftp_settings, path, force);
    return status;
}

static int cmd_ftp_mkdir(gs_command_context_t *ctx)
{
    // default mode to 775
    uint32_t mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH; 

    if (ctx->argc != 2) {
        return GS_ERROR_ARG;
    }

    const char *path = ctx->argv[1];

    gs_error_t status = gs_ftp_mkdir(&ftp_settings, path, mode);
    return status;
}

static int cmd_ftp_rmdir(gs_command_context_t *ctx)
{
    if (ctx->argc != 2) {
        return GS_ERROR_ARG;
    }

    const char *path = ctx->argv[1];

    gs_error_t status = gs_ftp_rmdir(&ftp_settings, path);
    return status;
}

static int cmd_ftp_move(gs_command_context_t *ctx)
{
    if (ctx->argc != 3) {
        return GS_ERROR_ARG;
    }
    const char * from_path = ctx->argv[1];
    if (strlen(from_path) >= GS_FTP_PATH_LENGTH || strlen(from_path) < 3) {
        return GS_ERROR_ARG;
    }

    const char * to_path = ctx->argv[2];
    if (strlen(to_path) >= GS_FTP_PATH_LENGTH || strlen(to_path) < 3) {
        return GS_ERROR_ARG;
    }

    gs_error_t status = gs_ftp_move(&ftp_settings, from_path, to_path);
    return status;
}

static int cmd_ftp_copy(gs_command_context_t *ctx)
{
    if (ctx->argc != 3) {
        return GS_ERROR_ARG;
    }

    const char * from_path = ctx->argv[1];
    if (strlen(from_path) >= GS_FTP_PATH_LENGTH || strlen(from_path) < 3) {
        return GS_ERROR_ARG;
    }

    const char * to_path = ctx->argv[2];
    if (strlen(to_path) >= GS_FTP_PATH_LENGTH || strlen(to_path) < 3) {
        return GS_ERROR_ARG;
    }

    gs_error_t status = gs_ftp_copy(&ftp_settings, from_path, to_path);
    return status;
}

static int cmd_ftp_timeout(gs_command_context_t *ctx)
{
    if (ctx->argc > 1) {
        uint32_t timeout;
        if (gs_string_to_uint32(ctx->argv[1], &timeout)) {
            return GS_ERROR_ARG;
        }
        ftp_settings.timeout = timeout;
    }
    fprintf(ctx->out, "timeout is %"PRIu32" msec\r\n", gs_ftp_get_timeout(&ftp_settings));
    return GS_OK;
}

static const gs_command_t GS_COMMAND_SUB ftp_subcommands[] = {
    {
        .name = "ls",
        .help = "list files",
        .usage = "<path>",
        .handler = cmd_ftp_ls,
        .mandatory_args = 1,
    },{
        .name = "rm",
        .help = "rm files",
        .usage = "<path>",
        .handler = cmd_ftp_remove,
        .mandatory_args = 1,
    },{
        .name = "mkfs",
        .help = "make file system",
        .usage = "<path> [force]",
        .handler = cmd_ftp_mkfs,
    },{
        .name = "mkdir",
        .help = "make directory in file system",
        .usage = "<path>",
        .handler = cmd_ftp_mkdir,
    },{
        .name = "rmdir",
        .help = "remove a directory from the file system",
        .usage = "<path>",
        .handler = cmd_ftp_rmdir,
    },{
        .name = "mv",
        .help = "move files",
        .usage = "<from> <to>",
        .handler = cmd_ftp_move,
    },{
        .name = "cp",
        .help = "copy files",
        .usage = "<from> <to>",
        .handler = cmd_ftp_copy,
    },{
        .name = "zip",
        .help = "zip file",
        .usage = "<input file> <output file>",
        .handler = cmd_ftp_zip,
    },{
        .name = "unzip",
        .help = "unzip file",
        .usage = "<input file> <output file>",
        .handler = cmd_ftp_zip,
    },{
        .name = "local_zip",
        .help = "zip local file",
        .usage = "<input file> <output file>",
        .handler = cmd_ftp_zip_local,
        .mandatory_args = 2,
    },{
        .name = "local_unzip",
        .help = "unzip local file",
        .usage = "<input file> <output file>",
        .handler = cmd_ftp_zip_local,
        .mandatory_args = 2,
    },{
        .name = "server",
        .help = "set server, chunk size and mode",
        .usage = "[server] [chunk size] [mode]",
        .handler = cmd_ftp_set_server,
        .mandatory_args = GS_COMMAND_NO_ARGS,
        .optional_args = 3,
    },{
        .name = "upload",
        .help = "Upload url",
        .usage = "<local url> <remote url> ",
        .handler = cmd_ftp_upload,
        .mandatory_args = 2,
    },{
        .name = "download",
        .help = "Download url",
        .usage = "<remote url> [local url]",
        .handler = cmd_ftp_download,
        .mandatory_args = 1,
        .optional_args = 1,
    },{
        .name = "upload_file",
        .help = "Upload file",
        .usage = "<local filename> <remote filename>",
        .handler = cmd_ftp_upload,
        .mandatory_args = 2,
        .mode = GS_COMMAND_FLAG_HIDDEN,
    },{
        .name = "download_file",
        .help = "Download file",
        .usage = "<remote filename> [local filename]",
        .handler = cmd_ftp_download,
        .mandatory_args = 1,
        .optional_args = 1,
        .mode = GS_COMMAND_FLAG_HIDDEN,
    },{
        .name = "upload_mem",
        .help = "Upload to memory",
        .usage = "<local_file> <remote_addr>",
        .handler = cmd_ftp_upload_mem,
        .mandatory_args = 2,
        .mode = GS_COMMAND_FLAG_HIDDEN,
    },{
        .name = "download_mem",
        .help = "Download memory",
        .usage = "<local_file> <remote_addr> <length>",
        .handler = cmd_ftp_download_mem,
        .mandatory_args = 3,
        .mode = GS_COMMAND_FLAG_HIDDEN,
    },{
        .name = "timeout",
        .help = "Set general ftp timeout",
        .usage = "[timeout ms]",
        .handler = cmd_ftp_timeout,
        .mandatory_args = GS_COMMAND_NO_ARGS,
        .optional_args = 1,
    }
};

static const gs_command_t GS_COMMAND_ROOT ftp_commands[] = {
    {
        .name = "ftp",
        .help = "File Transfer Protocol client",
        .chain = GS_COMMAND_INIT_CHAIN(ftp_subcommands),
    }
};

gs_error_t gs_ftp_register_commands(void)
{
    return GS_COMMAND_REGISTER(ftp_commands);
}
