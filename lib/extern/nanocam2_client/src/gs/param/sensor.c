/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */

#include <gs/cam/param/sensor.h>
#include <gs/util/pgm.h>
#include "gs/param/types.h"

const gs_param_table_row_t gs_cam_sensor_table[] = {
    {.name = "chip-version",  .addr = GS_CAM_SENSOR_CHIP_VERSION,                        .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "row-start",     .addr = GS_CAM_SENSOR_ROW_START,                           .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "col-start",     .addr = GS_CAM_SENSOR_COL_START,                           .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "row-size",      .addr = GS_CAM_SENSOR_ROW_SIZE,                            .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "col-size",      .addr = GS_CAM_SENSOR_COL_SIZE,                            .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "horz-blank",    .addr = GS_CAM_SENSOR_HORZ_BLANK,                          .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "vert-blank",    .addr = GS_CAM_SENSOR_VERT_BLANK,                          .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "output-ctrl",   .addr = GS_CAM_SENSOR_OUTPUT_CTRL,                         .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "sh-width-up",   .addr = GS_CAM_SENSOR_SH_WIDTH_UP,                         .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "sh-width",      .addr = GS_CAM_SENSOR_SH_WIDTH,                            .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "pixclk-ctrl",   .addr = GS_CAM_SENSOR_PIXCLK_CTRL,                         .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "frame-rst",     .addr = GS_CAM_SENSOR_FRAME_RST,                           .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "shutter-delay", .addr = GS_CAM_SENSOR_SHUTTER_DELAY,                       .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "reset",         .addr = GS_CAM_SENSOR_RESET,                               .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "read-mode1",    .addr = GS_CAM_SENSOR_READ_MODE1,                          .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "read-mode2",    .addr = GS_CAM_SENSOR_READ_MODE2,                          .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "read-mode3",    .addr = GS_CAM_SENSOR_READ_MODE3,                          .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "row-addr-mode", .addr = GS_CAM_SENSOR_ROW_ADDR_MODE,                       .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "col-addr-mode", .addr = GS_CAM_SENSOR_COL_ADDR_MODE,                       .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "green1-gain",   .addr = GS_CAM_SENSOR_GREEN1_GAIN,                         .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "blue-gain",     .addr = GS_CAM_SENSOR_BLUE_GAIN,                           .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "red-gain",      .addr = GS_CAM_SENSOR_RED_GAIN,                            .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "green2-gain",   .addr = GS_CAM_SENSOR_GREEN2_GAIN,                         .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "testdata",      .addr = GS_CAM_SENSOR_TESTDATA,                            .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "global-gain",   .addr = GS_CAM_SENSOR_GLOBAL_GAIN,                         .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "black-level",   .addr = GS_CAM_SENSOR_BLACK_LEVEL,                         .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "row-blk-off",   .addr = GS_CAM_SENSOR_ROW_BLK_OFF,                         .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "blc-delta",     .addr = GS_CAM_SENSOR_BLC_DELTA,                           .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "cal-thres",     .addr = GS_CAM_SENSOR_CAL_THRES,                           .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "green1-offset", .addr = GS_CAM_SENSOR_GREEN1_OFFSET,                       .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "green2-offset", .addr = GS_CAM_SENSOR_GREEN2_OFFSET,                       .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "blc",           .addr = GS_CAM_SENSOR_BLC,                                 .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "red-offset",    .addr = GS_CAM_SENSOR_RED_OFFSET,                          .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "blue-offset",   .addr = GS_CAM_SENSOR_BLUE_OFFSET,                         .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "chip-enable",   .addr = GS_CAM_SENSOR_CHIP_ENABLE,                         .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
    {.name = "chip-version2", .addr = GS_CAM_SENSOR_CHIP_VERSION2,                       .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = GS_PARAM_F_SHOW_HEX},
};

