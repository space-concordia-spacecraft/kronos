#pragma once

typedef unsigned char byte;

//typedef char int8_t;
typedef unsigned char uint8_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef int int32_t;
typedef unsigned int uint32_t;

typedef long long int int64_t;
typedef unsigned long long int uint64_t;

//typedef uint32_t size_t;

#define NULL 0

// GCC Compiler Directive - Pack the struct/class so the compiler doesn't add useless padding bytes.
#define PACKED __attribute__((packed))
