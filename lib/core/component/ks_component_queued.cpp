#include "ks_component_queued.h"

namespace kronos {

    ComponentQueued::ComponentQueued(const String& name)
        : ComponentPassive(name), m_Queue(Queue<EventMessage>::Create()) {}

    KsResultType ComponentQueued::Init() {
        return ComponentPassive::Init();
    }

    KsResultType ComponentQueued::Destroy() {
        return ComponentPassive::Destroy();
    }

    KsResultType ComponentQueued::ProcessEventQueue() {
        EventMessage message;
        while (m_Queue->Pop(&message, 0) == pdPASS) {
            ProcessEvent(message);
        }

        return ks_success;
    }

    KsCmdResult ComponentQueued::ReceiveEvent(const EventMessage& message) {
        if (message.eventCode == ks_event_empty_queue) {
            ProcessEventQueue();
            return KS_CMDRESULT_NORETURN;
        }

        if (m_Queue->Push(message) != pdPASS) {
            // TODO: HANDLE ERROR OR WARNING
        }

        return KS_CMDRESULT_NORETURN;
    }

}
