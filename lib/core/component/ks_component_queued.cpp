#include "ks_component_queued.h"
#include "ks_logger.h"
#include "ks_framework.h"

namespace kronos {

    ComponentQueued::ComponentQueued(const String& name)
        : ComponentPassive(name), m_Queue(Queue<const EventMessage*>::Create()) {}

    KsResultType ComponentQueued::Init() {
        return ComponentPassive::Init();
    }

    KsResultType ComponentQueued::Destroy() {
        return ComponentPassive::Destroy();
    }

    KsResultType ComponentQueued::ProcessEventQueue() {
        const EventMessage* message;
        while (m_Queue->Pop(&message, 0) == pdPASS) {
            ProcessEvent(*message);
            Framework::DeleteEventMessage(message);
        }
        return ks_success;
    }

    void ComponentQueued::ReceiveEvent(const EventMessage* message) {
        if (message->eventCode == ks_event_empty_queue) {
            ProcessEventQueue();
            return;
        }

        auto ret = m_Queue->Push(message);
        KS_ASSERT(ret == pdPASS, "Unable to push to queue.");
    }
}
