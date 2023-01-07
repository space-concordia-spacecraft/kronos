#include "ks_health_monitor.h"

namespace kronos {

    ComponentHealthMonitor::ComponentHealthMonitor(const std::string& name, BusBase* healthIn, BusBase* healthOut)
            : ComponentQueued(name), m_HealthIn(healthIn), m_HealthOut(healthOut) {}

    KsCmdResult ComponentHealthMonitor::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_RATE_GROUP_TICK:
                PingComponents();
                break;
            case KS_EVENT_CODE_HEALTH_PONG:
                auto* component = reinterpret_cast<ComponentActive*>(message.data);
                HandleComponentResponse(component);
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

    KsResult ComponentHealthMonitor::RegisterActiveComponent(ComponentActive* component) {
        if (m_ActiveComponentInfos.count(component)) {
            // TODO: HANDLE ERROR OR WARNING
            return KS_ERROR;
        }

        ComponentInfo tempInfo;
        m_ActiveComponentInfos[component] = tempInfo;
        return KS_SUCCESS;
    }

    KsResult ComponentHealthMonitor::PingComponents() {
        Framework::LogDebug("Health ping");
        PROFILE_SCOPE();
        EventMessage message;
        message.eventCode = KS_EVENT_CODE_HEALTH_PING;
        message.returnBus = m_HealthIn;
        m_HealthOut->Publish(message);

        for (auto entry: m_ActiveComponentInfos) {
            uint32_t time = xTaskGetTickCount();
            if (time - entry.second.lastResponse >= KS_HEALTH_PING_RATE) {
                // TODO: Component has not responded
            }
        }

        return KS_SUCCESS;
    }

    KsResult ComponentHealthMonitor::HandleComponentResponse(ComponentActive* component) {
        m_ActiveComponentInfos[component].lastResponse = xTaskGetTickCount();
        Framework::LogDebug("Health response from " + component->GetName());

        return KS_SUCCESS;
    }

}
