#ifndef GS_FTP_INTERNAL_TYPES_H
#define GS_FTP_INTERNAL_TYPES_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Internal FTP definitions, used between server and client.
*/

#if (GS_FTP_INTERNAL_USE)

#include <gs/util/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/** FTP port used by GATOSS */
#define GS_FTP_CSP_PORT_GATOSS    7

/** Number of chunks in status message */
#define GS_FTP_STATUS_CHUNKS     10

/** Maximum chunk size */
#define GS_FTP_MAX_CHUNK_SIZE   256

/**
   FTP request type.
*/
typedef enum __attribute__ ((__packed__)) {
    /** New upload request */
    FTP_UPLOAD_REQUEST   = 0,
    /** New upload reply */
    FTP_UPLOAD_REPLY     = 1,
    /** New download request */
    FTP_DOWNLOAD_REQUEST = 2,
    /** New download reply */
    FTP_DOWNLOAD_REPLY   = 3,
    /** Status request */
    FTP_STATUS_REQUEST   = 5,
    /** Status reply */
    FTP_STATUS_REPLY     = 6,
    /** List request */
    FTP_LIST_REQUEST     = 7,
    /** List reply */
    FTP_LIST_REPLY       = 8,
    /** List entry */
    FTP_LIST_ENTRY       = 9,
    /** Move request */            
    FTP_MOVE_REQUEST     = 10,
    /** Move reply */
    FTP_MOVE_REPLY       = 11,
    /** Remove request */
    FTP_REMOVE_REQUEST   = 12,
    /** Remove reply */
    FTP_REMOVE_REPLY     = 13,
    /** CRC request */            
    FTP_CRC_REQUEST      = 14,
    /** CRC reply */            
    FTP_CRC_REPLY        = 15,
    /** Data chunk */
    FTP_DATA             = 16,
    /** Transfer done */
    FTP_DONE             = 17,
    /** Abort transfer */
    FTP_ABORT            = 18,
    FTP_MKFS_REQUEST     = 19,
    FTP_MKFS_REPLY       = 20,
    FTP_ZIP_REQUEST      = 21,
    FTP_ZIP_REPLY        = 22,
    /** Copy request */
    FTP_COPY_REQUEST     = 23,
    /** Copy reply */
    FTP_COPY_REPLY       = 24,
    FTP_MKDIR_REQUEST    = 25,
    FTP_MKDIR_REPLY      = 26,
    FTP_RMDIR_REQUEST    = 27,
    FTP_RMDIR_REPLY      = 28,
} gs_ftp_type_t;

/** Upload file request */
typedef struct {
    uint32_t size;
    uint32_t crc32;
    uint16_t chunk_size;
    gs_ftp_backend_type_t backend;
    uint32_t mem_addr;
    char path[GS_FTP_PATH_LENGTH];
} __attribute__ ((__packed__)) ftp_upload_request_t;

/** Upload file reply */
typedef struct {
    uint8_t ret;
} __attribute__ ((__packed__)) ftp_upload_reply_t;

/** Download file request */
typedef struct {
    uint16_t chunk_size;
    gs_ftp_backend_type_t backend;
    uint32_t mem_addr;
    uint32_t mem_size;
    char path[GS_FTP_PATH_LENGTH];
} __attribute__ ((__packed__)) ftp_download_request_t;

/** Download file reply */
typedef struct {
    uint8_t ret;
    uint32_t size;
    uint32_t crc32;
} __attribute__ ((__packed__)) ftp_download_reply_t;

/** Move file request */
typedef struct {
    gs_ftp_backend_type_t backend;
    char from[GS_FTP_PATH_LENGTH];
    char to[GS_FTP_PATH_LENGTH];
} __attribute__ ((__packed__)) ftp_move_request_t;

/** Move file reply */
typedef struct {
    uint8_t ret;
} __attribute__ ((__packed__)) ftp_move_reply_t;

/** Remove file request */
typedef struct {
    gs_ftp_backend_type_t backend;
    char path[GS_FTP_PATH_LENGTH];
} __attribute__ ((__packed__)) ftp_remove_request_t;

/** Remove file reply */
typedef struct {
    uint8_t ret;
} __attribute__ ((__packed__)) ftp_remove_reply_t;

/** mkfs file request */
typedef struct {
    gs_ftp_backend_type_t backend;
    uint8_t force;
    char path[GS_FTP_PATH_LENGTH];
} __attribute__ ((__packed__)) ftp_mkfs_request_t;

/** mkfs file reply */
typedef struct {
    uint8_t ret;
} __attribute__ ((__packed__)) ftp_mkfs_reply_t;

/** mkdir request */
typedef struct {
    gs_ftp_backend_type_t backend;
    char path[GS_FTP_PATH_LENGTH];
    uint32_t mode;
} __attribute__ ((__packed__)) ftp_mkdir_request_t;

/** mkdir reply */
typedef struct {
    uint8_t ret;
} __attribute__ ((__packed__)) ftp_mkdir_reply_t;

/** rmdir request */
typedef struct {
    gs_ftp_backend_type_t backend;
    char path[GS_FTP_PATH_LENGTH];
} __attribute__ ((__packed__)) ftp_rmdir_request_t;

/** rmdir reply */
typedef struct {
    uint8_t ret;
} __attribute__ ((__packed__)) ftp_rmdir_reply_t;

/** Data chunk */
typedef struct {
    uint32_t chunk;
    uint8_t bytes[GS_FTP_MAX_CHUNK_SIZE];
} __attribute__ ((__packed__)) ftp_data_t;

typedef struct {
    uint32_t next;
    uint32_t count;
} __attribute__ ((__packed__)) ftp_status_element_t;

/** Status packet */
typedef struct {
    uint8_t ret;
    uint32_t complete;
    uint32_t total;
    uint16_t entries;
    ftp_status_element_t entry[GS_FTP_STATUS_CHUNKS];
} __attribute__ ((__packed__)) ftp_status_reply_t;

/** List files request */
typedef struct {
    gs_ftp_backend_type_t backend;
    char path[GS_FTP_PATH_LENGTH];
} __attribute__ ((__packed__)) ftp_list_request_t;

/** List files reply */
typedef struct {
    uint8_t ret;
    uint16_t entries;
} __attribute__ ((__packed__)) ftp_list_reply_t;

/** CRC reply */
typedef struct {
    uint8_t ret;
    uint32_t crc;
} __attribute__ ((__packed__)) ftp_crc_reply_t;

/** Zip file action request */
typedef struct {
    gs_ftp_backend_type_t backend;
    gs_ftp_zip_action_t action;
    char src[GS_FTP_PATH_LENGTH];
    char dest[GS_FTP_PATH_LENGTH];
} __attribute__ ((__packed__)) ftp_zip_request_t;

/** Zip file action reply */
typedef struct {
    uint8_t ret;
    uint8_t action;
    uint32_t decomp_sz;
    uint32_t comp_sz;
} __attribute__ ((__packed__)) ftp_zip_reply_t;

/** Copy file request */
typedef struct {
    gs_ftp_backend_type_t backend;
    char from[GS_FTP_PATH_LENGTH];
    char to[GS_FTP_PATH_LENGTH];
} __attribute__ ((__packed__)) ftp_copy_request_t;

/** Copy file reply */
typedef struct {
    uint8_t ret;
} __attribute__ ((__packed__)) ftp_copy_reply_t;

/** FTP packet */
typedef struct {
    uint8_t type;
    union {
        /* Download */
        ftp_download_request_t down;
        ftp_download_reply_t downrep;

        /* Upload */
        ftp_upload_request_t up;
        ftp_upload_reply_t uprep;

        /* Data */
        ftp_data_t data;

        /* Status reply */
        ftp_status_reply_t statusrep;

        /* Listing */
        ftp_list_request_t list;
        ftp_list_reply_t listrep;
        gs_ftp_list_entry_t listent;

        /* Remove */
        ftp_remove_request_t remove;
        ftp_remove_reply_t removerep;

        /* Move */
        ftp_move_request_t move;
        ftp_move_reply_t moverep;

        /* mkfs */
        ftp_mkfs_request_t mkfs;
        ftp_mkfs_reply_t mkfsrep;

        /* mkdir */
        ftp_mkdir_request_t mkdir;
        ftp_mkdir_reply_t mkdirrep;

        /* rmdir */
        ftp_rmdir_request_t rmdir;
        ftp_rmdir_reply_t rmdirrep;

        /* CRC */
        ftp_crc_reply_t crcrep;

        /* Zip */
        ftp_zip_request_t zip;
        ftp_zip_reply_t ziprep;

        /* Copy */
        ftp_copy_request_t copy;
        ftp_copy_reply_t copyrep;
    };
} __attribute__ ((__packed__)) ftp_packet_t;

#ifdef __cplusplus
}
#endif
#endif
#endif
