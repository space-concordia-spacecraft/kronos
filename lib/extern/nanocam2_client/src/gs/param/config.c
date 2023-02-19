/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */

#include <gs/cam/param/host/config.h>
#include <gs/util/pgm.h>

const gs_param_table_row_t gs_cam_config_table[] = {
    {.name = "csp-addr",      .addr = GS_CAM_CONFIG_CSP_ADDR,                            .type = GS_PARAM_UINT8,   .size =   1, .array_size =  1, .flags = 0},
    {.name = "csp-gateway",   .addr = GS_CAM_CONFIG_CSP_GATEWAY,                         .type = GS_PARAM_UINT8,   .size =   1, .array_size =  1, .flags = 0},
    {.name = "csp-rtable",    .addr = GS_CAM_CONFIG_CSP_RTABLE,                          .type = GS_PARAM_STRING,  .size =  96, .array_size =  1, .flags = 0},
    {.name = "can-bitrate",   .addr = GS_CAM_CONFIG_CAN_BITRATE,                         .type = GS_PARAM_UINT32,  .size =   4, .array_size =  1, .flags = 0},
    {.name = "i2c-bitrate",   .addr = GS_CAM_CONFIG_I2C_BITRATE,                         .type = GS_PARAM_UINT32,  .size =   4, .array_size =  1, .flags = 0},
    {.name = "kiss-bitrate",  .addr = GS_CAM_CONFIG_KISS_BITRATE,                        .type = GS_PARAM_UINT32,  .size =   4, .array_size =  1, .flags = 0},
    {.name = "kiss-device",   .addr = GS_CAM_CONFIG_KISS_DEVICE,                         .type = GS_PARAM_STRING,  .size =  16, .array_size =  1, .flags = 0},
};

const gs_cam_config_memory_t gs_cam_config_default_values GS_PGM_OBJECT = {
    .csp_addr = 6,
    .csp_gateway = 5,
    .csp_rtable = "",
    .can_bitrate = 1000000,
    .i2c_bitrate = 400000,
    .kiss_bitrate = 500000,
    .kiss_device = "/dev/ttyS1",
};

