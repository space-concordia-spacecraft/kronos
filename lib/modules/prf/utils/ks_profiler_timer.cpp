#include "ks_profiler_timer.h"

namespace kronos {
    ProfilerTimer::ProfilerTimer(String scopeName, String path)
        : m_ScopeName(std::move(scopeName)), m_Path(std::move(path)) {
        m_Start = xTaskGetTickCount() * portTICK_RATE_MS;
    }

    ProfilerTimer::~ProfilerTimer() {
        auto endTime = xTaskGetTickCount() * portTICK_RATE_MS;
        Profiler::Get().Log(m_Name, m_Path, m_Start, endTime);
    }
}
