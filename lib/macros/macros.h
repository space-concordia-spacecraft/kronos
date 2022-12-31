#pragma once

#define KS_SINGLETON(name)                      \
private:                                        \
static Scope<name> s_instance;                  \
                                                \
public:                                         \
static inline void Init() {                     \
    if (s_instance == nullptr)                  \
        s_instance = CreateScope<name>();       \
}                                               \
                                                \
static inline name& GetInstance() {             \
    return *s_instance.get();                   \
}                                               \
                                                \
name(name& other) = delete;                     \
void operator=(const name& other) = delete

#define KS_SINGLETON_INSTANCE(name) Scope<name> name::s_instance = nullptr

#define KS_SINGLETON_EXPOSE_METHOD(instanceName, fn, ...)  \
static inline fn {                                         \
    return s_instance->instanceName(__VA_ARGS__);          \
}
