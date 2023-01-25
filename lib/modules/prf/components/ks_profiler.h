#pragma once

#include "ks_component_queued.h"
#include "ks_file.h"

#define KS_PROFILER_FILE_PATH       "/profile"

namespace kronos {
    struct ProfileInfo {
        String path;
        size_t interval;
        size_t stackSizeRemaining;
    };

    class Profiler : public ComponentQueued {
    KS_SINGLETON(Profiler);
    public:
        Profiler();
        ~Profiler() override = default;
        void ProcessEvent(const EventMessage& message) override;

    public:
        KS_SINGLETON_EXPOSE_METHOD(_Update,
            void Update(const String& scopeName, const String& path, TickType_t start, TickType_t end),
            scopeName, path, start, end);

    private:
        void _Update(const String& scopeName, const String& path, TickType_t start, TickType_t end);

        void SaveProfiles();
    private:
        Map <String, ProfileInfo> m_ProfiledFunctions{};
        File m_File;
        size_t m_TickCount = 0;
    };
}