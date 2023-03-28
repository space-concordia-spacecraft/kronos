#include "ks_profiler_timer.h"
#include "ks_profiler.h"

namespace kronos {
    ProfilerTimer::ProfilerTimer(String scopeName, String path)
        : m_ScopeName(std::move(scopeName)), m_Path(std::move(path)) {
        m_Start = xTaskGetTickCount() * portTICK_RATE_MS;
    }

    ProfilerTimer::~ProfilerTimer() {
        auto endTime = xTaskGetTickCount() * portTICK_RATE_MS;
        Profiler::Update(m_ScopeName, m_Path, m_Start, endTime);
    }
}
