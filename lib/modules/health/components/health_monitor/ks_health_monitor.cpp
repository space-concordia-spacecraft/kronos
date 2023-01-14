#include "ks_health_monitor.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(HealthMonitor);

    HealthMonitor::HealthMonitor()
        : ComponentQueued("CQ_HEALTH"),
          m_BusPing("B_HEALTH_PING", ks_event_health_ping),
          m_BusPong("B_HEALTH_PONG", ks_event_health_pong) {}

    KsCmdResult HealthMonitor::ProcessEvent(const EventMessage& message) {
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
            default:
                return KS_CMDRESULT_NORETURN;
        }
        return KS_CMDRESULT_NORETURN;
    }

    KsResultType HealthMonitor::RegisterActiveComponent(const Ref <ComponentActive>& component) {
        if (m_ActiveComponentInfos.count(component.get())) {
            Logger::Warn(
                "Component '{}' is already registered in the Health Monitor. Subsequent registration ignored.",
                component->GetName()
            );
            return ks_error_component_healthmonitor_already_registered;
        }

        ComponentHealthInfo tempInfo;
        m_ActiveComponentInfos[component.get()] = tempInfo;
        return ks_success;
    }

    KsResultType HealthMonitor::PingComponents() {
        Logger::Debug("Health ping.");
        EventMessage message;
        message.eventCode = ks_event_health_ping;
        message.returnBus = &m_BusPong;
        m_BusPing.Publish(message);

        for (auto entry: m_ActiveComponentInfos) {
            uint32_t time = xTaskGetTickCount();
            if (time - entry.second.lastResponse >= KS_HEALTH_PING_RATE) {
                Logger::Error("Component '{}' has not responded.", entry.first->GetName());
            }
        }

        return ks_success;
    }

    KsResultType HealthMonitor::HandleComponentResponse(ComponentActive* component) {
        m_ActiveComponentInfos[component].lastResponse = xTaskGetTickCount();
        Logger::Debug("Health response from component '{}'", component->GetName());

        return ks_success;
    }

}
