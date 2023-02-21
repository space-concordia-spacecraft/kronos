#ifndef GS_UTIL_PROGMEM_H
#define GS_UTIL_PROGMEM_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Macros for handling special memory access.

   On most targets/processors, constant data/strings are located in the program space and can be read in the same way as data in the data space.
   However, on a few targets (e.g. avr/avr8), data/strings must be marked in a special way in order to go into the program space, see #GS_PGM_STR()

   Using following macros, will make it easier to make cross-platform code and avoid \#if/\#endif.
   These macros should only be used where the code also needs to run on avr/avr8.

   @note Including this header on avr/avr8 will REDEFINE printf!.

   http://www.atmel.com/webdoc/avrlibcreferencemanual/group__avr__pgmspace.html.
   http://www.nongnu.org/avr-libc/user-manual/pgmspace.html.
*/

#include <gs/util/stdio.h>
#if defined(__AVR__)
#include <avr/pgmspace.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__AVR__) || (__DOXYGEN__)
/**
   Special program/data memory handling.
*/
#define GS_PGM                        1

/**
   Place object in program space (must be const).
   Example: static const uint8_t data8[] GS_PGM_OBJECT = {1, 255};
*/
#define GS_PGM_OBJECT                 PROGMEM

/**
   Place const string in program space.
   By default the string goes into data, uses thereby uses up space.
   Once the string is placed in program space, xx_P functions must be used to access them - see #GS_PGM_PRINTF.
   @note printf is re-defined by including this header
*/
#define GS_PGM_STR(str)               PSTR(str)

/**
   Read uint8 from program space (near).
*/
#define GS_PGM_UINT8(value)           pgm_read_byte(&(value))

/**
   Read uint8 from program space using a pointer (near).
*/
#define GS_PGM_UINT8_BY_PTR(value)    pgm_read_byte(value)

/**
   Read word from program space (near).
*/
#define GS_PGM_UINT16(value)          pgm_read_word(&(value))
/**
   Read word from program space using a pointer (near).
*/
#define GS_PGM_UINT16_BY_PTR(value)   pgm_read_word(value)

/**
   Read dword from program space (near).
*/
#define GS_PGM_UINT32(value)          pgm_read_dword(&(value))
/**
   Read word from program space using a pointer (near).
*/
#define GS_PGM_UINT32_BY_PTR(value)   pgm_read_dword(value)
    
/**
   Memcpy from program space (near).
   @param[in] dst destination.
   @param[in] src source - program space.
   @param[in] n number of bytes to copy
*/
#define GS_PGM_MEMCPY(dst, src, n)    memcpy_P(dst, src, n)

/**
   String compare (program space)
   @param[in] s1 string 1
   @param[in] s2 string 2 - program space.
   @param[in] n max number of bytes to compare
*/
#define GS_PGM_STRNCMP(s1,s2,n)       strncmp_P(s1, s2, n)

/**
   String compare (program space)
   @param[in] s1 string 1
   @param[in] s2 string 2 - program space.
   @param[in] n max number of bytes to compare
*/
#define GS_PGM_STRNCASECMP(s1,s2,n)       strncasecmp_P(s1, s2, n)

/**
   String formatting character for referencing a string placed in programs space.
*/
#define GS_PGM_FMT_STR                "S"

/**
   printf (format string in program space).
   Example: print \a param->name (from prgram space) and \a value from data space, using a format string in program space.
      GS_PGM_PRINTF(GS_PGM_STR("%"GS_PGM_FMT_STR", %d"), param->name, value)
*/
#define GS_PGM_PRINTF(format, ...)    printf_P(format, ##__VA_ARGS__)

/**
   vprintf (format string in program space).
*/
#define GS_PGM_VPRINTF(format, va)    vfprintf_P(stdout, format, va)

/**
   vfprintf (format string in program space).
*/
#define GS_PGM_VFPRINTF(stream, format, va) vfprintf_P(stream, format, va)

/**
   snprintf (format string in program space).
*/
#define GS_PGM_SNPRINTF(buf, bufsize, format, ...) snprintf_P(buf, bufsize, format, ##__VA_ARGS__)

/**
   vsnprintf (format string in program space).
*/
#define GS_PGM_VSNPRINTF(buf, bufsize, format, va) vsnprintf_P(buf, bufsize, format, va)

/**
   redefines printf (puts format string in program space)
 */
#undef printf
#define printf(format, ...) GS_PGM_PRINTF(GS_PGM_STR(format), ## __VA_ARGS__)

#else

#undef GS_PGM

#define GS_PGM_OBJECT
#define GS_PGM_STR(str)                     (str)
#define GS_PGM_UINT8(value)                 (value)
#define GS_PGM_UINT8_BY_PTR(value)          (*(value))
#define GS_PGM_UINT16(value)                (value)
#define GS_PGM_UINT16_BY_PTR(value)         (*(value))
#define GS_PGM_UINT32(value)                (value)
#define GS_PGM_UINT32_BY_PTR(value)         (*(value))
#define GS_PGM_MEMCPY(dst, src, size)       memcpy(dst, src, size)
#define GS_PGM_STRNCMP(s1,pgmstr,size)      strncmp(s1, pgmstr, size)
#define GS_PGM_STRNCASECMP(s1,pgmstr,size)  strncasecmp(s1, pgmstr, size)
    
#define GS_PGM_FMT_STR                      "s"
#define GS_PGM_PRINTF(format, ...)          printf(format, ## __VA_ARGS__)
#define GS_PGM_VPRINTF(format, va)          vprintf(format, va)
#define GS_PGM_VFPRINTF(stream, format, va) vfprintf(stream, format, va)
#define GS_PGM_SNPRINTF(buf, bufsize, format, ...) snprintf(buf, bufsize, format, ##__VA_ARGS__)
#define GS_PGM_VSNPRINTF(buf, bufsize, format, va) vsnprintf(buf, bufsize, format, va)

#endif

#ifdef __cplusplus
}
#endif
#endif
