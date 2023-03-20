#pragma once

#include "drivers/protocols/ks_io.h"
#include "ks_component_queued.h"

namespace kronos {
    class LcnSlave : public ComponentQueued {
    public:
        explicit LcnSlave(const std::string& name, IoDriver* ioDriver);
        void ProcessEvent(const EventMessage& message) override;

    private:
        IoDriver* m_IoDriver;

        void CheckNetwork();
    };
}
