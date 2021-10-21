#include "ks_string.h"
#include "task.h"

#define PROFILE_SCOPE_NAME(name) ProfilerTimer timer("name -> __FILE__:__LINE__")
#define PROFILE_SCOPE() PROFILE_SCOPE_NAME(__FUNCTION__)

namespace kronos {
    /// ProfilingSession object that stores the name of the session
    struct ProfilingSession {
        ///
        String name;
    };

    /// Profiler class to write benchmarks onto a file
    class Profiler {
    private:
        /// Pointer holding the current profiling session
        ProfilingSession* m_CurrentSession;
    public:

        /**
         * BeginSession begins a profiling session.
         * @param name - Name of the profiling session.
         */
        void BeginSession(const String& name) {
            m_CurrentSession = new ProfilingSession({name});
        }

        /**
         * EndSession deletes the
         */
        void EndSession() {
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
        }

        /**
         *
         * @param name
         * @param start
         * @param end
         */
        void Log(String& name, TickType_t start, TickType_t end) {
            TaskHandle_t currentTask = xTaskGetCurrentTaskHandle();
            TaskStatus_t currentTaskDetails;

            vTaskGetInfo(currentTask, &currentTaskDetails, pdFALSE, eRunning);

            printf("%s %lu %lu %s %d", name.Ptr(), start, end, currentTaskDetails.pcTaskName, currentTaskDetails.xTaskNumber);
        }

        /**
         *
         * @return
         */
        static Profiler& Get() {
            static Profiler* instance  = new Profiler();
            return *instance;
        }

    };

    ///
    class ProfilerTimer {
    private:
        ///
        TickType_t m_Start;

        ///
        String m_Name;
    public:
        /**
         *
         * @param name
         */
        ProfilerTimer(const String & name):m_Name(name) {
            m_Start = xTaskGetTickCount() * portTICK_RATE_MS;
        }

        /**
         *
         */
        ~ProfilerTimer() {
            TickType_t endTime = xTaskGetTickCount() * portTICK_RATE_MS;
            Profiler::Get().Log(m_Name, m_Start, endTime);
        }
    };
}