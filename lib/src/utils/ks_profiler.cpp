#include "ks_profiler.h"

namespace kronos {
    void Profiler::BeginSession(const String& name) {
        m_CurrentSession = new ProfilingSession({ name });
    }

    void Profiler::EndSession() {
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
    }

    void Profiler::Log(const String& functionName, const String& location, TickType_t start, TickType_t end) {
        TaskHandle_t currentTask = xTaskGetCurrentTaskHandle();
        TaskStatus_t currentTaskDetails;

        vTaskGetInfo(currentTask, &currentTaskDetails, pdFALSE, eRunning);

        Framework::LogDebug(functionName + " -> " + location);
    }

    Profiler& Profiler::Get() {
        static Profiler* instance = new Profiler();
        return *instance;
    }

    ProfilerTimer::ProfilerTimer(const String& name, const String& path) : m_Name(name), m_Path(path) {
        m_Start = xTaskGetTickCount() * portTICK_RATE_MS;
    }

    /**
     * ProfileTimer destructor stops the timer and logs the info.
     */
    ProfilerTimer::~ProfilerTimer() {
        TickType_t endTime = xTaskGetTickCount() * portTICK_RATE_MS;

        Profiler::Get().Log(m_Name, m_Path, m_Start, endTime);
    }
}