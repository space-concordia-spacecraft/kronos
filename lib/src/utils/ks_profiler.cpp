#include "ks_profiler.h"

namespace kronos {
    void Profiler::BeginSession(const String& name, TickType_t interval) {
        m_CurrentSession = new ProfilingSession(name, interval);
    }

    void Profiler::EndSession() {
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
    }

    void Profiler::Log(const String& functionName, const String& location, TickType_t start, TickType_t end) {
        TaskHandle_t currentTask = xTaskGetCurrentTaskHandle();
        TickType_t currentTime = xTaskGetTickCount() * portTICK_RATE_MS;

        // TBD, should we use FreeRTOS timers instead?
        if(m_CurrentSession->longestProfiles.Get(functionName) < start - end)
            m_CurrentSession->longestProfiles.Put(functionName, start - end);

        if(m_CurrentSession->profileLogInterval <= currentTime - m_CurrentSession->startLog) {
            m_CurrentSession->startLog = xTaskGetTickCount() * portTICK_RATE_MS;

            TaskStatus_t currentTaskDetails;
            vTaskGetInfo(currentTask, &currentTaskDetails, pdFALSE, eRunning);

            Framework::LogDebug(functionName + " in " + location);
            // TODO: Store in a file. (currentTaskDetails, longestProfile)
        }

    }

    Profiler& Profiler::Get() {
        static Profiler* instance = new Profiler();
        return *instance;
    }

    ProfilerTimer::ProfilerTimer(const String& name, const String& path) : m_Name(name), m_Path(path) {
        m_Start = xTaskGetTickCount() * portTICK_RATE_MS;
    }

    ProfilerTimer::~ProfilerTimer() {
        TickType_t endTime = xTaskGetTickCount() * portTICK_RATE_MS;

        Profiler::Get().Log(m_Name, m_Path, m_Start, endTime);
    }
}