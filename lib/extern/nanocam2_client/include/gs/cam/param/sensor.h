#ifndef GS_CAM_PARAM_SENSOR_H
#define GS_CAM_PARAM_SENSOR_H
/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */
/**
   @file
   Table definitions for \a sensor.
*/

#ifdef __cplusplus
extern "C" {
#endif

/** Table name */
#define GS_CAM_SENSOR_TABLE_NAME                         "sensor"
/** Table id */
#define GS_CAM_SENSOR_TABLE_ID                           2
/** Legacy table id definition */
#define GS_CAM_SENSOR_TABLE_MEM_ID                       2
/** Table data size (bytes */
#define GS_CAM_SENSOR_TABLE_SIZE                         72 // bytes
/** Table rows (count) */
#define GS_CAM_SENSOR_TABLE_ROWS                         36

/**
   @defgroup param_addr Parameter addresses within the table.
   @{
*/
/** Address of chip-version: sensor chip register */
#define GS_CAM_SENSOR_CHIP_VERSION                                       0                  // 0x0000 uint16
/** Address of row-start: sensor chip register */
#define GS_CAM_SENSOR_ROW_START                                          2                  // 0x0002 uint16
/** Address of col-start: sensor chip register */
#define GS_CAM_SENSOR_COL_START                                          4                  // 0x0004 uint16
/** Address of row-size: sensor chip register */
#define GS_CAM_SENSOR_ROW_SIZE                                           6                  // 0x0006 uint16
/** Address of col-size: sensor chip register */
#define GS_CAM_SENSOR_COL_SIZE                                           8                  // 0x0008 uint16
/** Address of horz-blank: sensor chip register */
#define GS_CAM_SENSOR_HORZ_BLANK                                        10                  // 0x000a uint16
/** Address of vert-blank: sensor chip register */
#define GS_CAM_SENSOR_VERT_BLANK                                        12                  // 0x000c uint16
/** Address of output-ctrl: sensor chip register */
#define GS_CAM_SENSOR_OUTPUT_CTRL                                       14                  // 0x000e uint16
/** Address of sh-width-up: sensor chip register */
#define GS_CAM_SENSOR_SH_WIDTH_UP                                       16                  // 0x0010 uint16
/** Address of sh-width: sensor chip register */
#define GS_CAM_SENSOR_SH_WIDTH                                          18                  // 0x0012 uint16
/** Address of pixclk-ctrl: sensor chip register */
#define GS_CAM_SENSOR_PIXCLK_CTRL                                       20                  // 0x0014 uint16
/** Address of frame-rst: sensor chip register */
#define GS_CAM_SENSOR_FRAME_RST                                         22                  // 0x0016 uint16
/** Address of shutter-delay: sensor chip register */
#define GS_CAM_SENSOR_SHUTTER_DELAY                                     24                  // 0x0018 uint16
/** Address of reset: sensor chip register */
#define GS_CAM_SENSOR_RESET                                             26                  // 0x001a uint16
/** Address of read-mode1: sensor chip register */
#define GS_CAM_SENSOR_READ_MODE1                                        28                  // 0x001c uint16
/** Address of read-mode2: sensor chip register */
#define GS_CAM_SENSOR_READ_MODE2                                        30                  // 0x001e uint16
/** Address of read-mode3: sensor chip register */
#define GS_CAM_SENSOR_READ_MODE3                                        32                  // 0x0020 uint16
/** Address of row-addr-mode: sensor chip register */
#define GS_CAM_SENSOR_ROW_ADDR_MODE                                     34                  // 0x0022 uint16
/** Address of col-addr-mode: sensor chip register */
#define GS_CAM_SENSOR_COL_ADDR_MODE                                     36                  // 0x0024 uint16
/** Address of green1-gain: sensor chip register */
#define GS_CAM_SENSOR_GREEN1_GAIN                                       38                  // 0x0026 uint16
/** Address of blue-gain: sensor chip register */
#define GS_CAM_SENSOR_BLUE_GAIN                                         40                  // 0x0028 uint16
/** Address of red-gain: sensor chip register */
#define GS_CAM_SENSOR_RED_GAIN                                          42                  // 0x002a uint16
/** Address of green2-gain: sensor chip register */
#define GS_CAM_SENSOR_GREEN2_GAIN                                       44                  // 0x002c uint16
/** Address of testdata: sensor chip register */
#define GS_CAM_SENSOR_TESTDATA                                          46                  // 0x002e uint16
/** Address of global-gain: sensor chip register */
#define GS_CAM_SENSOR_GLOBAL_GAIN                                       48                  // 0x0030 uint16
/** Address of black-level: sensor chip register */
#define GS_CAM_SENSOR_BLACK_LEVEL                                       50                  // 0x0032 uint16
/** Address of row-blk-off: sensor chip register */
#define GS_CAM_SENSOR_ROW_BLK_OFF                                       52                  // 0x0034 uint16
/** Address of blc-delta: sensor chip register */
#define GS_CAM_SENSOR_BLC_DELTA                                         54                  // 0x0036 uint16
/** Address of cal-thres: sensor chip register */
#define GS_CAM_SENSOR_CAL_THRES                                         56                  // 0x0038 uint16
/** Address of green1-offset: sensor chip register */
#define GS_CAM_SENSOR_GREEN1_OFFSET                                     58                  // 0x003a uint16
/** Address of green2-offset: sensor chip register */
#define GS_CAM_SENSOR_GREEN2_OFFSET                                     60                  // 0x003c uint16
/** Address of blc: sensor chip register */
#define GS_CAM_SENSOR_BLC                                               62                  // 0x003e uint16
/** Address of red-offset: sensor chip register */
#define GS_CAM_SENSOR_RED_OFFSET                                        64                  // 0x0040 uint16
/** Address of blue-offset: sensor chip register */
#define GS_CAM_SENSOR_BLUE_OFFSET                                       66                  // 0x0042 uint16
/** Address of chip-enable: sensor chip register */
#define GS_CAM_SENSOR_CHIP_ENABLE                                       68                  // 0x0044 uint16
/** Address of chip-version2: sensor chip register */
#define GS_CAM_SENSOR_CHIP_VERSION2                                     70                  // 0x0046 uint16
/**@}*/
#ifdef __cplusplus
}
#endif
#endif
