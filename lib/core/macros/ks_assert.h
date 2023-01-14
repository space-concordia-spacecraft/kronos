#pragma once

#ifdef KS_DEBUG
#define KS_DEBUGBREAK() __asm("BKPT #0")
#else
#define KS_DEBUGBREAK()
#endif

#define KS_INTERNAL_ASSERT_IMPL(type, check, msg, ...) if (!(check)) { Logger::Error(msg, __VA_ARGS__); KS_DEBUGBREAK(); }
#define KS_INTERNAL_ASSERT_WITH_MSG(type, check, ...) KS_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: %s\n", __VA_ARGS__)
#define KS_INTERNAL_ASSERT_NO_MSG(type, check) KS_INTERNAL_ASSERT_IMPL(type, check, "Assertion '%s' failed at %s:%d\n", #check, __FILE__, __LINE__)

#define KS_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define KS_INTERNAL_ASSERT_GET_MACRO(...) KS_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, KS_INTERNAL_ASSERT_WITH_MSG, KS_INTERNAL_ASSERT_NO_MSG)

#define KS_ASSERT(...) KS_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__)
