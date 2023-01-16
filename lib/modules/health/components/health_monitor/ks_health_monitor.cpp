#include "ks_health_monitor.h"
#include "ks_framework.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(HealthMonitor);

    HealthMonitor::HealthMonitor()
        : ComponentPassive("CP_HEALTH"),
          m_BusPing(Framework::CreateBus<BusAsync>("BA_HEALTH_PING", ks_event_health_ping)),
          m_BusPong(Framework::CreateBus<BusAsync>("BA_HEALTH_PONG", ks_event_health_pong)) {
        m_BusPong->AddReceivingComponent(this);
    }

    KsCmdResult HealthMonitor::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_scheduler_tick: {
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

    KsResultType HealthMonitor::_RegisterActiveComponent(ComponentActive* component) {
        if (m_ActiveComponentInfos.contains(component)) {
            Logger::Warn(
                "Component '%s' is already registered in the Health Monitor. Subsequent registration ignored.",
                component->GetName().c_str()
            );
            return ks_error_component_healthmonitor_already_registered;
        }

        m_BusPing->AddReceivingComponent(component);
        m_ActiveComponentInfos[component] = {};
        return ks_success;
    }

    KsResultType HealthMonitor::PingComponents() {
        Logger::Debug("Health ping ...");
        EventMessage message;
        message.eventCode = ks_event_health_ping;
        message.returnBus = m_BusPong;
        m_BusPing->Publish(message);

        for (auto entry: m_ActiveComponentInfos) {
            uint32_t time = xTaskGetTickCount();
            if (time - entry.second.lastResponse >= KS_HEALTH_PING_RATE) {
                Logger::Error("Component '%s' has not responded.", entry.first->GetName().c_str());
            }
        }

        return ks_success;
    }

    KsResultType HealthMonitor::HandleComponentResponse(ComponentActive* component) {
        m_ActiveComponentInfos[component].lastResponse = xTaskGetTickCount();
        Logger::Debug("Health response from component '%s'", component->GetName().c_str());

        return ks_success;
    }

}
