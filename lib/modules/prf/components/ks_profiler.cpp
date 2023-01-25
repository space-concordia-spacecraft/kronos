#include "ks_profiler.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(Profiler);

    Profiler::Profiler() : ComponentQueued("CQ_PROFILER") {}

    void Profiler::ProcessEvent(const EventMessage& message) {

    }

    void Profiler::_Update(const String& scopeName, const String& path, TickType_t start, TickType_t end) {
        auto intervalMs = (start - end) * portTICK_RATE_MS;

        TaskStatus_t currentTaskDetails;
        vTaskGetInfo(nullptr, &currentTaskDetails, pdTRUE, eRunning);

        if (m_ProfiledFunctions.contains(scopeName) && m_ProfiledFunctions[scopeName].interval > intervalMs)
            return;

        m_ProfiledFunctions[scopeName] = {
            .name = scopeName,
            .path = path,
            .interval = intervalMs,
            .stackSizeRemaining = currentTaskDetails.usStackHighWaterMark
        };
    }

    void Profiler::SaveProfiles() {
        // TODO: save the results to a file. Include the time spent in each task.
    }
}
