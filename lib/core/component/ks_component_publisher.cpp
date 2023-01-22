#include "ks_component_publisher.h"
#include "ks_framework.h"

namespace kronos {
    ComponentPublisher::ComponentPublisher(
        const std::string& name,
        KsEventCodeType eventCode,
        size_t stackSize,
        uint16_t priority
    ) :
        ComponentActive(name, stackSize, priority),
        m_BusSend(Framework::CreateBus("B_P_" + name)),
        m_EventCode(eventCode){}

    void ComponentPublisher::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_scheduler_tick:
                m_BusSend->Publish(m_EventCode);
                break;
        }

        ComponentActive::ProcessEvent(message);
    }

    KsResultType ComponentPublisher::RegisterComponent(ComponentBase* componentBase) {
        m_BusSend->AddReceivingComponent(componentBase);
        return ks_success;
    }
}
