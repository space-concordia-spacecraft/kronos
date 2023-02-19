/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#define GS_UTIL_DEPRECATED_ERROR_CODES 1

#include <gs/util/error.h>
#include <gs/util/string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __AVR__
const char * gs_error_string(int error)
{
    /**
       avr: const strings are not automatically stored in program space (see gs/util/pgm.h), and if stored
       in program space, they require special formatting in logs (i.e. "%S").
       So we settle for simple error string, with the error nnumber - no need to change log/(s)printf etc.
       @note: solution is not 100% thread/task safe.
    */
    static char buffer[15]; // large enough to always keep zero termination, due to no thread/task lock
    snprintf(buffer, sizeof(buffer), "%d", error);
    return buffer;
}
#else
const char * gs_error_string(int error)
{
    switch (error) {
        case GS_OK: return "GS_OK(0)";
        case GS_ERROR_PERM: return GS_DEF2STRING(GS_ERROR_PERM) "(-1)";
        case GS_ERROR_INTR: return GS_DEF2STRING(GS_ERROR_INTR) "(-4)";
        case GS_ERROR_IO: return GS_DEF2STRING(GS_ERROR_IO) "(-5)";
        case GS_ERROR_AGAIN: return GS_DEF2STRING(GS_ERROR_AGAIN) "(-11)";
        case GS_ERROR_ALLOC: return GS_DEF2STRING(GS_ERROR_ALLOC) "(-12)";
        case GS_ERROR_ACCESS: return GS_DEF2STRING(GS_ERROR_ACCESS) "(-13)";
        case GS_ERROR_BUSY: return GS_DEF2STRING(GS_ERROR_BUSY) "(-16)";
        case GS_ERROR_EXIST: return GS_DEF2STRING(GS_ERROR_EXIST) "(-17)";
        case GS_ERROR_ARG: return GS_DEF2STRING(GS_ERROR_ARG) "(-22)";
        case GS_ERROR_NOT_IMPLEMENTED: return GS_DEF2STRING(GS_ERROR_NOT_IMPLEMENTED) "(-38)";
        case GS_ERROR_OVERFLOW: return GS_DEF2STRING(GS_ERROR_OVERFLOW) "(-75)";
        case GS_ERROR_NOT_SUPPORTED: return GS_DEF2STRING(GS_ERROR_NOT_SUPPORTED) "(-95)";
        case GS_ERROR_IN_USE: return GS_DEF2STRING(GS_ERROR_IN_USE) "(-98)";
        case GS_ERROR_CONNECTION_RESET: return GS_DEF2STRING(GS_ERROR_CONNECTION_RESET) "(-104)";
        case GS_ERROR_NO_BUFFERS: return GS_DEF2STRING(GS_ERROR_NO_BUFFERS) "(-105)";
        case GS_ERROR_TIMEOUT: return GS_DEF2STRING(GS_ERROR_TIMEOUT) "(-110)";
        case GS_ERROR_ALREADY_IN_PROGRESS: return GS_DEF2STRING(GS_ERROR_ALREADY_IN_PROGRESS) "(-114)";

        case GS_ERROR_HANDLE: return GS_DEF2STRING(GS_ERROR_HANDLE) "(-2000)";
        case GS_ERROR_NOT_FOUND: return GS_DEF2STRING(GS_ERROR_NOT_FOUND) "(-2001)";
        case GS_ERROR_FULL: return GS_DEF2STRING(GS_ERROR_FULL) "(-2002)";
        case GS_ERROR_RANGE: return GS_DEF2STRING(GS_ERROR_RANGE) "(-2003)";
        case GS_ERROR_DATA: return GS_DEF2STRING(GS_ERROR_DATA) "(-2004)";
        case GS_ERROR_UNKNOWN: return GS_DEF2STRING(GS_ERROR_UNKNOWN) "(-2005)";
        case GS_ERROR_NO_DATA: return GS_DEF2STRING(GS_ERROR_NO_DATA) "(-2006)";
        case GS_ERROR_STALE: return GS_DEF2STRING(GS_ERROR_STALE) "(-2007)";
        case GS_ERROR_TYPE: return GS_DEF2STRING(GS_ERROR_TYPE) "(-2008)";
        case GS_ERROR_AMBIGUOUS: return GS_DEF2STRING(GS_ERROR_AMBIGUOUS) "(-2009)";
        case GS_ERROR_STATE: return GS_DEF2STRING(GS_ERROR_STATE) "(-2010)";
    }

    // as fallback we use standard POSIX error string
    const int posix_error = abs(error);
    return strerror(posix_error);
}
#endif

gs_error_t gs_error(int error)
{
    return (abs(error) * -1);
}

#ifndef __AVR__
const char * error_string(int code)
{
    switch (code) {
        case E_NO_ERR:
            return "No error";
        case E_NO_DEVICE:
            return "No device";
        case E_MALLOC_FAIL:
            return "Malloc fail";
        case E_THREAD_FAIL:
            return "Thread failure";
        case E_NO_QUEUE:
            return "No such queue";
        case E_INVALID_BUF_SIZE:
            return "Invalid buffer size";
        case E_INVALID_PARAM:
            return "Invalid paramater";
        case E_NO_SS:
            return "No such subsystem";
        case E_GARBLED_BUFFER:
            return "Rubbish in buffer";
        case E_FLASH_ERROR:
            return "FLASH error";
        case E_BOOT_SER:
            return "Thread boot fail: serial driver";
        case E_BOOT_DEBUG:
            return "Thread boot fail: debug console";
        case E_BOOT_FLASH:
            return "Thread boot fail: flash driver";
        case E_NO_BUFFER:
            return "No buffer";
        default:
            return "Unknown error";
    }
}
#endif
