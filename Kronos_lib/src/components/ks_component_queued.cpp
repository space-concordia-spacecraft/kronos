#include "ks_component_queue.h"

namespace kronos {

    void ComponentQueued::ProcessCommandQueue() {
        CommandMessage message;
        while (m_Queue.Pop(&message) == pdPASS) {
            ProcessCommand(message);
        }
    }

    void ComponentQueued::ReceiveCommand(const CommandMessage& commandMessage) {
        if (m_Queue.Push(commandMessage) != pdPASS) {

#ifndef NDEBUG
            // TODO: Handle Error
            printf("asdasd");
#endif
        } else {

        }
    }
}
