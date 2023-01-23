#pragma once

#include "ks_component_queued.h"
#include "ks_packet_parser.h"
#include "ks_io.h"
#include "ks_bus.h"

namespace kronos {
    class CommandTransmitter : public ComponentQueued {
    public:
        explicit CommandTransmitter(const std::string& name, IoDriver* ioDriver);
        void ProcessEvent(const EventMessage& message) override;

    private:
        Bus* m_Bus;
        IoDriver* m_IoDriver;

        KsResultType Transmit(const Packet& packet);
    };
}
