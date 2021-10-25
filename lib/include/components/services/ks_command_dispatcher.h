#pragma once

#include "ks_component_active.h"

#include "ks_hashmap.h"

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
        HashMap<KsOpcode , void (*)(void*)> m_CommandMessages;
    };
}