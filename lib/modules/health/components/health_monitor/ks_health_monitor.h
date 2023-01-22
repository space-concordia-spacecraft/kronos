#pragma once

#include "ks_component_publisher.h"
#include "ks_bus.h"
#include "ks_logger.h"
#include "ks_error_component_codes.h"

#define KS_HEALTH_PONG_MAX_RESPONSE_TIME 3000

namespace kronos {

    struct ComponentHealthInfo {
        uint32_t lastResponse = 0;
    };

    class HealthMonitor : public ComponentPublisher {
    KS_SINGLETON(HealthMonitor);

    public:
        HealthMonitor();

        void ProcessEvent(const EventMessage& message) override;
        KsResultType PostInit() override;

    private:
        KsResultType PingComponents();
        KsResultType HandleComponentResponse(ComponentActive* component);

    private:
        Map<ComponentActive*, ComponentHealthInfo> m_ActiveComponentInfos{};
        Bus* m_BusPong;
    };
}
