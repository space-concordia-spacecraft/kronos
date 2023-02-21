/* Copyright (c) 2013-2019 GomSpace A/S. All rights reserved. */
/**
 * NanoCam Client library
 */

#include <gs/cam/nanocam.h>

#include <inttypes.h>
#include <string.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <util/console.h>
#include <util/log.h>
#include <util/color_printf.h>
#include <gosh/getopt.h>

#include <gs/csp/csp.h>
#include <csp/csp.h>
#include <csp/csp_endian.h>
#include <csp/csp_cmp.h>
#include <gs/util/minmax.h>

/** Width of histogram */
#define HIST_WIDTH	50

/** Default timeout in ms */
static uint32_t cam_timeout = 5000;

/* Default store format is JPEG */
static uint8_t cam_format = NANOCAM_STORE_JPG;

static const char *format_to_string(uint8_t format)
{
    switch (format) {
	case NANOCAM_STORE_RAW:
            return "RAW";
	case NANOCAM_STORE_BMP:
            return "BMP";
	case NANOCAM_STORE_JPG:
            return "JPG";
	case NANOCAM_STORE_DNG:
            return "DNG";
	case NANOCAM_STORE_RAW10:
            return "RAW10";
	default:
            return "unknown";
    }
}

static int string_to_format(const char *format, uint8_t *value)
{
    int result = CMD_ERROR_NONE;
    if (!strcasecmp(format, "raw")) {
        *value = NANOCAM_STORE_RAW;
    } else if (!strcasecmp(format, "bmp")) {
        *value = NANOCAM_STORE_BMP;
    } else if (!strcasecmp(format, "jpg")) {
        *value = NANOCAM_STORE_JPG;
    } else if (!strcasecmp(format, "dng")) {
        *value = NANOCAM_STORE_DNG;
    } else if (!strcasecmp(format, "raw10")) {
        *value = NANOCAM_STORE_RAW10;
    } else {
        /* assume input was an uint8 - if not return error */
        if (gs_string_to_uint8(format, value) != GS_OK) {
            result = CMD_ERROR_SYNTAX;
        }
    }

    return result;
}

int cmd_cam_node(struct command_context *ctx)
{
    uint8_t node;
    if(ctx->argc < 2) {
        printf("CAM node = %d\r\n", nanocam_get_node());
        return CMD_ERROR_NONE;
    }

    if (gs_string_to_uint8(ctx->argv[1], &node) != GS_OK) {
        return CMD_ERROR_SYNTAX;
    }

    nanocam_set_node(node);
    return CMD_ERROR_NONE;
}

int cmd_cam_set_timeout(struct command_context *ctx)
{
    if (ctx->argc < 2) {
        printf("Current timeout is %"PRIu32"\r\n", cam_timeout);
        return CMD_ERROR_NONE;
    }

    if(gs_string_to_uint32(ctx->argv[1], &cam_timeout) != GS_OK) {
        return CMD_ERROR_SYNTAX;
    }

    printf("Timeout set to %"PRIu32"\r\n", cam_timeout);
    return CMD_ERROR_NONE;
}

int cmd_cam_format(struct command_context *ctx)
{
    if (ctx->argc < 2) {
        printf("Current format is %s (%hhu)\r\n", format_to_string(cam_format), cam_format);
        return CMD_ERROR_NONE;
    }

    if(string_to_format(ctx->argv[1], &cam_format) != CMD_ERROR_NONE) {
        return CMD_ERROR_SYNTAX;
    }

    printf("Format set to %s (%hhu)\r\n", format_to_string(cam_format), cam_format);
    return CMD_ERROR_NONE;
}

static color_printf_t hist_bin_color(bool red, bool green, bool blue)
{
    if (red && green && blue)
        return COLOR_WHITE;

    if (red && green)
        return COLOR_YELLOW;
    if (red && blue)
        return COLOR_MAGENTA;
    if (green && blue)
        return COLOR_CYAN;

    if (red)
        return COLOR_RED;
    if (green)
        return COLOR_GREEN;
    if (blue)
        return COLOR_BLUE;

    return COLOR_NONE;
}

int cmd_cam_snap(struct command_context *ctx)
{
    /* Argument parsing */
    const char *colors = "rgb";
    int c, remain, result;
    unsigned int i, j;
    bool histogram = true;

    cam_snap_t snap;
    cam_snap_reply_t reply;

    snap.count  = 1;
    snap.width  = 0;
    snap.height = 0;
    snap.delay  = 0;
    snap.flags  = 0;
    snap.format = cam_format;
    snap.left   = 0; // not used, initialize to zero
    snap.top    = 0; // not used, initialize to zero

    /* Histogram print */
    unsigned int bin_max = 0;
    bool r, g, b, show_r, show_g, show_b;

    while ((c = gosh_getopt(ctx, "ead:h:in:rstx")) != -1) {
        switch (c) {
            case 'e':
                snap.flags |= NANOCAM_SNAP_FLAG_NOEXPOSURE;
                break;
            case 'a':
                snap.flags |= NANOCAM_SNAP_FLAG_AUTO_GAIN;
                break;
            case 'd':
                if (gs_string_to_uint16(ctx->optarg, &snap.delay) != GS_OK) {
                    return CMD_ERROR_SYNTAX;
                }
                break;
            case 'h':
                colors = ctx->optarg;
                break;
            case 'i':
                snap.flags |= NANOCAM_SNAP_FLAG_STORE_TAGS;
                break;
            case 'n':
                if (gs_string_to_uint8(ctx->optarg, &snap.count) != GS_OK) {
                    return CMD_ERROR_SYNTAX;
                }
                break;
            case 'r':
                snap.flags |= NANOCAM_SNAP_FLAG_STORE_RAM;
                break;
            case 's':
                snap.flags |= NANOCAM_SNAP_FLAG_STORE_FLASH;
                break;
            case 't':
                snap.flags |= NANOCAM_SNAP_FLAG_STORE_THUMB;
                break;
            case 'x':
                histogram = false;
                snap.flags |= NANOCAM_SNAP_FLAG_NOHIST;
                break;
            case '?':
                return CMD_ERROR_SYNTAX;
        }
    }

    remain = ctx->argc - ctx->optind;
    int idx = ctx->optind;

    if (remain > 0)
        if (gs_string_to_uint16(ctx->argv[idx], &snap.width) != GS_OK) {
            return CMD_ERROR_SYNTAX;
    }
    if (remain > 1)
        if (gs_string_to_uint16(ctx->argv[idx + 1], &snap.height) != GS_OK) {
            return CMD_ERROR_SYNTAX;
    }

    if (snap.width || snap.height) {
        printf("Snapping %"PRIu16"x%"PRIu16" image, timeout %"PRIu32"\r\n",
               snap.width, snap.height, cam_timeout);
    } else {
        printf("Snapping full-size image, timeout %"PRIu32"\r\n", cam_timeout);
    }

    result = nanocam_snap(&snap, &reply, cam_timeout);
    if (result != NANOCAM_ERROR_NONE) {
        printf("Snap error: %d\r\n", result);
        return CMD_ERROR_FAIL;
    }

    printf("Snap result: %d\r\n", result);

    if (!histogram)
        return CMD_ERROR_NONE;

    /* Brightness */
    color_printf(COLOR_WHITE, "Brightness\r\n");
    color_printf(COLOR_NONE,  "  All   : %u%% (min/max/peak/avg %03hhu/%03hhu/%03hhu/%03hhu)\r\n",
                 reply.light_avg[0] * 100 / UINT8_MAX,
                 reply.light_min[0], reply.light_max[0],
                 reply.light_peak[0], reply.light_avg[0]);
    color_printf(COLOR_RED,   "  Red   : %u%% (min/max/peak/avg %03hhu/%03hhu/%03hhu/%03hhu)\r\n",
                 reply.light_avg[1] * 100 / UINT8_MAX,
                 reply.light_min[1], reply.light_max[1],
                 reply.light_peak[1], reply.light_avg[1]);
    color_printf(COLOR_GREEN, "  Green : %u%% (min/max/peak/avg %03hhu/%03hhu/%03hhu/%03hhu)\r\n",
                 reply.light_avg[2] * 100 / UINT8_MAX,
                 reply.light_min[2], reply.light_max[2],
                 reply.light_peak[2], reply.light_avg[2]);
    color_printf(COLOR_BLUE,  "  Blue  : %u%% (min/max/peak/avg %03hhu/%03hhu/%03hhu/%03hhu)\r\n",
                 reply.light_avg[3] * 100/ UINT8_MAX,
                 reply.light_min[3], reply.light_max[3],
                 reply.light_peak[3], reply.light_avg[3]);

    /* Histogram */
    color_printf(COLOR_WHITE, "Histogram\r\n");

    show_r = strchr(colors, 'r');
    show_g = strchr(colors, 'g');
    show_b = strchr(colors, 'b');

    /* Find max bin count */
    for (i = 0; i < NANOCAM_SNAP_HIST_BINS; i++) {
        for (j = 0; j < NANOCAM_SNAP_COLORS; j++) {
            if (reply.hist[j][i] > bin_max)
                bin_max = reply.hist[j][i];
        }
    }

    color_printf(COLOR_WHITE, "    Min +");
    for (j = 0; j < HIST_WIDTH + 1; j++)
        color_printf(COLOR_WHITE, "-");
    color_printf(COLOR_WHITE, "+\r\n");

    for (i = 0; i < NANOCAM_SNAP_HIST_BINS; i++) {
        /* Overall brightness */
        float pct = (float)reply.hist[0][i] * 100 / UINT8_MAX;
        printf(" %5.1f%% |", pct);

        /* Per-color brightness (round-up and scale to max bin count) */
        unsigned int r_blocks = (reply.hist[1][i] * HIST_WIDTH + bin_max - 1) / bin_max;
        unsigned int g_blocks = (reply.hist[2][i] * HIST_WIDTH + bin_max - 1) / bin_max;
        unsigned int b_blocks = (reply.hist[3][i] * HIST_WIDTH + bin_max - 1) / bin_max;

        /* Show bar */
        for (j = 0; j < HIST_WIDTH; j++) {
            r = (j < r_blocks) && show_r;
            g = (j < g_blocks) && show_g;
            b = (j < b_blocks) && show_b;
            if (r || g || b)
                color_printf(hist_bin_color(r, g, b), "#");
            else
                printf(" ");
        }

        printf(" | %03u-%03u\r\n",
               i * 256 / NANOCAM_SNAP_HIST_BINS,
               i * 256 / NANOCAM_SNAP_HIST_BINS + 15);
    }

    color_printf(COLOR_WHITE, "    Max +");
    for (j = 0; j < HIST_WIDTH + 1; j++)
        color_printf(COLOR_WHITE, "-");
    color_printf(COLOR_WHITE, "+\r\n");

    return CMD_ERROR_NONE;
}

int cmd_cam_store(struct command_context *ctx)
{
    int result;
    cam_store_t store;
    cam_store_reply_t reply;
    char filename[40];
    uint8_t scale = 0;

    memset(filename, 0, sizeof(filename));

    if (ctx->argc > 1) {
        strncpy(filename, ctx->argv[1], sizeof(filename) - 1);
    }
    if (ctx->argc > 2) {
        if (gs_string_to_uint8(ctx->argv[2], &scale) != GS_OK) {
            return CMD_ERROR_SYNTAX;
        }
    }

    memcpy(store.filename, filename, sizeof(store.filename));
    store.filename[sizeof(store.filename) - 1] = '\0';
    store.format = cam_format;
    store.flags = 0;
    store.scale = scale;

    result = nanocam_store(&store, &reply, cam_timeout);
    if (result != NANOCAM_ERROR_NONE) {
        printf("Store error: %d\r\n", result);
        return CMD_ERROR_FAIL;
    }

    printf("Result %"PRIu8"\r\n", reply.result);
    printf("Image 0x%08"PRIx32" %"PRIu32"\r\n",
           reply.image_ptr, reply.image_size);

    return CMD_ERROR_NONE;
}

int cmd_cam_read(struct command_context *ctx)
{
    int result;
    uint8_t reg;
    uint16_t value;

    if(gs_string_to_uint8(ctx->argv[1], &reg) != GS_OK) {
        return CMD_ERROR_SYNTAX;
    }

    result = nanocam_reg_read(reg, &value, cam_timeout);
    if (result != NANOCAM_ERROR_NONE) {
        printf("Register read error: %d\r\n", result);
        return CMD_ERROR_FAIL;
    }

    printf("Register 0x%02hhx value 0x%04hx\r\n", reg, value);

    return CMD_ERROR_NONE;
}

int cmd_cam_write(struct command_context *ctx)
{
    int result;
    uint8_t reg;
    uint16_t value;

    if(gs_string_to_uint8(ctx->argv[1], &reg) != GS_OK) {
        return CMD_ERROR_SYNTAX;
    }
    if(gs_string_to_uint16(ctx->argv[2], &value) != GS_OK) {
        return CMD_ERROR_SYNTAX;
    }

    result = nanocam_reg_write(reg, value, cam_timeout);
    if (result != NANOCAM_ERROR_NONE) {
        printf("Register write error: %d\r\n", result);
        return CMD_ERROR_FAIL;
    }

    result = nanocam_reg_read(reg, &value, cam_timeout);
    if (result != NANOCAM_ERROR_NONE) {
        printf("Register read error: %d\r\n", result);
        return CMD_ERROR_FAIL;
    }
    printf("Register 0x%02hhx value 0x%04hx\r\n", reg, value);

    return CMD_ERROR_NONE;
}

static void cam_list_cb(int seq, cam_list_element_t *elm)
{
    if (!elm) {
        printf("No images to list\r\n");
    } else {
        uint32_t time_sec = elm->time / 1000000000;
        printf("[%02u] F:%"PRIu8", T:%"PRIu32" 0x%08"PRIx32" %"PRIu32"\r\n",
               seq, elm->format, time_sec, elm->ptr, elm->size);
    }
}

int cmd_cam_list(struct command_context *ctx)
{
    int result;

    result = nanocam_img_list(cam_list_cb, cam_timeout);
    if (result != NANOCAM_ERROR_NONE) {
        printf("Image list error: %d\r\n", result);
        return CMD_ERROR_FAIL;
    }

    return CMD_ERROR_NONE;
}

int cmd_cam_list_flush(struct command_context *ctx)
{
    int result;

    result = nanocam_img_list_flush(cam_timeout);
    if (result != NANOCAM_ERROR_NONE) {
        printf("Image list flush error: %d\r\n", result);
        return CMD_ERROR_FAIL;
    }

    return CMD_ERROR_NONE;
}

int cmd_cam_focus(struct command_context *ctx)
{
    int result;
    uint32_t af;

    result = nanocam_focus(0, &af, cam_timeout);
    if (result != NANOCAM_ERROR_NONE) {
        printf("Focus error: %d\r\n", result);
        return CMD_ERROR_FAIL;
    }

    printf("Result %"PRIu32"\r\n", af);
    return CMD_ERROR_NONE;
}

int cmd_cam_recoverfs(struct command_context *ctx)
{
    int timeout = cam_timeout;

    /* It takes a while to recreate the file system, so
     * use a minimum timeout of 120s */
    if (timeout < 120000) {
        printf("warning: adjusting timeout to 120s\r\n");
        timeout = 120000;
    }

    int result = nanocam_recoverfs(timeout);
    if (result < 0) {
        printf("Recoverfs error: %d\r\n", result);
        return CMD_ERROR_FAIL;
    }

    return CMD_ERROR_NONE;
}

#if defined(__linux__)
int cmd_cam_peek(struct command_context *ctx)
{
    int result = CSP_ERR_NONE;
    uint32_t addr, len;
    struct csp_cmp_message msg;


    if(gs_string_to_uint32(ctx->argv[1], &addr) != GS_OK) {
        return CMD_ERROR_SYNTAX;
    }
    if(gs_string_to_uint32(ctx->argv[2], &len) != GS_OK) {
        return CMD_ERROR_SYNTAX;
    }

    printf("Dumping mem from cam addr 0x%"PRIx32" len %"PRIu32" timeout %"PRIu32"\r\n", addr, len, cam_timeout);

    FILE *fp_new = fopen("cam_hexdump.bin", "wb");
    if(fp_new==NULL) {
        printf("Failed to open ./cam_hexdump.bin!\r\n");
        return CMD_ERROR_FAIL;
    }
    printf("Writing to file cam_hexdump.bin (this takes a while!): \r\n");

    while(len>0) {
        int retry = 5;
        uint32_t max_msg_size = gs_min(GS_CSP_DEFAULT_MAX_PAYLOAD_SIZE, CSP_CMP_PEEK_MAX_LEN);
        msg.peek.addr = csp_hton32(addr);
        if(len > max_msg_size)
            msg.peek.len = max_msg_size;
        else
            msg.peek.len = len;

        while(retry>0)
        {
            result = csp_cmp_peek(nanocam_get_node(), cam_timeout, &msg);
            if (result == CSP_ERR_NONE)
                break;
            else
                retry--;
        }

        if (result != CSP_ERR_NONE) {
            printf("Peek error: %d ", result);
            printf("(addr=%"PRIX32" len=%"PRIu8")\r\n", msg.peek.addr, msg.peek.len);
            break;
        }

        printf(".");
        fwrite(msg.peek.data, msg.peek.len, 1, fp_new);

        len = len-msg.peek.len;
        addr = addr+msg.peek.len;
    }
    fclose(fp_new);

    if (result != CSP_ERR_NONE) {
        return CMD_ERROR_FAIL;
    } else {
        printf("Done\r\n");
        return CMD_ERROR_NONE;
    }
}
#endif

static const command_t __sub_command cam_subcommands[] = {
    {
        .name = "node",
        .help = "Get/Set CAM address in host table",
        .usage = "[node]",
        .handler = cmd_cam_node,
        .mandatory_args=0,
        .optional_args=1,
    },{
        .name = "timeout",
        .help = "Get/Set timeout",
        .usage = "<time>",
        .handler = cmd_cam_set_timeout,
        .mandatory_args=0,
        .optional_args=1,
    },{
        .name = "format",
        .help = "Get/Set store format (0=raw, 1=bmp, 2=jpg, 3=dng, 4=raw10)",
        .usage = "<format>",
        .handler = cmd_cam_format,
        .mandatory_args=0,
        .optional_args=1,
    },{
        .name = "snap",
        .help = "Snap picture",
        .usage = "[-a][-d <delay>][-h <color>][-i][-n <count>][-r][-s][-t][-x][-e] [width] [height]",
        .handler = cmd_cam_snap,
        .mandatory_args=0,
        .optional_args=15,
    },{
        .name = "store",
        .help = "Store picture",
        .usage = "[path] [scale-down]",
        .handler = cmd_cam_store,
        .mandatory_args=1,
        .optional_args=1,
    },{
        .name = "read",
        .help = "Read sensor register",
        .usage = "<reg-hex>",
        .handler = cmd_cam_read,
        .mandatory_args=1,
    },{
        .name = "write",
        .help = "Write sensor register",
        .usage = "<reg-hex> <value-hex>",
        .handler = cmd_cam_write,
        .mandatory_args=2,
    },{
        .name = "list",
        .help = "List RAM pictures",
        .handler = cmd_cam_list,
        .mandatory_args=0,
        .optional_args=0,
    },{
        .name = "flush",
        .help = "Flush RAM pictures",
        .handler = cmd_cam_list_flush,
        .mandatory_args=0,
        .optional_args=0,
    },{
        .name = "focus",
        .help = "Run focus assist algorithm",
        .handler = cmd_cam_focus,
        .mandatory_args=0,
        .optional_args=0,
    },{
        .name = "recoverfs",
        .help = "Recreate data file system",
        .handler = cmd_cam_recoverfs,
        .mandatory_args=0,
        .optional_args=0,
    }
#if defined(__linux__)
    ,{
        .name = "peek",
        .help = "Dump picture from RAM to ./cam_hexdump.bin via 'cmp peek'",
        .usage = "<address-hex> <size-dec>",
        .handler = cmd_cam_peek,
        .mandatory_args=2,
        .optional_args=0,
    }
#endif
};

static const command_t __root_command cam_commands[] = {
    {
        .name = "cam",
        .help = "NanoCam C1U client",
        .chain = INIT_CHAIN(cam_subcommands),
    }
};

void gs_nanocam_register_commands(void) {
    command_register(cam_commands);
}

void nanocam_register_commands(void) {
    gs_nanocam_register_commands();
}
