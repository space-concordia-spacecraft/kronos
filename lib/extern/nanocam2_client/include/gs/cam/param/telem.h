#ifndef GS_CAM_PARAM_TELEM_H
#define GS_CAM_PARAM_TELEM_H
/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */
/**
   @file
   Table definitions for \a telem.
*/

#ifdef __cplusplus
extern "C" {
#endif

/** Table name */
#define GS_CAM_TELEM_TABLE_NAME                          "telem"
/** Table id */
#define GS_CAM_TELEM_TABLE_ID                            4
/** Legacy table id definition */
#define GS_CAM_TELEM_TABLE_MEM_ID                        4
/** Table data size (bytes */
#define GS_CAM_TELEM_TABLE_SIZE                          46 // bytes
/** Table rows (count) */
#define GS_CAM_TELEM_TABLE_ROWS                          15

/**
   @defgroup param_addr Parameter addresses within the table.
   @{
*/
/** Address of boot-count: Number of system boots */
#define GS_CAM_TELEM_BOOT_COUNT                                          0                  // 0x0000 uint32
/** Address of image-count: Number of captured images */
#define GS_CAM_TELEM_IMAGE_COUNT                                         4                  // 0x0004 uint32
/** Address of unixtime: System time in ns */
#define GS_CAM_TELEM_UNIXTIME                                            8                  // 0x0008 uint64
/** Address of uptime: System uptime in seconds */
#define GS_CAM_TELEM_UPTIME                                             16                  // 0x0010 uint32
/** Address of loads: System loads in 5,10,15 min */
#define GS_CAM_TELEM_LOADS(idx)                                     (   20 + ( 1 * (idx)))  // 0x0014 uint8
/** Array size of loads: System loads in 5,10,15 min */
#define GS_CAM_TELEM_LOADS_ARRAY_SIZE                                    3
/** Address of freeram: Free RAM in bytes */
#define GS_CAM_TELEM_FREERAM                                            24                  // 0x0018 uint32
/** Address of procs: Number of running processes */
#define GS_CAM_TELEM_PROCS                                              28                  // 0x001c uint16
/** Address of temp1: Temperature 1 in 1/10 C */
#define GS_CAM_TELEM_TEMP1                                              30                  // 0x001e int16
/** Address of temp2: Temperature 2 in 1/10 C */
#define GS_CAM_TELEM_TEMP2                                              32                  // 0x0020 int16
/** Address of icore: Core current in mA */
#define GS_CAM_TELEM_ICORE                                              34                  // 0x0022 uint16
/** Address of iddr: DDR current in mA */
#define GS_CAM_TELEM_IDDR                                               36                  // 0x0024 uint16
/** Address of ivcc: VCC current in mA */
#define GS_CAM_TELEM_IVCC                                               38                  // 0x0026 uint16
/** Address of vddcore: Core voltage in mV */
#define GS_CAM_TELEM_VDDCORE                                            40                  // 0x0028 uint16
/** Address of vddioddr: VDD voltage in mV */
#define GS_CAM_TELEM_VDDIODDR                                           42                  // 0x002a uint16
/** Address of vcc: VCC voltage in mV */
#define GS_CAM_TELEM_VCC                                                44                  // 0x002c uint16
/**@}*/
#ifdef __cplusplus
}
#endif
#endif
