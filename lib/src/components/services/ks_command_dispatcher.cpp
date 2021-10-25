#include "ks_command_dispatcher.h"

namespace kronos {
    KsCmdResult ComponentCommandDispatcher::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_DISPATCH_COMMAND:
                auto* commandMessage = reinterpret_cast<CommandMessage*>(message.data);
                m_CommandMessages.Get(commandMessage->opcode)(commandMessage->parameter);
                delete commandMessage;
                break;
        }
        return ComponentActive::ProcessEvent(message);
    }

    KsResult ComponentCommandDispatcher::RegisterCommand(KsOpcode opcode, void (* pFunction)(void*)) {
        m_CommandMessages.Put(opcode, pFunction);

        return KS_SUCCESS;
    }
}