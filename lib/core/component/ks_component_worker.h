#pragma once

#include "ks_bus.h"
#include "ks_component_active.h"

namespace kronos {

    class ComponentWorker : public ComponentActive {

    public:
        explicit ComponentWorker(
            const std::string& name,
            KsEventCodeType eventCode,
            size_t stackSize = KS_COMPONENT_STACK_SIZE_XLARGE,
            uint16_t priority = KS_COMPONENT_PRIORITY_MEDIUM);

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        KsResultType RegisterComponent(ComponentBase* component);

    protected:
        BusAsync* m_BusSend;
        EventMessage m_EventMessage;
    };

}
