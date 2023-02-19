#ifndef GS_CAM_PARAM_CONFIG_H
#define GS_CAM_PARAM_CONFIG_H
/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */
/**
   @file
   Table definitions for \a config.
*/

#ifdef __cplusplus
extern "C" {
#endif

/** Table name */
#define GS_CAM_CONFIG_TABLE_NAME                         "config"
/** Table id */
#define GS_CAM_CONFIG_TABLE_ID                           0
/** Legacy table id definition */
#define GS_CAM_CONFIG_TABLE_MEM_ID                       0
/** Table data size (bytes */
#define GS_CAM_CONFIG_TABLE_SIZE                         124 // bytes
/** Table rows (count) */
#define GS_CAM_CONFIG_TABLE_ROWS                         5

/**
   @defgroup param_addr Parameter addresses within the table.
   @{
*/
/** Address of csp-addr: CSP address */
#define GS_CAM_CONFIG_CSP_ADDR                                           0                  // 0x0000 uint8
/** Address of csp-rtable: CSP routing table */
#define GS_CAM_CONFIG_CSP_RTABLE                                         1                  // 0x0001 string
/** Length/size of csp-rtable (bytes): CSP routing table */
#define GS_CAM_CONFIG_CSP_RTABLE_LENGTH                                 96
/** Address of can-bitrate: CAN bitrate */
#define GS_CAM_CONFIG_CAN_BITRATE                                      100                  // 0x0064 uint32
/** Address of kiss-bitrate: KISS bitrate */
#define GS_CAM_CONFIG_KISS_BITRATE                                     104                  // 0x0068 uint32
/** Address of kiss-device: KISS uart (ttyS1 or ttyS2) */
#define GS_CAM_CONFIG_KISS_DEVICE                                      108                  // 0x006c string
/** Length/size of kiss-device (bytes): KISS uart (ttyS1 or ttyS2) */
#define GS_CAM_CONFIG_KISS_DEVICE_LENGTH                                16
/**@}*/
#ifdef __cplusplus
}
#endif
#endif
