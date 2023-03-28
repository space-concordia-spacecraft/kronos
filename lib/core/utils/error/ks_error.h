#pragma once

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "queue.h"

#include "ks_error_codes.h"
#include "ks_pch.h"

#define KS_THROW(error)                                             \
    ({                                                              \
        StackTrace::Trace(error, __FILE__, __LINE__);               \
        return error;                                               \
    })                                                              \

#define KS_TRY(error, expression)                                   \
    ({                                                              \
        if (expression != ks_success) [[unlikely]]                  \
            KS_THROW(error);                                        \
    })                                                              \

#define KS_PANIC_IF(expression, message)                            \
    ({                                                              \
        if (expression != ks_success) [[unlikely]]                  \
            KS_PANIC(message);                                      \
    })

#define KS_PANIC(message) ksPanicHook((message), __FILE__, __LINE__)

#define KS_ASSERT_IF(expression)                                    \
    ({                                                              \
        if (expression != ks_success) [[unlikely]]                  \
            KS_ASSERT(false)                                        \
    })

#define taskDISABLE_INTERRUPTS() portDISABLE_INTERRUPTS()

namespace kronos {
    void ksPanicHook(const char *message, const char *file, int line);

    struct ErrorInfo {
        KsResult code{ks_error};
        String file{};
        int line{0};
    };

    class StackTrace {
    public:
        explicit StackTrace(size_t length);

        static KsResult Pop(ErrorInfo* pElement);
        static KsResult Trace(KsResult code, String file, int line);
        static bool IsEmpty();

        static inline void CreateInstance(size_t length = 40) {
            if (s_Instance == nullptr)
                s_Instance = CreateScope<StackTrace>(length);
        }

    private:
        static inline StackTrace& GetInstance() {
            return *s_Instance;
        }

    private:
        static Scope<StackTrace> s_Instance;
        QueueHandle_t m_Queue;
    };
}
