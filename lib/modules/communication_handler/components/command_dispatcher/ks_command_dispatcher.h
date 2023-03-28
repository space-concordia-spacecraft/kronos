#pragma once

#include "ks_component_active.h"
#include "ks_bus.h"
#include "ks_packet.h"

namespace kronos {
    class CommandDispatcher : public ComponentActive {
    public:
        explicit CommandDispatcher(const String& name);
        KsResult Init() override;
        KsResult ProcessEvent(const EventMessage& message) override;

    private:
        KsResult ProcessCommand(const Packet& packet);
    };
}