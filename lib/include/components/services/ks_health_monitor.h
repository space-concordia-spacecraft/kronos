#pragma once

#include "ks_hashmap.h"

#include "ks_component_active.h"
#include "ks_bus.h"

#include "ks_profiler.h"

#define KS_HEALTH_PING_RATE 3000

namespace kronos {

    struct ComponentInfo {
        uint32_t lastResponse = 0;
    };

    class ComponentHealthMonitor : public ComponentQueued {
    public:
        ComponentHealthMonitor(const kronos::String& name, kronos::BusBase* healthIn, kronos::BusBase* healthOut);

        KsCmdResult ProcessEvent(const EventMessage& message) override;
        void RegisterActiveComponent(ComponentActive* component);

    private:
        HashMap<ComponentActive*, ComponentInfo> m_ActiveComponentInfos;
        BusBase* m_HealthIn;
        BusBase* m_HealthOut;

        void PingComponents();
        void HandleComponentResponse(ComponentActive* component);
    };

}