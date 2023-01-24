#pragma once

#include "ks_component_active.h"
#include "ks_default_commands.h"
#include "ks_bus.h"
#include "ks_packet.h"

namespace kronos {

    struct CommandMessage {
        KsCommandId commandId;
        void* parameter;
    };

    class CommandDispatcher : public ComponentActive {
    public:
        explicit CommandDispatcher(const String& name);
        void ProcessEvent(const EventMessage& message) override;

    private:
        void ProcessCommand(const Packet& packet);
        Bus* m_Bus;
    };
}