#include "ks_command_dispatcher.h"

namespace kronos {
    KsCmdResult ComponentCommandDispatcher::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_command_dispatch:
                auto* commandMessage = reinterpret_cast<CommandMessage*>(message.data);
                m_CommandMessages[commandMessage->commandId](commandMessage->parameter);
                delete commandMessage;
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

    KsResult
    ComponentCommandDispatcher::RegisterCommand(KsCommandId opcode, const std::function<void(void*)>& pFunction) {
        m_CommandMessages[opcode] = pFunction;
        return ks_success;
    }
}