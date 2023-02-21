#ifndef GS_CAM_PARAM_TAG_H
#define GS_CAM_PARAM_TAG_H
/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */
/**
   @file
   Table definitions for \a tag.
*/

#ifdef __cplusplus
extern "C" {
#endif

/** Table name */
#define GS_CAM_TAG_TABLE_NAME                            "tag"
/** Table id */
#define GS_CAM_TAG_TABLE_ID                              3
/** Legacy table id definition */
#define GS_CAM_TAG_TABLE_MEM_ID                          3
/** Table data size (bytes */
#define GS_CAM_TAG_TABLE_SIZE                            6 // bytes
/** Table rows (count) */
#define GS_CAM_TAG_TABLE_ROWS                            4

/**
   @defgroup param_addr Parameter addresses within the table.
   @{
*/
/** Address of tag-adcs: Add ADCS tags to tag file */
#define GS_CAM_TAG_TAG_ADCS                                              0                  // 0x0000 bool
/** Address of tag-adcs-host: ADCS tagging host */
#define GS_CAM_TAG_TAG_ADCS_HOST                                         1                  // 0x0001 uint8
/** Address of tag-adcs-port: ADCS tagging port */
#define GS_CAM_TAG_TAG_ADCS_PORT                                         2                  // 0x0002 uint8
/** Address of tag-adcs-tout: ADCS tagging timeout in ms */
#define GS_CAM_TAG_TAG_ADCS_TOUT                                         4                  // 0x0004 uint16
/**@}*/
#ifdef __cplusplus
}
#endif
#endif
