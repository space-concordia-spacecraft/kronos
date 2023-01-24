#pragma once

#include "ks_bus.h"
#include "ks_component_active.h"

namespace kronos {

    class ComponentWorker : public ComponentActive {

    public:
        explicit ComponentWorker(
            const std::string& name,
            size_t stackSize,
            uint16_t priority
        );

        [[noreturn]] void Run() override;
        KsResultType RegisterComponent(ComponentQueued* component);

    protected:
        List<ComponentQueued*> m_QueuedComponents{};
        EventMessage m_EventMessage;

    };

}
