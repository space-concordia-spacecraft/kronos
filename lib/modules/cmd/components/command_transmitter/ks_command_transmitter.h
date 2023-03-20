#pragma once

#include "ks_component_queued.h"
#include "ks_command_ids.h"
#include "ks_packet_parser.h"
#include "drivers/protocols/ks_io.h"
#include "ks_bus.h"

namespace kronos {
    class CommandTransmitter : public ComponentQueued {
    public:
        explicit CommandTransmitter(const std::string& name, IoDriver* ioDriver);
        void ProcessEvent(const EventMessage& message) override;

        static void TransmitPayload(KsCommand cmd, const uint8_t* payload, size_t payloadSize, bool setEOF = true);

    private:
        Bus* m_Bus;
        IoDriver* m_IoDriver;

        KsResultType Transmit(const Packet& packet);
    };
}
