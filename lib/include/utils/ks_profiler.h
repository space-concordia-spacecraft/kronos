#pragma once

#include "ks_string.h"
#include "task.h"
#include "ks_framework.h"

namespace kronos {
    /// ProfilingSession object that stores the name of the session
    struct ProfilingSession {
        /// Name of the profiling session
        String name;
    };

    /// Profiler class to write benchmarks onto a file. It uses the Singleton design pattern.
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
         * EndSession deletes the current session.
         */
        void EndSession() {
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
        }

        /**
         * Log outputs the benchmarks results onto a file.
         * @param functionName - Name of the function.
         * @param location - File path location and line number for the function that was benchmarked.
         * @param start - Start time for the benchmark.
         * @param end - End time for the benchmark.
         */
        void Log(const String& functionName, const String& location, TickType_t start, TickType_t end) {
            TaskHandle_t currentTask = xTaskGetCurrentTaskHandle();
            TaskStatus_t currentTaskDetails;

            vTaskGetInfo(currentTask, &currentTaskDetails, pdFALSE, eRunning);

            Framework::LogDebug(functionName + " -> " + location);
        }

        /**
         * Getter function to get the instance of Profiler.
         * @return
         */
        static Profiler& Get() {
            static Profiler* instance  = new Profiler();
            return *instance;
        }

    };

    /// Class to create a timer for the benchmark.
    class ProfilerTimer {
    private:
        /// Start time for the benchmark.
        TickType_t m_Start;

        /// Name of the function being benchmarked.
        String m_Name;

        /// Path location for the function being benchmarked as well as its line number.
        String m_Location;
    public:
        /**
         * ProfilerTimer starts a new timer.
         * @param name - Name of the function being benchmarked.
         * @param location - Path location as well as line number for the function being benchmarked.
         */
        ProfilerTimer(const String & name, const String & location):m_Name(name), m_Location(location) {
            m_Start = xTaskGetTickCount() * portTICK_RATE_MS;
        }

        /**
         * ProfileTimer destructor stops the timer and logs the info.
         */
        ~ProfilerTimer() {
            TickType_t endTime = xTaskGetTickCount() * portTICK_RATE_MS;
            Profiler::Get().Log(m_Name, m_Location, m_Start, endTime);
        }
    };
}

// Macros to concatenate the file name and line number to a single string.
#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)
#define LOCATION TO_STRING(__FILE__) ":" TO_STRING(__LINE__)

// Macros to be used for general profiling
#define PROFILE_SCOPE_NAME(name) ProfilerTimer timer(name, LOCATION)
#define PROFILE_SCOPE() PROFILE_SCOPE_NAME(__PRETTY_FUNCTION__)

#define PROFILE_BEGIN(name) kronos::Profiler::Get().BeginSession(name);