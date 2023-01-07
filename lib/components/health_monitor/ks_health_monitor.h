#pragma once

#include "ks_component_active.h"
#include "ks_bus.h"

#include <unordered_map>

namespace kronos {
    struct ComponentInfo {
        uint32_t lastResponse = 0;
    };

    class ComponentHealthMonitor : public ComponentQueued {
    public:
        ComponentHealthMonitor(const std::string& name, kronos::BusBase* healthIn, kronos::BusBase* healthOut);

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        KsResult RegisterActiveComponent(ComponentActive* component);

    private:
        std::unordered_map<ComponentActive*, ComponentInfo> m_ActiveComponentInfos;
        BusBase* m_HealthIn;
        BusBase* m_HealthOut;

        KsResult PingComponents();

        KsResult HandleComponentResponse(ComponentActive* component);
    };
}
