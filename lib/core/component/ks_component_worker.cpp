#include "ks_component_worker.h"
#include "ks_framework.h"

namespace kronos {
    ComponentWorker::ComponentWorker(
        const std::string& name,
        KsEventCodeType eventCode,
        size_t stackSize,
        uint16_t priority
    ) :
        ComponentActive(name, stackSize, priority),
        m_BusSend(Framework::CreateBus<BusAsync>("BA_W_" + name, eventCode)){}

    KsCmdResult ComponentWorker::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_scheduler_tick:
                EventMessage msg;
                msg.eventCode = m_BusSend->GetEventCode();
                m_BusSend->Publish(msg);
                break;
        }
        return ComponentActive::ProcessEvent(message);
    }

    KsResultType ComponentWorker::RegisterComponent(ComponentBase* componentBase) {
        m_BusSend->AddReceivingComponent(componentBase);
        return ks_success;
    }
}
