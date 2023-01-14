#include "ks_profiler.h"

namespace kronos {

    void Profiler::BeginSession(const std::string& name, TickType_t interval) {
        m_CurrentSession = new ProfilingSession(name, interval);
    }

    void Profiler::EndSession() {
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
    }

    void Profiler::Log(const std::string& functionName, const std::string& location, TickType_t start, TickType_t end) {
        TaskHandle_t currentTask = xTaskGetCurrentTaskHandle();
        TickType_t currentTime = xTaskGetTickCount() * portTICK_RATE_MS;

        // TBD, should we use FreeRTOS timers instead?
        if (m_CurrentSession->longestProfiles[functionName] < start - end)
            m_CurrentSession->longestProfiles[functionName] = start - end;

        if (m_CurrentSession->profileLogInterval <= currentTime - m_CurrentSession->startLog) {
            m_CurrentSession->startLog = xTaskGetTickCount() * portTICK_RATE_MS;

            TaskStatus_t currentTaskDetails;
            vTaskGetInfo(currentTask, &currentTaskDetails, pdFALSE, eRunning);

            Logger::Debug(functionName + " in " + location);
            // TODO: Store in a file. (currentTaskDetails, longestProfile)
        }

    }

    Profiler& Profiler::Get() {
        static Profiler* instance = new Profiler();
        return *instance;
    }

    ProfilerTimer::ProfilerTimer(const std::string& name, const std::string& path) : m_Name(name), m_Path(path) {
        m_Start = xTaskGetTickCount() * portTICK_RATE_MS;
    }

    ProfilerTimer::~ProfilerTimer() {
        TickType_t endTime = xTaskGetTickCount() * portTICK_RATE_MS;

        Profiler::Get().Log(m_Name, m_Path, m_Start, endTime);
    }

}