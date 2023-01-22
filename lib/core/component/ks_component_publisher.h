#pragma once

#include "ks_bus.h"
#include "ks_component_active.h"

namespace kronos {

    class ComponentPublisher : public ComponentActive {

    public:
        explicit ComponentPublisher(
            const std::string& name,
            KsEventCodeType eventCode,
            size_t stackSize = KS_COMPONENT_STACK_SIZE_XLARGE,
            uint16_t priority = KS_COMPONENT_PRIORITY_MEDIUM);

        void ProcessEvent(const EventMessage& message) override;

        KsResultType RegisterComponent(ComponentBase* component);

    protected:
        Bus* m_BusSend;
        KsEventCodeType m_EventCode;
        EventMessage m_EventMessage;
    };

}
