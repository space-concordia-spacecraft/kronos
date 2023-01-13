#include "ks_health_monitor.h"

namespace kronos {

    ComponentHealthMonitor::ComponentHealthMonitor(
        const std::string& name
    ) : ComponentQueued(name),
        m_BusPing("B_HEALTH_PING", ks_event_health_ping),
        m_BusPong("B_HEALTH_PONG", ks_event_health_pong) {}

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

    KsResultType ComponentHealthMonitor::RegisterActiveComponent(ComponentActive* component) {
        if (m_ActiveComponentInfos.count(component)) {
            // TODO: HANDLE ERROR OR WARNING
            return ks_error_component_healthmonitor_already_registered;
        }

        ComponentInfo tempInfo;
        m_ActiveComponentInfos[component] = tempInfo;
        return ks_success;
    }

    KsResultType ComponentHealthMonitor::PingComponents() {
        Logger::Debug("Health ping");
        EventMessage message;
        message.eventCode = ks_event_health_ping;
        message.returnBus = &m_BusPong;
        m_BusPing.Publish(message);

        for (auto entry: m_ActiveComponentInfos) {
            uint32_t time = xTaskGetTickCount();
            if (time - entry.second.lastResponse >= KS_HEALTH_PING_RATE) {
                Logger::Error("%s has not responded.", entry.first->GetName().c_str());
            }
        }

        return ks_success;
    }

    KsResultType ComponentHealthMonitor::HandleComponentResponse(ComponentActive* component) {
        m_ActiveComponentInfos[component].lastResponse = xTaskGetTickCount();
        Logger::Debug("Health response from %s", component->GetName().c_str());

        return ks_success;
    }

}
