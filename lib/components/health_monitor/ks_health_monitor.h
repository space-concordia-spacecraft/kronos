#pragma once

#include "ks_component_active.h"
#include "ks_bus.h"
#include "ks_logger.h"
#include "ks_error_component_codes.h"

#include <unordered_map>

#define KS_HEALTH_PING_RATE 3000

namespace kronos {
    struct ComponentInfo {
        uint32_t lastResponse = 0;
    };

    class ComponentHealthMonitor : public ComponentQueued {
    public:
        ComponentHealthMonitor(
            const std::string& name
        );

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        KsResultType RegisterActiveComponent(ComponentActive* component);

    private:
        std::unordered_map<ComponentActive*, ComponentInfo> m_ActiveComponentInfos;
        BusAsync m_BusPing;
        BusAsync m_BusPong;

        KsResultType PingComponents();

        KsResultType HandleComponentResponse(ComponentActive* component);
    };
}
