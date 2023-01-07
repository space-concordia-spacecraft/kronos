#pragma once

#include <memory>

#define KS_SINGLETON(name)                      \
private:                                        \
static std::unique_ptr<name> s_instance;        \
                                                \
public:                                         \
static inline void Init() {                     \
    if (s_instance == nullptr)                  \
        s_instance = std::make_unique<name>();  \
}                                               \
                                                \
static inline name& GetInstance() {             \
    return *s_instance.get();                   \
}                                               \
                                                \
name(name& other) = delete;                     \
void operator=(const name& other) = delete

#define KS_SINGLETON_INSTANCE(name) std::unique_ptr<name> name::s_instance = nullptr

#define KS_SINGLETON_EXPOSE_METHOD(instanceName, fn, ...)  \
static inline fn {                                         \
    return s_instance->instanceName(__VA_ARGS__);          \
}
