/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */

#include <gs/cam/param/host/telem.h>
#include <gs/util/pgm.h>

const gs_param_table_row_t gs_cam_telem_table[] = {
    {.name = "boot-count",    .addr = GS_CAM_TELEM_BOOT_COUNT,                           .type = GS_PARAM_UINT32,  .size =   4, .array_size =  1, .flags = GS_PARAM_F_AUTO_PERSIST},
    {.name = "image-count",   .addr = GS_CAM_TELEM_IMAGE_COUNT,                          .type = GS_PARAM_UINT32,  .size =   4, .array_size =  1, .flags = GS_PARAM_F_AUTO_PERSIST},
    {.name = "unixtime",      .addr = GS_CAM_TELEM_UNIXTIME,                             .type = GS_PARAM_UINT64,  .size =   8, .array_size =  1, .flags = 0},
    {.name = "uptime",        .addr = GS_CAM_TELEM_UPTIME,                               .type = GS_PARAM_UINT32,  .size =   4, .array_size =  1, .flags = 0},
    {.name = "loads",         .addr = GS_CAM_TELEM_LOADS(0),                             .type = GS_PARAM_UINT8,   .size =   1, .array_size =  3, .flags = 0},
    {.name = "freeram",       .addr = GS_CAM_TELEM_FREERAM,                              .type = GS_PARAM_UINT32,  .size =   4, .array_size =  1, .flags = 0},
    {.name = "procs",         .addr = GS_CAM_TELEM_PROCS,                                .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = 0},
    {.name = "temp1",         .addr = GS_CAM_TELEM_TEMP1,                                .type = GS_PARAM_INT16,   .size =   2, .array_size =  1, .flags = 0},
    {.name = "temp2",         .addr = GS_CAM_TELEM_TEMP2,                                .type = GS_PARAM_INT16,   .size =   2, .array_size =  1, .flags = 0},
    {.name = "icore",         .addr = GS_CAM_TELEM_ICORE,                                .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = 0},
    {.name = "iddr",          .addr = GS_CAM_TELEM_IDDR,                                 .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = 0},
    {.name = "ivcc",          .addr = GS_CAM_TELEM_IVCC,                                 .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = 0},
    {.name = "vddcore",       .addr = GS_CAM_TELEM_VDDCORE,                              .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = 0},
    {.name = "vddioddr",      .addr = GS_CAM_TELEM_VDDIODDR,                             .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = 0},
    {.name = "vcc",           .addr = GS_CAM_TELEM_VCC,                                  .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = 0},
};

