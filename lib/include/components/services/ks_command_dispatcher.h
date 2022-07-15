#pragma once

#include "ks_component_active.h"

#include <unordered_map>

namespace kronos {

    struct CommandMessage {
        KsOpcode opcode;
        void* parameter;
    };

    class ComponentCommandDispatcher : public ComponentActive {
    public:
        KsCmdResult ProcessEvent(const EventMessage& message) override;

        KsResult RegisterCommand(KsOpcode opcode, void (* pFunction)(void*));

    private:
        std::unordered_map<KsOpcode, void (*)(void*)> m_CommandMessages;
    };
}