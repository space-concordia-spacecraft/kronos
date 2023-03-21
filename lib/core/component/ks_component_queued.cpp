#include "ks_component_queued.h"
#include "ks_logger.h"
#include "ks_framework.h"

namespace kronos {

    ComponentQueued::ComponentQueued(const String& name, KsTickType queueTicksToWait)
        : ComponentPassive(name), m_Queue(Queue<const EventMessage*>::Create()), m_QueueTicksToWait(queueTicksToWait) {}

    ErrorOr<void> ComponentQueued::Init() {
        return ComponentPassive::Init();
    }

    ErrorOr<void> ComponentQueued::Destroy() {
        return ComponentPassive::Destroy();
    }

    ErrorOr<void> ComponentQueued::ProcessEventQueue() {
        const EventMessage* message;
        while (m_Queue->Pop(&message, m_QueueTicksToWait) == pdPASS) {
            ProcessEvent(*message);
            Framework::DeleteEventMessage(message);
        }

        return {};
    }

    ErrorOr<void> ComponentQueued::ReceiveEvent(const EventMessage* message) {
        if (m_Queue->Push(message) != pdPASS) {
            // KS_ASSERT(false, "Unable to push to queue.");
            Framework::DeleteEventMessage(message);
        }

        return {};
    }
}
