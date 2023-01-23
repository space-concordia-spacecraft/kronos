#include "ks_component_worker.h"
#include "ks_framework.h"

namespace kronos {
    ComponentWorker::ComponentWorker(
        const std::string& name,
        size_t stackSize,
        uint16_t priority
    ) : ComponentActive(name, stackSize, priority),
        m_BusSend(Framework::CreateBus("B_P_" + name)) {}

    void ComponentWorker::Run() {
        while (true) {
            const EventMessage* message;
            if (m_Queue->Pop(&message, 0) == pdPASS) {
                ComponentActive::ProcessEvent(*message);
                Framework::DeleteEventMessage(message);
            }

            m_BusSend->Publish(ks_event_empty_queue);

            taskYIELD();
        }
    }

    KsResultType ComponentWorker::RegisterComponent(ComponentBase* componentBase) {
        m_BusSend->AddReceivingComponent(componentBase);
        return ks_success;
    }
}
