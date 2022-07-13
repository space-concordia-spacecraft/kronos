// ==================================================================================
// \title ks_profiler.h
// \brief Profiler tool to measure time spent in a function.
// ==================================================================================

#pragma once

#define INITIALIZATION_INTERVAL 10000
#define RUNNING_INTERVAL 30000

// Kronos includes
#include "kronos.h"

namespace kronos {
    //! \struct ProfilingSession
    //! \brief A struct used to store the name of a profiling session
    //!
    //! This struct is used to hold information about the profiling session.
    struct ProfilingSession {
        ProfilingSession(const String& name, TickType_t interval):name(name),profileLogInterval(interval) {
            startLog = xTaskGetTickCount() * portTICK_RATE_MS;
        }
        //! Name of the profiling session
        String name;

        //! Time interval at which the function profiles get logged
        TickType_t profileLogInterval;

        //! Start logging interval
        TickType_t startLog;

        //! HashMap that maps a function being profiled to the longest time it took to run in a given interval
        HashMap<String, TickType_t> longestProfiles;
    };

    //! \class Profiler
    //! \brief A class that handles the profiling information of a function.
    //!
    //! This class is used to either store or print to the terminal the profiling information for a given function. It uses the Singleton design pattern.
    class Profiler {
    private:
        //! Pointer holding the instance of the current profiling session
        ProfilingSession* m_CurrentSession;
    public:

        //! \brief Begins a profiling session.
        //!
        //! \param name the name of the profiling session.
        void BeginSession(const String& name, TickType_t interval);

        //! \brief Ends the profiling session.
        void EndSession();

        //! \brief Logs the resulting information from the timer into the terminal.
        //!
        //! \param functionName The name of the function being profiled
        //! \param location The path location of the function being profiled
        //! \param start The start time for the profiling
        //! \param end The end time for the profiling
        void Log(const String& functionName, const String& location, TickType_t start, TickType_t end);

        //! Getter for the Singleton design pattern
        //! \return Instance of the profiler
        static Profiler& Get();

    };

    //! \class ProfilerTimer
    //! \brief A class that implements a simple timer
    //!
    //! This class implements a simple timer used for the profiling of scopes
    class ProfilerTimer {
    private:
        //! Start time for the timer
        TickType_t m_Start;

        //! Name of the scope being benchmarked
        String m_Name;

        //! Path location for the scope being benchmarked as well as its line number
        String m_Path;
    public:
        //! \brief Starts the timer
        //!
        //! \param name the name of the scope being benchmarked
        //! \param path the path location for the scope being benchmarked
        ProfilerTimer(const String & name, const String & path);

        //! \brief Ends the timer
        ~ProfilerTimer();
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