#pragma once

namespace kronos {
    //! \class ProfilerTimer
    //! \brief A class that implements a simple timer
    //!
    //! This class implements a simple timer used for the profiling of scopes
    class ProfilerTimer {
    private:
        //! Start time for the timer
        TickType_t m_Start;

        //! Name of the scope being benchmarked
        String m_ScopeName;

        //! Path location for the scope being benchmarked as well as its line number
        String m_Path;

    public:
        //! \brief Starts the timer
        //!
        //! \param scopeName the name of the scope being benchmarked
        //! \param path the path location for the scope being benchmarked
        ProfilerTimer(String scopeName, String path);

        //! \brief Ends the timer
        ~ProfilerTimer();
    };
}

// Macros to be used for general profiling
#define PROFILE_SCOPE_NAME(name) ProfilerTimer timer(name, LOCATION)
#define PROFILE_SCOPE() PROFILE_SCOPE_NAME(__PRETTY_FUNCTION__)
