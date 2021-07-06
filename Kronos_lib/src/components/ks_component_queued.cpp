#include "ks_component_queued.h"

namespace kronos {

    ComponentQueued::ComponentQueued(const String& name)
        : ComponentPassive(name) {}

    void ComponentQueued::Init() {
        ComponentPassive::Init();
    }

    void ComponentQueued::Destroy() {
        ComponentPassive::Destroy();
    }

    void ComponentQueued::ProcessCommandQueue() {
        CommandMessage message;
        while (m_Queue.Pop(&message) == pdPASS) {
            ProcessCommand(message);
        }
    }

    void ComponentQueued::ReceiveCommand(const CommandMessage& message) {
        if (message.opcode == KS_OPCODE_EMPTY_QUEUE) {
            ProcessCommandQueue();
            return;
        }

        if (m_Queue.Push(message) != pdPASS) {
            // TODO: HANDLE ERROR OR WARNING
        }
    }

}
