#include "ks_profiler.h"
#include "ks_logger.h"

namespace kronos {

    void Profiler::BeginSession(const String& name, TickType_t interval) {
        m_CurrentSession = new ProfilingSession(name, interval);
    }

    void Profiler::EndSession() {
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
    }

    void Profiler::Log(const String& functionName, const String& location, TickType_t start, TickType_t end) {
        TickType_t currentTime = xTaskGetTickCount() * portTICK_RATE_MS;

        // TBD, should we use FreeRTOS timers instead?
        if (m_CurrentSession->longestProfiles[functionName] < start - end)
            m_CurrentSession->longestProfiles[functionName] = start - end;

        if (m_CurrentSession->profileLogInterval <= currentTime - m_CurrentSession->startLog) {
            m_CurrentSession->startLog = xTaskGetTickCount() * portTICK_RATE_MS;

            TaskStatus_t currentTaskDetails;
            vTaskGetInfo(nullptr, &currentTaskDetails, pdTRUE, eRunning);

            Logger::Debug("%s in %s", functionName.c_str(), location.c_str());
        }

    }

    Profiler& Profiler::Get() {
        static auto* instance = new Profiler();
        return *instance;
    }

    ProfilerTimer::ProfilerTimer(String name, String path)
        : m_Name(std::move(name)), m_Path(std::move(path)) {
        m_Start = xTaskGetTickCount() * portTICK_RATE_MS;
    }

    ProfilerTimer::~ProfilerTimer() {
        auto endTime = xTaskGetTickCount() * portTICK_RATE_MS;
        Profiler::Get().Log(m_Name, m_Path, m_Start, endTime);
    }

}