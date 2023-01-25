#pragma once

#include "ks_component_queued.h"
#include "ks_file.h"

namespace kronos {
    struct ProfiledFunction {
        String name;
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
        Map <String, ProfiledFunction> m_ProfiledFunctions{};
    };
}