/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */

#include <gs/cam/param/image.h>
#include <gs/util/pgm.h>
#include "gs/param/types.h"

const gs_param_table_row_t gs_cam_image_table[] = {
    {.name = "exposure-us",   .addr = GS_CAM_IMAGE_EXPOSURE_US,                          .type = GS_PARAM_UINT32,  .size =   4, .array_size =  1, .flags = 0},
    {.name = "hist-skip",     .addr = GS_CAM_IMAGE_HIST_SKIP,                            .type = GS_PARAM_UINT8,   .size =   1, .array_size =  1, .flags = 0},
    {.name = "gain-target",   .addr = GS_CAM_IMAGE_GAIN_TARGET,                          .type = GS_PARAM_UINT8,   .size =   1, .array_size =  1, .flags = 0},
    {.name = "gain-slack",    .addr = GS_CAM_IMAGE_GAIN_SLACK,                           .type = GS_PARAM_UINT8,   .size =   1, .array_size =  1, .flags = 0},
    {.name = "gain-iters",    .addr = GS_CAM_IMAGE_GAIN_ITERS,                           .type = GS_PARAM_UINT8,   .size =   1, .array_size =  1, .flags = 0},
    {.name = "gain-global",   .addr = GS_CAM_IMAGE_GAIN_GLOBAL,                          .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = 0},
    {.name = "gain-red",      .addr = GS_CAM_IMAGE_GAIN_RED,                             .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = 0},
    {.name = "gain-green",    .addr = GS_CAM_IMAGE_GAIN_GREEN,                           .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = 0},
    {.name = "gain-blue",     .addr = GS_CAM_IMAGE_GAIN_BLUE,                            .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = 0},
    {.name = "gain-max",      .addr = GS_CAM_IMAGE_GAIN_MAX,                             .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = 0},
    {.name = "jpeg-qual",     .addr = GS_CAM_IMAGE_JPEG_QUAL,                            .type = GS_PARAM_UINT8,   .size =   1, .array_size =  1, .flags = 0},
    {.name = "jpeg-prog",     .addr = GS_CAM_IMAGE_JPEG_PROG,                            .type = GS_PARAM_BOOL,    .size =   1, .array_size =  1, .flags = 0},
    {.name = "bayer-algo",    .addr = GS_CAM_IMAGE_BAYER_ALGO,                           .type = GS_PARAM_UINT8,   .size =   1, .array_size =  1, .flags = 0},
    {.name = "color-correct", .addr = GS_CAM_IMAGE_COLOR_CORRECT,                        .type = GS_PARAM_BOOL,    .size =   1, .array_size =  1, .flags = 0},
    {.name = "gamma-correct", .addr = GS_CAM_IMAGE_GAMMA_CORRECT,                        .type = GS_PARAM_BOOL,    .size =   1, .array_size =  1, .flags = 0},
    {.name = "white-balance", .addr = GS_CAM_IMAGE_WHITE_BALANCE,                        .type = GS_PARAM_BOOL,    .size =   1, .array_size =  1, .flags = 0},
    {.name = "gamma",         .addr = GS_CAM_IMAGE_GAMMA,                                .type = GS_PARAM_FLOAT,   .size =   4, .array_size =  1, .flags = 0},
    {.name = "gamma-break",   .addr = GS_CAM_IMAGE_GAMMA_BREAK,                          .type = GS_PARAM_FLOAT,   .size =   4, .array_size =  1, .flags = 0},
    {.name = "ccm-red",       .addr = GS_CAM_IMAGE_CCM_RED(0),                           .type = GS_PARAM_FLOAT,   .size =   4, .array_size =  3, .flags = 0},
    {.name = "ccm-green",     .addr = GS_CAM_IMAGE_CCM_GREEN(0),                         .type = GS_PARAM_FLOAT,   .size =   4, .array_size =  3, .flags = 0},
    {.name = "ccm-blue",      .addr = GS_CAM_IMAGE_CCM_BLUE(0),                          .type = GS_PARAM_FLOAT,   .size =   4, .array_size =  3, .flags = 0},
    {.name = "thumb-scale",   .addr = GS_CAM_IMAGE_THUMB_SCALE,                          .type = GS_PARAM_UINT8,   .size =   1, .array_size =  1, .flags = 0},
};

const gs_cam_image_memory_t gs_cam_image_default_values GS_PGM_OBJECT = {
    .exposure_us = 10000,
    .hist_skip = 3,
    .gain_target = 30,
    .gain_slack = 3,
    .gain_iters = 10,
    .gain_global = 2048,
    .gain_red = 2048,
    .gain_green = 2048,
    .gain_blue = 2048,
    .gain_max = 65535,
    .jpeg_qual = 85,
    .jpeg_prog = false,
    .bayer_algo = 2,
    .color_correct = true,
    .gamma_correct = true,
    .white_balance = false,
    .gamma = 2.2,
    .gamma_break = 0.1,
    .ccm_red = {1.6809, 0.1496, -0.6606},
    .ccm_green = {-0.7613, 2.4481, -0.5168},
    .ccm_blue = {-0.3891, -0.5243, 2.0834},
    .thumb_scale = 8,
};

