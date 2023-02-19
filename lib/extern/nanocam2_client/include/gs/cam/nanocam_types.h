#ifndef __NANOCAM_TYPES_H__
#define __NANOCAM_TYPES_H__
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   NanoCam Network Types.
*/

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Default CSP address */
#define NANOCAM_DEFAULT_ADDRESS		6

/* Specific ports */
#define NANOCAM_PORT_SNAP		20 /** Snap picture port */
#define NANOCAM_PORT_STORE		21 /** Store picture port */
#define NANOCAM_PORT_IMG_LIST		22 /** List RAM pictures port */
#define NANOCAM_PORT_IMG_FLUSH		23 /** Flush RAM pictures port */
#define NANOCAM_PORT_REG_READ		24 /** Read sensor register port */
#define NANOCAM_PORT_REG_WRITE		25 /** Write sensor register port */
#define NANOCAM_PORT_FOCUS		26 /** Focus assist port */
#define NANOCAM_PORT_REMOTE_SHELL	27 /** Remote Linux shell port */
#define NANOCAM_PORT_RECOVERFS		28 /** File systems recovery port */
#define NANOCAM_PORT_GSCRIPT		29 /** Gscript server port */


/* Network data types */

/* Error codes */
#define NANOCAM_ERROR_NONE		0 /** No error */
#define NANOCAM_ERROR_NOMEM		1 /** Not enough space */
#define NANOCAM_ERROR_INVAL		2 /** Invalid value */
#define NANOCAM_ERROR_IOERR		3 /** I/O error */
#define NANOCAM_ERROR_NOENT		4 /** No such file or directory */

/* NANOCAM_PORT_SNAP */
#define NANOCAM_SNAP_FLAG_AUTO_GAIN     (1 <<  0) /** Automatically adjust gain */
#define NANOCAM_SNAP_FLAG_STORE_RAM     (1 <<  8) /** Store snapped image to RAM */
#define NANOCAM_SNAP_FLAG_STORE_FLASH   (1 <<  9) /** Store snapped image to flash */
#define NANOCAM_SNAP_FLAG_STORE_THUMB   (1 << 10) /** Store thumbnail to flash */
#define NANOCAM_SNAP_FLAG_STORE_TAGS    (1 << 11) /** Store image tag file */
#define NANOCAM_SNAP_FLAG_NOHIST        (1 << 16) /** Do not calculate histogram */
#define NANOCAM_SNAP_FLAG_NOEXPOSURE    (1 << 17) /** Do not adjust exposure */

typedef struct __attribute__((packed)) {
	/** Snap flags */
	uint32_t flags;
	/** Number of images to snap */
	uint8_t count;
	/** Autostore image format */
	uint8_t format;
	/** Inter-snap delay in milliseconds */
	uint16_t delay;
	/** Width of image, 0 for default */
	uint16_t width;
	/** Height of image, 0 for default */
	uint16_t height;
	/** Start column (currently unused) */
	uint16_t top;
	/** Start row (currently unused) */
	uint16_t left;
} cam_snap_t;

#define NANOCAM_SNAP_COLORS		4
#define NANOCAM_SNAP_HIST_BINS		16
typedef struct __attribute__((packed)) {
	/** Snap result, 0 for success, non-zero for error */
	uint8_t result;
	/** Reply sequence number (0 to count-1) */
	uint8_t seq;
	/** Pad to 32-bit boundary */
	uint8_t reserved[2];
	/** Average brightness */
	uint8_t light_avg[NANOCAM_SNAP_COLORS];
	/** Peak brightness */
	uint8_t light_peak[NANOCAM_SNAP_COLORS];
	/** Min brightness */
	uint8_t light_min[NANOCAM_SNAP_COLORS];
	/** Max brightness */
	uint8_t light_max[NANOCAM_SNAP_COLORS];
	/** Histogram */
	uint8_t hist[NANOCAM_SNAP_COLORS][NANOCAM_SNAP_HIST_BINS];
} cam_snap_reply_t;


/* NANOCAM_PORT_STORE */
#define NANOCAM_STORE_RAW       0 /* Store RAW sensor output (1 pixel into 2 bytes) */
#define NANOCAM_STORE_BMP       1 /* Store bitmap output */
#define NANOCAM_STORE_JPG       2 /* Store JPEG compressed output */
#define NANOCAM_STORE_DNG       3 /* Store DNG output (Raw, digital negative) */
#define NANOCAM_STORE_RAW10     4 /* Store packed RAW output (4 pixels into 5 bytes) */
#define NANOCAM_STORE_UNKNOWN   UINT8_MAX

#define NANOCAM_STORE_FLAG_FREEBUF	(1 << 0) /* Free buffer after store */
#define NANOCAM_STORE_FLAG_THUMB	(1 << 1) /* Create thumbnail */
#define NANOCAM_STORE_FLAG_TAG		(1 << 2) /* Create tag file */
typedef struct __attribute__((packed)) {
	/** Image format */
	uint8_t format;
	/** Scale down factor */
	uint8_t scale;
	/** Pad to 32-bit boundary */
	uint8_t reserved[2];
	/** Flags */
	uint32_t flags;
	/** Store filename. Can be absolute or relative */
	char filename[40];
} cam_store_t;

typedef struct __attribute__((packed)) {
	/** Store result, 0 for success, non-zero for error */
	uint8_t result;
	/** Pad to 32-bit boundary */
	uint8_t reserved[3];
	/** Pointer to image data */
	uint32_t image_ptr;
	/** Size of image data */
	uint32_t image_size;
} cam_store_reply_t;


/* NANOCAM_PORT_FOCUS */
#define NANOCAM_AF_JPEG			0
typedef struct __attribute__((packed)) {
	/** Autofocus algorithm */
	uint8_t algorithm;
	/** Pad to 32-bit boundary */
	uint8_t reserved[3];
} cam_focus_t;

typedef struct __attribute__((packed)) {
	/** AF result, 0 for success, non-zero for error */
	uint8_t result;
	/** Pad to 32-bit boundary */
	uint8_t reserved[3];
	/** Focus value. Higher value means more in focus */
	uint32_t focus;
} cam_focus_reply_t;


/* NANOCAM_PORT_IMG_LIST */
typedef struct __attribute__((packed)) {
	/** Elements per packet */
	uint8_t elements;
	/** Pad to 32-bit boundary */
	uint8_t reserved[3];
	/** Flags, must be zero */
	uint32_t flags;
} cam_list_t;

typedef struct __attribute__((packed)) {
	/** ID (sequence number) */
	uint8_t id;
	/** Image format */
	uint8_t format;
	/** Image flags */
	uint16_t flags;
	/** Image width */
	uint16_t width;
	/** Image height */
	uint16_t height;
	/** Pointer to image data */
	uint32_t ptr;
	/** Size of image data */
	uint32_t size;
	/** Timestamp in ns */
	uint64_t time;
} cam_list_element_t;

typedef struct __attribute__((packed)) {
	/** Total image list elements */
	uint16_t total;
	/** First element in this packet */
	uint16_t seq;
	/** Number of elements in this packet */
	uint8_t count;
	/** Pad to 32-bit boundary */
	uint8_t reserved[3];
	/** List of image elements */
	cam_list_element_t images[];
} cam_list_reply_t;


/* NANOCAM_PORT_IMG_FLUSH */
#define NANOCAM_IMG_FLUSH_MAGIC		0xCA11F155 /** Image flush magic */
typedef struct __attribute__ ((packed)) {
	/** Flags, must be zero */
	uint32_t flags;
	/** Magic value, must be NANOCAM_IMG_FLUSH_MAGIC */
	uint32_t magic;
} cam_list_flush_t;

typedef struct __attribute__ ((packed)) {
	/** Flush result, 0 for success, non-zero for error */
	uint8_t result;
	/** Pad to 32-bit boundary */
	uint8_t reserved[3];
} cam_list_flush_reply_t;


/* NANOCAM_PORT_REG_READ/NANOCAM_PORT_REG_WRITE */
typedef struct __attribute__((packed)) {
	/** Reg result, 0 for success, non-zero for error */
	uint8_t result;
	/** Register address */
	uint8_t reg_addr;
	/** Register value */
	uint16_t reg_value;
} cam_reg_t;


/* NANOCAM_PORT_RECOVERFS */
#define NANOCAM_RECOVERFS_MAGIC		0xD1EBADF5 /** File system recovery magic */
typedef struct __attribute__ ((packed)) {
	/** Flags, must be zero */
	uint32_t flags;
	/** Magic value, must be NANOCAM_RECOVERFS_MAGIC */
	uint32_t magic;
} cam_recoverfs_t;

typedef struct __attribute__ ((packed)) {
	/** Reg result, 0 for success, non-zero for error */
	uint8_t result;
	/** Pad to 32-bit boundary */
	uint8_t reserved[3];
} cam_recoverfs_reply_t;

#ifdef __cplusplus
}
#endif
#endif
