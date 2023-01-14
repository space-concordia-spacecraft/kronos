#pragma once

#include "ks_component_active.h"
#include "ks_bus.h"
#include "ks_logger.h"
#include "ks_error_component_codes.h"

#define KS_HEALTH_PING_RATE 3000

namespace kronos {

    struct ComponentHealthInfo {
        uint32_t lastResponse = 0;
    };

    class HealthMonitor : public ComponentQueued {
    KS_SINGLETON(HealthMonitor);

    public:
        HealthMonitor();

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        KsResultType RegisterActiveComponent(const Ref <ComponentActive>& component);

    private:
        KsResultType PingComponents();
        KsResultType HandleComponentResponse(ComponentActive* component);

    private:
        Map<ComponentActive*, ComponentHealthInfo> m_ActiveComponentInfos;
        BusAsync m_BusPing;
        BusAsync m_BusPong;

    };
}
