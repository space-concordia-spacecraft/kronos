#include "ks_health_monitor.h"

namespace kronos {

    ComponentHealthMonitor::ComponentHealthMonitor(const std::string& name, BusBase* healthIn, BusBase* healthOut)
            : ComponentQueued(name), m_HealthIn(healthIn), m_HealthOut(healthOut) {}

    KsCmdResult ComponentHealthMonitor::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_health_ping: {
                PingComponents();
                break;
            }
            case ks_event_health_pong: {
                auto* component = reinterpret_cast<ComponentActive*>(message.data);
                HandleComponentResponse(component);
                break;
            }
            default: {
                // TODO: Weird case
            }
        }
        return KS_CMDRESULT_NORETURN;
    }

    KsResult ComponentHealthMonitor::RegisterActiveComponent(ComponentActive* component) {
        if (m_ActiveComponentInfos.count(component)) {
            // TODO: HANDLE ERROR OR WARNING
            return ks_error_component_healthmonitor_component_exists;
        }

        ComponentInfo tempInfo;
        m_ActiveComponentInfos[component] = tempInfo;
        return ks_success;
    }

    KsResult ComponentHealthMonitor::PingComponents() {
        Logger::LogDebug("Health ping");
        EventMessage message;
        message.eventCode = ks_event_health_ping;
        message.returnBus = m_HealthIn;
        m_HealthOut->Publish(message);

        for (auto entry: m_ActiveComponentInfos) {
            uint32_t time = xTaskGetTickCount();
            if (time - entry.second.lastResponse >= KS_HEALTH_PING_RATE) {
                // TODO: Component has not responded
            }
        }

        return ks_success;
    }

    KsResult ComponentHealthMonitor::HandleComponentResponse(ComponentActive* component) {
        m_ActiveComponentInfos[component].lastResponse = xTaskGetTickCount();
        Logger::LogDebug("Health response from " + component->GetName());

        return ks_success;
    }

}
