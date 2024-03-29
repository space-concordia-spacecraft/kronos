#pragma once

#define KS_SINGLETON(name)                      \
private:                                        \
static Ref<name> s_Instance;                    \
                                                \
public:                                         \
static inline void CreateInstance() {           \
    if (s_Instance == nullptr)                  \
        s_Instance = CreateRef<name>();         \
}                                               \
                                                \
static inline name& GetInstance() {             \
    return *s_Instance;                         \
}                                               \
                                                \
static inline Ref<name> GetInstanceRef() {      \
    return s_Instance;                          \
}                                               \
                                                \
name(name& other) = delete;                     \
void operator=(const name& other) = delete

#define KS_SINGLETON_INSTANCE(name) Ref<name> name::s_Instance = nullptr

#define KS_SINGLETON_EXPOSE_METHOD(instanceName, fn, ...)  \
static inline fn {                                         \
    return s_Instance->instanceName(__VA_ARGS__);          \
}

// This macro is to find a key
#define KS_MAP_FIND(map, key, it) if (auto it = map.find(key); it != map.end())
#define KS_LIST_FIND(list, value, it) if (auto it = std::find(list.begin(), list.end(), value); it != list.end())

// This macro counts the number of variadic arguments passed to it
#define KS_NARG(...) \
         KS_NARG_(__VA_ARGS__,KS_RSEQ_N())
#define KS_NARG_(...) \
         KS_ARG_N(__VA_ARGS__)
#define KS_ARG_N(\
          _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
         _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
         _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
         _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
         _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
         _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
         _61, _62, _63, N, ...) N
#define KS_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0

// Macros to concatenate the file name and line number to a single string.
#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)
#define LOCATION TO_STRING(__FILE__) ":" TO_STRING(__LINE__)

#if defined(KS_DEBUG) || defined(KS_TRACE)
#define KS_DEBUGPRINT(...) printf(__VA_ARGS__)
#else
#define KS_DEBUGPRINT(...)
#endif

#include "ks_assert.h"