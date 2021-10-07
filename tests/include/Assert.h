#pragma once

#define KT_DEBUGBREAK()

#define KT_INTERNAL_ASSERT_IMPL(type, check, msg, ...) if(!(check)) { printf(msg, __VA_ARGS__); KT_DEBUGBREAK(); return false; }
#define KT_INTERNAL_ASSERT_WITH_MSG(type, check, ...) KT_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: %s", __VA_ARGS__)
#define KT_INTERNAL_ASSERT_NO_MSG(type, check) KT_INTERNAL_ASSERT_IMPL(type, check, "Assertion '%s' failed at %s:%d", #check, __FILE__, __LINE__)

#define KT_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define KT_INTERNAL_ASSERT_GET_MACRO(...) KT_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, KT_INTERNAL_ASSERT_WITH_MSG, KT_INTERNAL_ASSERT_NO_MSG)

#define KT_ASSERT(...) KT_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__)