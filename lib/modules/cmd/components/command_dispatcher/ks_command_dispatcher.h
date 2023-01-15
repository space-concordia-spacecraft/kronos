#pragma once

#include "ks_component_active.h"
#include "ks_default_commands.h"

#include <unordered_map>
#include <functional>

namespace kronos {

    struct CommandMessage {
        KsCommandId commandId;
        void* parameter;
    };

    class CommandDispatcher : public ComponentActive {
    public:
        KsCmdResult ProcessEvent(const EventMessage& message) override;
        KsResultType RegisterCommand(KsCommandId opcode, const Function<void(void*)>&);

    private:
        Map<KsCommandId, Function<void(void*)>> m_CommandMessages;
    };
}