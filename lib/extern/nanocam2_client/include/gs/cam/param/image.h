#ifndef GS_CAM_PARAM_IMAGE_H
#define GS_CAM_PARAM_IMAGE_H
/* Copyright (c) 2013 GomSpace A/S. All rights reserved. (AUTO GENERATED) */
/**
   @file
   Table definitions for \a image.
*/

#ifdef __cplusplus
extern "C" {
#endif

/** Table name */
#define GS_CAM_IMAGE_TABLE_NAME                          "image"
/** Table id */
#define GS_CAM_IMAGE_TABLE_ID                            1
/** Legacy table id definition */
#define GS_CAM_IMAGE_TABLE_MEM_ID                        1
/** Table data size (bytes */
#define GS_CAM_IMAGE_TABLE_SIZE                          70 // bytes
/** Table rows (count) */
#define GS_CAM_IMAGE_TABLE_ROWS                          23

/**
   @defgroup param_addr Parameter addresses within the table.
   @{
*/
/** Address of exposure-us: Exposure time in us */
#define GS_CAM_IMAGE_EXPOSURE_US                                         0                  // 0x0000 uint32
/** Address of hist-skip: Histogram calculation skip */
#define GS_CAM_IMAGE_HIST_SKIP                                           4                  // 0x0004 uint8
/** Address of gain-target: Auto-gain target */
#define GS_CAM_IMAGE_GAIN_TARGET                                         5                  // 0x0005 uint8
/** Address of gain-slack: Auto-gain target tolerance */
#define GS_CAM_IMAGE_GAIN_SLACK                                          6                  // 0x0006 uint8
/** Address of gain-iters: Max auto-gain iterations */
#define GS_CAM_IMAGE_GAIN_ITERS                                          7                  // 0x0007 uint8
/** Address of gain-global: Global gain (0-65535), will overwrite gain-red, green and blue */
#define GS_CAM_IMAGE_GAIN_GLOBAL                                         8                  // 0x0008 uint16
/** Address of gain-red: Red channel gain (0-65535), when using -a option gain-global is used */
#define GS_CAM_IMAGE_GAIN_RED                                           10                  // 0x000a uint16
/** Address of gain-green: Green channel gain (0-65535), when using -a option gain-global is used */
#define GS_CAM_IMAGE_GAIN_GREEN                                         12                  // 0x000c uint16
/** Address of gain-blue: Blue channel gain (0-65535), when using -a option gain-global is used */
#define GS_CAM_IMAGE_GAIN_BLUE                                          14                  // 0x000e uint16
/** Address of gain-max: Maximum auto-gain (0-65535) */
#define GS_CAM_IMAGE_GAIN_MAX                                           16                  // 0x0010 uint16
/** Address of jpeg-qual: JPEG quality setting (0-100) */
#define GS_CAM_IMAGE_JPEG_QUAL                                          18                  // 0x0012 uint8
/** Address of jpeg-prog: Store progressive JPEG */
#define GS_CAM_IMAGE_JPEG_PROG                                          19                  // 0x0013 bool
/** Address of bayer-algo: Bayer demosaicing algorithm */
#define GS_CAM_IMAGE_BAYER_ALGO                                         20                  // 0x0014 uint8
/** Address of color-correct: Perform color correction */
#define GS_CAM_IMAGE_COLOR_CORRECT                                      21                  // 0x0015 bool
/** Address of gamma-correct: Perform gamma correction */
#define GS_CAM_IMAGE_GAMMA_CORRECT                                      22                  // 0x0016 bool
/** Address of white-balance: Perform white balancing */
#define GS_CAM_IMAGE_WHITE_BALANCE                                      23                  // 0x0017 bool
/** Address of gamma: Gamma-correction */
#define GS_CAM_IMAGE_GAMMA                                              24                  // 0x0018 float
/** Address of gamma-break: Linear gamma breakpoint */
#define GS_CAM_IMAGE_GAMMA_BREAK                                        28                  // 0x001c float
/** Address of ccm-red: Color-correction, red */
#define GS_CAM_IMAGE_CCM_RED(idx)                                   (   32 + ( 4 * (idx)))  // 0x0020 float
/** Array size of ccm-red: Color-correction, red */
#define GS_CAM_IMAGE_CCM_RED_ARRAY_SIZE                                  3
/** Address of ccm-green: Color-correction, green */
#define GS_CAM_IMAGE_CCM_GREEN(idx)                                 (   44 + ( 4 * (idx)))  // 0x002c float
/** Array size of ccm-green: Color-correction, green */
#define GS_CAM_IMAGE_CCM_GREEN_ARRAY_SIZE                                3
/** Address of ccm-blue: Color-correction, blue */
#define GS_CAM_IMAGE_CCM_BLUE(idx)                                  (   56 + ( 4 * (idx)))  // 0x0038 float
/** Array size of ccm-blue: Color-correction, blue */
#define GS_CAM_IMAGE_CCM_BLUE_ARRAY_SIZE                                 3
/** Address of thumb-scale: Thumbnail scaling factor */
#define GS_CAM_IMAGE_THUMB_SCALE                                        68                  // 0x0044 uint8
/** Address of led-en: LED enable */
#define GS_CAM_IMAGE_LED_EN                                             69                  // 0x0045 bool
/**@}*/
#ifdef __cplusplus
}
#endif
#endif
