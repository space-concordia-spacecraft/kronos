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
        EventMessage message;
        while (m_Queue.Pop(&message, 0) == pdPASS) {
            ProcessEvent(message);
        }
    }

    KsCmdResult ComponentQueued::ReceiveEvent(const EventMessage& message) {
        if (message.opcode == KS_OPCODE_EMPTY_QUEUE) {
            ProcessCommandQueue();
            return KS_CMDRESULT_NORETURN;
        }

        if (m_Queue.Push(message) != pdPASS) {
            // TODO: HANDLE ERROR OR WARNING
        }

        return KS_CMDRESULT_NORETURN;
    }

}
