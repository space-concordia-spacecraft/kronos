#include "ks_component_queued.h"

namespace kronos {

    ComponentQueued::ComponentQueued(const std::string& name)
            : ComponentPassive(name) {}

    KsResult ComponentQueued::Init() {
        return ComponentPassive::Init();
    }

    KsResult ComponentQueued::Destroy() {
        return ComponentPassive::Destroy();
    }

    KsResult ComponentQueued::ProcessEventQueue() {
        EventMessage message;
        while (m_Queue.Pop(&message, 0) == pdPASS) {
            ProcessEvent(message);
        }

        return KS_SUCCESS;
    }

    KsCmdResult ComponentQueued::ReceiveEvent(const EventMessage& message) {
        if (message.eventCode == KS_EVENT_CODE_EMPTY_QUEUE) {
            ProcessEventQueue();
            return KS_CMDRESULT_NORETURN;
        }

        if (m_Queue.Push(message) != pdPASS) {
            // TODO: HANDLE ERROR OR WARNING
        }

        return KS_CMDRESULT_NORETURN;
    }

}
