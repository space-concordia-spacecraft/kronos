#include "ks_health_monitor.h"
#include "ks_framework.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(HealthMonitor);

    HealthMonitor::HealthMonitor()
        : ComponentActive(
        "CA_HEALTH",
        KS_QUEUE_DEFAULT_WAIT_TIME,
        KS_COMPONENT_STACK_SIZE_MEDIUM
    ),
          m_BusPong(Framework::GetBus("B_HEALTH_PONG")),
          m_BusPing(Framework::GetBus("B_HEALTH_PING")) {
        m_BusPong->AddReceivingComponent(this);
    }

    void HealthMonitor::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_health_ping: {
                PingComponents();
                break;
            }
            case ks_event_health_pong: {
                HandleComponentResponse(message.Cast<ComponentActive*>());
                break;
            }
        }
    }

    KsResultType HealthMonitor::PostInit() {
        auto list = Framework::GetInstance().m_ActiveComponents;

        std::erase_if(list, [](const String& name){
            return name == "CA_HEALTH" || name == "CA_CMD_LISTENER";
        });

        std::transform(
            list.begin(),
            list.end(),
            std::inserter(m_ActiveComponentInfos, m_ActiveComponentInfos.end()),
            [](const String& name) {
                return std::pair<ComponentActive*, ComponentHealthInfo>{
                    dynamic_cast<ComponentActive*>(Framework::GetInstance().m_Components[name].get()),
                    {}
                };
            }
        );

        for (const auto& [componentActive, healthInfo]: m_ActiveComponentInfos) {
            m_BusPing->AddReceivingComponent(componentActive);
        }

        return ks_success;
    }

    KsResultType HealthMonitor::PingComponents() {
        m_BusPing->Publish(ks_event_health_ping, m_BusPong);

        for (auto [component, healthInfo]: m_ActiveComponentInfos) {
            uint32_t time = xTaskGetTickCount();
            if (time - healthInfo.lastResponse >= pdMS_TO_TICKS(KS_HEALTH_PONG_MAX_RESPONSE_TIME)) {
                Logger::Error("Component '%s' has not responded.", component->GetName().c_str());
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
