#include "ks_component_worker.h"
#include "ks_framework.h"

namespace kronos {
    ComponentWorker::ComponentWorker(
        const std::string& name,
        size_t stackSize,
        uint16_t priority
    ) : ComponentActive(name, stackSize, priority){}

    void ComponentWorker::Run() {
        while (true) {
            const EventMessage* message;
            if (m_Queue->Pop(&message, 0) == pdPASS) {
                ComponentActive::ProcessEvent(*message);
                Framework::DeleteEventMessage(message);
            }

            for(const auto& component: m_QueuedComponents)
                component->ProcessEventQueue();

            taskYIELD();
        }
    }

    KsResultType ComponentWorker::RegisterComponent(ComponentQueued* component) {
        m_QueuedComponents.push_back(component);
        return ks_success;
    }
}
