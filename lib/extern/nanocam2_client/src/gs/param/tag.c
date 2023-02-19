/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */

#include <gs/cam/param/tag.h>
#include <gs/util/pgm.h>
#include "gs/param/types.h"

const gs_param_table_row_t gs_cam_tag_table[] = {
    {.name = "tag-adcs",      .addr = GS_CAM_TAG_TAG_ADCS,                               .type = GS_PARAM_BOOL,    .size =   1, .array_size =  1, .flags = 0},
    {.name = "tag-adcs-host", .addr = GS_CAM_TAG_TAG_ADCS_HOST,                          .type = GS_PARAM_UINT8,   .size =   1, .array_size =  1, .flags = 0},
    {.name = "tag-adcs-port", .addr = GS_CAM_TAG_TAG_ADCS_PORT,                          .type = GS_PARAM_UINT8,   .size =   1, .array_size =  1, .flags = 0},
    {.name = "tag-adcs-tout", .addr = GS_CAM_TAG_TAG_ADCS_TOUT,                          .type = GS_PARAM_UINT16,  .size =   2, .array_size =  1, .flags = 0},
};

//const gs_cam_tag_memory_t gs_cam_tag_default_values GS_PGM_OBJECT = {
//    .tag_adcs = false,
//    .tag_adcs_host = 4,
//    .tag_adcs_port = 20,
//    .tag_adcs_tout = 200,
//};

