#ifndef GS_FTP_TYPES_H
#define GS_FTP_TYPES_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Common FTP definitions.
*/

#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Maximum path length - including zero termination.
*/
#define GS_FTP_PATH_LENGTH   50

/**
   Backend types.

   @deprecated Avoid creating new backend types, instead use the URL/path to determine the correct backend.
*/
typedef enum __attribute__ ((__packed__)) {
    /**
       Direct memory access.
    */
    GS_FTP_BACKEND_RAM   = 0,
    /**
       Legacy reference to file system used on SD cards.
    */
    GS_FTP_BACKEND_FAT   = 1,
    /**
       Legacy reference to the file system used on e.g. A3200 Nanomind FLASH chip.
    */
    GS_FTP_BACKEND_UFFS  = 2,
    /**
       Standard/default backend.
    */
    GS_FTP_BACKEND_FILE  = 3,
} gs_ftp_backend_type_t;

/**
   FTP error codes.
*/
typedef enum __attribute__ ((__packed__)) {
    /** No error */
    GS_FTP_RET_OK      = 0,
    /** Protocol error */
    GS_FTP_RET_NOENT   = 2,
    /** Invalid argument */
    GS_FTP_RET_INVAL   = 3,
    /**< No space left on device */
    GS_FTP_RET_NOSPC   = 4,
    /** IO error */
    GS_FTP_RET_IO      = 5,
    /** File to large */
    GS_FTP_RET_FBIG    = 6,
    /** File exists */
    GS_FTP_RET_EXISTS  = 7,
    /** Operation not supported */
    GS_FTP_RET_NOTSUP  = 8,
    /** Not enough memory */
    GS_FTP_RET_NOMEM   = 9,
    /** Device or resource busy */
    GS_FTP_RET_BUSY    = 10,
} gs_ftp_return_t;
    
/**
   List entry type.
*/
typedef enum {
    /** File. */
    GS_FTP_LIST_FILE 	= 0,
    /** Directory */
    GS_FTP_LIST_DIR	= 1,
} __attribute__ ((__packed__)) gs_ftp_list_entry_type_t;

/**
   List files entry
*/
typedef struct {
    /** Entry index (zero based). */
    uint16_t entry;
    /** Path. */
    char path[GS_FTP_PATH_LENGTH];
    /** Entry type. */
    gs_ftp_list_entry_type_t type;
    /** Size (in bytes). */
    uint32_t size;
} __attribute__ ((__packed__)) gs_ftp_list_entry_t;

/**
   FTP zip action codes
*/
typedef enum {
    /** Zip file. */
    GS_FTP_ZIP    = 1,
    /** Un zip file. */
    GS_FTP_UNZIP  = 2,
} __attribute__ ((__packed__)) gs_ftp_zip_action_t;


#ifdef __cplusplus
}
#endif
#endif
