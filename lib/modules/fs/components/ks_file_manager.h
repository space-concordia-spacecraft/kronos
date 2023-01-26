#pragma once

#include "ks_component_queued.h"

#define KS_DOWNLINK_FILE_RATE 10

namespace kronos {
    class FileManager : public ComponentQueued {
    KS_SINGLETON(FileManager);

    public:
        FileManager();
        ~FileManager() override = default;

        void ProcessEvent(const EventMessage& message) override;

    private:
        void Downlink(const String& fileName);
    };
}