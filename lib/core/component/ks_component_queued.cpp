#include "ks_component_queued.h"
#include "ks_framework.h"

namespace kronos {

    ComponentQueued::ComponentQueued(const String& name, KsTickType queueTicksToWait)
        : ComponentPassive(name), m_QueueTicksToWait(queueTicksToWait) {}

    KsResult ComponentQueued::Init() {
        m_Queue = Queue<const EventMessage*>::Create();
        return ComponentPassive::Init();
    }

    KsResult ComponentQueued::Destroy() {
        return ComponentPassive::Destroy();
    }

    KsResult ComponentQueued::ProcessEventQueue() {
        const EventMessage* message;
        while (m_Queue->Pop(&message, m_QueueTicksToWait) == ks_success) {
            KS_TRY(ks_error_component_process_event, ProcessEvent(*message));
            KS_TRY(ks_error_component_process_event, Framework::DeleteEventMessage(message));
        }

        return ks_success;
    }

    KsResult ComponentQueued::ReceiveEvent(const EventMessage* message) {
        if (m_Queue->Push(message) != ks_success)
            KS_TRY(ks_error_component_receive_event, Framework::DeleteEventMessage(message));

        return ks_success;
    }
}
