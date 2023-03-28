#include "ks_error.h"

#include <utility>

namespace kronos {
    Scope<StackTrace>StackTrace::s_Instance;

    StackTrace::StackTrace(size_t length) {
        m_Queue = xQueueCreate(length, sizeof(ErrorInfo));
        if(m_Queue == 0) KS_PANIC("Failed to create the stack trace");
    }

    KsResult StackTrace::Pop(ErrorInfo* pElement) {
        if(xQueueReceive(s_Instance->m_Queue, pElement, 0) != pdPASS) return ks_error;

        return ks_success;
    }

    KsResult StackTrace::Trace(KsResult code, String file, int line) {
        ErrorInfo error = {
            .code = code,
            .file = std::move(file),
            .line = line
        };

        if(xQueueSend(s_Instance->m_Queue, &error, 0) != pdPASS) return ks_error;

        return ks_success;
    }

    bool StackTrace::IsEmpty() {
        return uxQueueSpacesAvailable(s_Instance->m_Queue) == 0;
    }

    void ksPanicHook(const char *message, const char *file, int line) {
        while(!kronos::StackTrace::IsEmpty()) {
            kronos::ErrorInfo info;
            if(kronos::StackTrace::Pop(&info) != ks_success)
                break;
            KS_DEBUGPRINT("[ERROR:%ld] in %s at line %d", info.code, info.file.c_str(), info.line);
        }

        KS_DEBUGPRINT("[PANIC] %s in %s at line %d", message, file, line);
        configASSERT((volatile void*) nullptr);
    }
}
