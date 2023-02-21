/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/param/pp/spi/spi.h>
#include <gs/param/pp/i2c/i2c.h>
#include <gs/util/gosh/command.h>
#include <gs/util/stdio.h>
#include <gs/util/string.h>
#include <gs/util/log.h>

static bool use_checksum;

static gs_pp_t gs_pp;

static int cmd_spi_init(gs_command_context_t *ctx)
{
    uint8_t slave;
    gs_error_t error = gs_string_to_uint8(ctx->argv[1], &slave);
    if (error) {
        return error;
    }

    bool big_endian = false;
    if (ctx->argc > 2) {
        error = gs_string_to_bool(ctx->argv[2], &big_endian);
        if (error) {
            return error;
        }
    }

    error = gs_pp_spi_init(&gs_pp, slave, big_endian);
    if (error) {
        memset(&gs_pp, 0, sizeof(gs_pp));
        return error;
    }

    return GS_OK;
}

static int cmd_i2c_init(gs_command_context_t *ctx)
{
    uint8_t bus;
    gs_error_t error = gs_string_to_uint8(ctx->argv[1], &bus);
    if (error) {
        return GS_ERROR_ARG;
    }

    uint8_t addr;
    error = gs_string_to_uint8(ctx->argv[2], &addr);
    if (error) {
        return GS_ERROR_ARG;
    }

    bool big_endian = false;
    if (ctx->argc > 3) {
        error = gs_string_to_bool(ctx->argv[3], &big_endian);
        if (error) {
            return GS_ERROR_ARG;
        }
    }

    error = gs_pp_i2c_init(&gs_pp, bus, addr, big_endian);
    if (error) {
        memset(&gs_pp, 0, sizeof(gs_pp));
        return error;
    }

    return GS_OK;
}

static int cmd_checksum(gs_command_context_t *ctx)
{
    if (ctx->argc > 1) {
        if (gs_string_to_bool(ctx->argv[1], &use_checksum)) {
            return GS_ERROR_ARG;
        }
    }
    fprintf(ctx->out, "Use CHECKSUM: %d\r\n", use_checksum);
    return GS_OK;
}

// get_xxxx <table> <addr>
static int cmd_parse(gs_command_context_t *ctx,
                     uint32_t * table, uint32_t * addr)
{
    if (ctx->argc < 3) {
        return GS_ERROR_ARG;
    }

    gs_error_t error = gs_string_to_uint32(ctx->argv[1], table);
    if (error || (*table > 7)) {
        return GS_ERROR_ARG;
    }

    error = gs_string_to_uint32(ctx->argv[2], addr);
    if (error || (*addr > 255)) {
        return GS_ERROR_ARG;
    }

    return GS_OK;
}

static int cmd_get_parse(gs_command_context_t * ctx,
                         uint32_t * table, uint32_t * addr, uint32_t * count)
{
    int res = cmd_parse(ctx, table, addr);
    if (res == GS_OK) {
        *count = 1;
        if (ctx->argc > 3) {
            gs_error_t error = gs_string_to_uint32(ctx->argv[3], count);
            if (error || (*count > 100)) {
                return GS_ERROR_ARG;
            }
        }
    }

    return res;
}

#define CMD_GET(_ctx, _type, _func, _format)                            \
    {                                                                   \
        uint32_t table;                                                 \
        uint32_t addr;                                                  \
        uint32_t count;                                                 \
        int res = cmd_get_parse(_ctx, &table, &addr, &count);           \
        if (res == GS_OK) {                                             \
            _type value[count];                                         \
            gs_error_t error = _func(&gs_pp, table, addr, value, count, use_checksum ? GS_PP_FLAG_CHECKSUM : 0); \
            if (error) {                                                \
                return error;                                           \
            }                                                           \
            fprintf(_ctx->out, "value(s): ");                           \
            for (unsigned int i = 0; i < count; ++i) {                  \
                fprintf(_ctx->out, "%" _format " ", value[i]);          \
            }                                                           \
            fprintf(_ctx->out, "\r\n");                                 \
        }                                                               \
        return res;                                                     \
    }

static int cmd_get_int8(gs_command_context_t * ctx)
{
    CMD_GET(ctx, int8_t, gs_pp_get_int8, PRId8);
}

static int cmd_get_int16(gs_command_context_t * ctx)
{
    CMD_GET(ctx, int16_t, gs_pp_get_int16, PRId16);
}

static int cmd_get_int32(gs_command_context_t * ctx)
{
    CMD_GET(ctx, int32_t, gs_pp_get_int32, PRId32);
}

static int cmd_get_uint8(gs_command_context_t * ctx)
{
    CMD_GET(ctx, uint8_t, gs_pp_get_uint8, PRIu8);
}

static int cmd_get_uint16(gs_command_context_t * ctx)
{
    CMD_GET(ctx, uint16_t, gs_pp_get_uint16, PRIu16);
}

static int cmd_get_uint32(gs_command_context_t * ctx)
{
    CMD_GET(ctx, uint32_t, gs_pp_get_uint32, PRIu32);
}

static int cmd_get_float(gs_command_context_t * ctx)
{
    CMD_GET(ctx, float, gs_pp_get_float, "f");
}

#define CMD_SET(_ctx, _type, _func, _convert)                           \
    {                                                                   \
        const unsigned int  MAX_VALUES = 20;                            \
        uint32_t table;                                                 \
        uint32_t addr;                                                  \
        int res = cmd_parse(_ctx, &table, &addr);                       \
        if (res == GS_OK) {                                             \
            _type value[MAX_VALUES];                                    \
            unsigned int count = 0;                                     \
            for (int i = 3; (i < _ctx->argc) && (count < MAX_VALUES); ++i, ++count) { \
                res = _convert(_ctx->argv[i], &value[count]);           \
                if (res) {                                              \
                    return GS_ERROR_DATA;                               \
                }                                                       \
            }                                                           \
            res = _func(&gs_pp, table, addr, value, count, use_checksum ? GS_PP_FLAG_CHECKSUM : 0); \
        }                                                               \
        return res;                                                     \
    }

static int cmd_set_int8(gs_command_context_t * ctx)
{
    CMD_SET(ctx, int8_t, gs_pp_set_int8, gs_string_to_int8);
}

static int cmd_set_int16(gs_command_context_t * ctx)
{
    CMD_SET(ctx, int16_t, gs_pp_set_int16, gs_string_to_int16);
}

static int cmd_set_int32(gs_command_context_t * ctx)
{
    CMD_SET(ctx, int32_t, gs_pp_set_int32, gs_string_to_int32);
}

static int cmd_set_uint8(gs_command_context_t * ctx)
{
    CMD_SET(ctx, uint8_t, gs_pp_set_uint8, gs_string_to_uint8);
}

static int cmd_set_uint16(gs_command_context_t * ctx)
{
    CMD_SET(ctx, uint16_t, gs_pp_set_uint16, gs_string_to_uint16);
}

static int cmd_set_uint32(gs_command_context_t * ctx)
{
    CMD_SET(ctx, uint32_t, gs_pp_set_uint32, gs_string_to_uint32);
}

static int cmd_set_float(gs_command_context_t * ctx)
{
    CMD_SET(ctx, float, gs_pp_set_float, gs_string_to_float);
}

static int cmd_get_table_lock(gs_command_context_t * ctx)
{
    uint32_t table;
    gs_error_t res = gs_string_to_uint32(ctx->argv[1], &table);
    if (res == GS_OK) {
        if (table <= 7) {
            bool lock;
            res = gs_pp_get_table_lock(&gs_pp, table, &lock, GS_PP_FLAG_CHECKSUM);
            if (res == GS_OK) {
                fprintf(ctx->out, "Table %s\r\n", lock ? "locked" : "unlocked");
            }
        } else {
            res = GS_ERROR_ARG;
        }
    }
    return res;
}

static int cmd_set_table_lock(gs_command_context_t * ctx)
{
    uint32_t table;
    gs_error_t res = gs_string_to_uint32(ctx->argv[1], &table);
    if (res == GS_OK) {
        if (table <= 7) {
            uint32_t lock;
            res = gs_string_to_uint32(ctx->argv[2], &lock);
            if (res == GS_OK) {
                if(lock <= 1) {
                    res = gs_pp_set_table_lock(&gs_pp, table, (bool *)&lock, GS_PP_FLAG_CHECKSUM);
                } else {
                    res = GS_ERROR_ARG;
                }
            }
        } else {
            res = GS_ERROR_ARG;
        }
    }
    return res;
}

static const gs_command_t gs_param_cmd_master_pp_sub[] = {
    {
        .name = "spi_init",
        .help = "Initialize/setup SPI device",
        .usage = "<slave> [big_endian]",
        .handler = cmd_spi_init,
        .mandatory_args = 1,
        .optional_args = 1,
    },{
        .name = "i2c_init",
        .help = "Initialize/setup I2C device",
        .usage = "<device> <addr> [big_endian]",
        .handler = cmd_i2c_init,
        .mandatory_args = 2,
        .optional_args = 1,
    },{
        .name = "checksum",
        .help = "Enable/disable checksum",
        .usage = "[0|1]",
        .handler = cmd_checksum,
        .optional_args = 1,
    },{
        .name = "get_table_lock",
        .help = "Get table lock (0 = unlocked, 1 = locked)",
        .usage = "<table>",
        .mandatory_args = 1,
        .handler = cmd_get_table_lock,
    },{
        .name = "set_table_lock",
        .help = "Set table lock (0 = unlocked, 1 = locked)",
        .usage = "<table> <lock/unlock>",
        .mandatory_args = 2,
        .handler = cmd_set_table_lock,
    },{
        .name = "get_int8",
        .help = "Get int8",
        .usage = "<table> <addr> [count]",
        .handler = cmd_get_int8,
        .mandatory_args = 2,
        .optional_args = 1,
    },{
        .name = "get_int16",
        .help = "Get int16",
        .usage = "<table> <addr> [count]",
        .handler = cmd_get_int16,
        .mandatory_args = 2,
        .optional_args = 1,
    },{
        .name = "get_int32",
        .help = "Get int32",
        .usage = "<table> <addr> [count]",
        .handler = cmd_get_int32,
        .mandatory_args = 2,
        .optional_args = 1,
    },{
        .name = "get_uint8",
        .help = "Get uint8",
        .usage = "<table> <addr> [count]",
        .handler = cmd_get_uint8,
        .mandatory_args = 2,
        .optional_args = 1,
    },{
        .name = "get_uint16",
        .help = "Get uint16",
        .usage = "<table> <addr> [count]",
        .handler = cmd_get_uint16,
        .mandatory_args = 2,
        .optional_args = 1,
    },{
        .name = "get_uint32",
        .help = "Get uint32",
        .usage = "<table> <addr> [count]",
        .handler = cmd_get_uint32,
        .mandatory_args = 2,
        .optional_args = 1,
    },{
        .name = "get_float",
        .help = "Get float",
        .usage = "<table> <addr> [count]",
        .handler = cmd_get_float,
        .mandatory_args = 2,
        .optional_args = 1,
    },{
        .name = "set_int8",
        .help = "Set int8",
        .usage = "<table> <addr> <data> [data] ...",
        .handler = cmd_set_int8,
        .mandatory_args = 3,
        .optional_args = 20,
    },{
        .name = "set_int16",
        .help = "Set int16",
        .usage = "<table> <addr> <data> [data] ...",
        .handler = cmd_set_int16,
        .mandatory_args = 3,
        .optional_args = 20,
    },{
        .name = "set_int32",
        .help = "Set int32",
        .usage = "<table> <addr> <data> [data] ...",
        .handler = cmd_set_int32,
        .mandatory_args = 3,
        .optional_args = 20,
    },{
        .name = "set_uint8",
        .help = "Set uint8",
        .usage = "<table> <addr> <data> [data] ...",
        .handler = cmd_set_uint8,
        .mandatory_args = 3,
        .optional_args = 20,
    },{
        .name = "set_uint16",
        .help = "Set uint16",
        .usage = "<table> <addr> <data> [data] ...",
        .handler = cmd_set_uint16,
        .mandatory_args = 3,
        .optional_args = 20,
    },{
        .name = "set_uint32",
        .help = "Set uint32",
        .usage = "<table> <addr> <data> [data] ...",
        .handler = cmd_set_uint32,
        .mandatory_args = 3,
        .optional_args = 20,
    },{
        .name = "set_float",
        .help = "Set float",
        .usage = "<table> <addr> <data> [data] ...",
        .handler = cmd_set_float,
        .mandatory_args = 3,
        .optional_args = 20,
    }
};

static const gs_command_t GS_COMMAND_ROOT gs_param_cmd_master_pp[] = {
    {
        .name = "pp",
        .help = "Param Protocol interface",
        .chain = GS_COMMAND_INIT_CHAIN(gs_param_cmd_master_pp_sub),
    }
};

gs_error_t gs_pp_register_commands(void)
{
    return GS_COMMAND_REGISTER(gs_param_cmd_master_pp);
}
