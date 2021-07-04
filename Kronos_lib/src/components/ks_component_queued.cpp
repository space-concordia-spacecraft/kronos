#include "ks_component_queued.h"

namespace kronos {

    void ComponentQueued::ProcessCommandQueue() {
        CommandMessage message;
        while (m_Queue.Pop(&message) == pdPASS) {
            ProcessCommand(message);
        }
    }

    void ComponentQueued::ReceiveCommand(const CommandMessage& commandMessage) {
        if (commandMessage.opcode == KS_OPCODE_EMPTY_QUEUE) {
            ProcessCommandQueue();
            return;
        }

        if (m_Queue.Push(commandMessage) != pdPASS) {
#ifndef NDEBUG
            // TODO: Handle Error
#endif
        }

    }
}
