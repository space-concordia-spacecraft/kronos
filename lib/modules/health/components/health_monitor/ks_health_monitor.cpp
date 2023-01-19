#include "ks_health_monitor.h"
#include "ks_framework.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(HealthMonitor);

    HealthMonitor::HealthMonitor()
        : ComponentWorker(
        "W_HEALTH",
        ks_event_health_ping
    ),
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
                HandleComponentResponse(message.Cast<ComponentActive>());
                break;
            }
            default:
                return KS_CMDRESULT_NORETURN;
        }
        return KS_CMDRESULT_NORETURN;
    }

    KsResultType HealthMonitor::PostInit() {
        const auto& list = Framework::GetInstance().m_ActiveComponents;
        std::transform(
            list.begin(),
            list.end(),
            std::inserter(m_ActiveComponentInfos, m_ActiveComponentInfos.end()),
            [](const String& name) {
                return std::pair<const ComponentActive*, ComponentHealthInfo>{
                    dynamic_cast<ComponentActive*>(Framework::GetInstance().m_Components[name].get()),
                    {}
                };
            }
        );

        for(const auto& [componentActive, healthInfo] : m_ActiveComponentInfos) {
            m_BusSend->AddReceivingComponent(componentActive);
        }

        return ks_success;
    }

    KsResultType HealthMonitor::PingComponents() {
        EventMessage message;
        message.eventCode = ks_event_health_ping;
        message.returnBus = m_BusPong;
        m_BusSend->Publish(message);

        for (auto [component, healthInfo]: m_ActiveComponentInfos) {
            uint32_t time = xTaskGetTickCount();
            if (time - healthInfo.lastResponse >= KS_HEALTH_PONG_MAX_RESPONSE_TIME) {
                Logger::Error("Component '%s' has not responded.", component->GetName().c_str());
            }
        }

        return ks_success;
    }

    KsResultType HealthMonitor::HandleComponentResponse(const ComponentActive& component) {
        m_ActiveComponentInfos[&component].lastResponse = xTaskGetTickCount();
        Logger::Debug("Health response from component '%s'", component.GetName().c_str());

        return ks_success;
    }

}
