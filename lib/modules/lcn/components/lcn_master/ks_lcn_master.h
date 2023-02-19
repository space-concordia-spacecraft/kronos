#pragma once

#include "ks_io.h"
#include "ks_component_queued.h"

namespace kronos {
    class LcnMaster : public ComponentQueued {
    public:
        explicit LcnMaster(const std::string& name, IoDriver* ioDriver);
        void ProcessEvent(const EventMessage& message) override;

    private:
        IoDriver* m_IoDriver;

        void Ping();
    };
}
