#include "ks_component_worker.h"
#include "ks_framework.h"

namespace kronos {
    ComponentWorker::ComponentWorker(
        const std::string& name,
        size_t stackSize,
        uint16_t priority
    ) : ComponentActive(name, 0, stackSize, priority) {}

    void ComponentWorker::Run() {
        while (true) {
            { // SCOPE FOR PROFILER
                const EventMessage* message;
                if (m_Queue->Pop(&message, 0) != ks_success) {
                    ComponentActive::ProcessEvent(*message);
                    Framework::DeleteEventMessage(message);

                    // if(res.HasError()) StackTrace::Flush(), use the framework housekeeping
                }

                for (const auto& component: m_QueuedComponents)
                    component->ProcessEventQueue();

                // if(res.HasError()) StackTrace::Flush(), use the framework housekeeping
            }

            taskYIELD();
        }
    }

    KsResult ComponentWorker::RegisterComponent(ComponentQueued* component) {
        m_QueuedComponents.push_back(component);
        return ks_success;
    }
}
