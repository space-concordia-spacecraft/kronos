#ifndef GS_UTIL_TYPES_H
#define GS_UTIL_TYPES_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Base type definitions and functions.

   In some rare cases, it is impossible to make code that works on all platforms. In these cases the following defines may be used to
   exclude/include code:

   |  define         | Platform                                                            |
   | :----:          | :----                                                               |
   | \_\_AVR\_\_     | 8 bit, e.g. atmega1281, atmega2560, attiny25, attiny44, attiny84    |
   | \_\_linux\_\_   | 32/64 bit, Linux based                                              |
   | #GS_PLATFORM_64 | Defined to 1 on 64 bit platforms, otherwise 0.                      |


*/

#include <stdint.h>    // intXX_t
#include <stdbool.h>   // bool
#include <stddef.h>    // size_t

#ifdef __cplusplus
extern "C" {
#endif

/**
   Control static declaration at compile time.
   Allows unit tests to access internal functions or variables.
   @note Static declared variables are initialized to zero by the compiler - BUT if you use GS_NO_STATIC instead of static, they will not be initialized.
*/
#if GS_NO_STATIC
#define GS_STATIC
#else
#define GS_STATIC static
#endif

/**
   Convert integer to pointer.
*/
#define GS_TYPES_INT2PTR(value)   ((void*)(intptr_t)(value))

/**
   Convert integer to pointer.
*/
#define GS_TYPES_UINT2PTR(value)  ((void*)(uintptr_t)(value))

/**
   Convert pointer to integer.
*/
#define GS_TYPES_PTR2INT(value)   ((intptr_t)(void*)(value))

/**
   Convert pointer to integer.
*/
#define GS_TYPES_PTR2UINT(value)  ((uintptr_t)(void*)(value))

/**
   Assert on 'value'.

   Example:
      GS_STATIC_ASSERT(sizeof(int) >= 2, int_must_be_at_least_16bit);
   fails if size of (int) is less than 2 bytes.
*/
#define GS_STATIC_ASSERT(condition, name)   typedef char name[(condition) ? 1 : -1]

/**
   Context switch state.
   Used by FreeRTOS when waking a higher priority task/thread from within an ISR.
   The actual struct is defined in libembed.
*/
typedef struct gs_context_switch gs_context_switch_t;

/**
   Return element count of array.
*/
#define GS_ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))

/**
   Address union.
*/
typedef union {
    /**
       Normal address pointer.
    */
    void* p;
    /**
       Address pointer as an unsigned value.
    */
    uintptr_t u;
} gs_address_t;

/**
   @cond HIDDEN_SYMBOLS 
   Compile check size of primitives (just to be sure, that they are what we expect).
*/
GS_STATIC_ASSERT(sizeof(gs_address_t) == sizeof(void*), unexpected_address_void_pointer_size);
GS_STATIC_ASSERT(sizeof(gs_address_t) == sizeof(uintptr_t), unexpected_address_uintptr_size);
GS_STATIC_ASSERT(sizeof(bool) == sizeof(uint8_t), unexpected_bool_size);
GS_STATIC_ASSERT(sizeof(float) == sizeof(uint32_t), unexpected_float_size);
#if (__AVR__)
// avr/avr8 is 8 bit
GS_STATIC_ASSERT(sizeof(int) == sizeof(int16_t), unexpected_int_size_on_avr8);
#else
// rest should be 32 or 64 bit
GS_STATIC_ASSERT(sizeof(int) == sizeof(int32_t), unexpected_int_size);
GS_STATIC_ASSERT(sizeof(double) == sizeof(uint64_t), unexpected_double_size);
#endif
/** @endcond */

/**
   Defined to 1 on 64 bit platforms, otherwise 0.
*/
#if (__LP64__)
#define GS_PLATFORM_64 1
#else
#define GS_PLATFORM_64 0
#endif

/**
   Max PATH length.
   The length is based on most code being cross-platform, and some targets have limited stack-size.
*/
#if !defined(GS_PATH_MAX)
#define GS_PATH_MAX 255
#endif
    
#ifdef __cplusplus
}
#endif
#endif
