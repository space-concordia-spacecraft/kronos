#include "ks_health_monitor.h"
#include "ks_framework.h"

namespace kronos {

    ComponentHealthMonitor::ComponentHealthMonitor(const String& name, BusBase* healthIn, BusBase* healthOut)
            : ComponentQueued(name), m_HealthIn(healthIn), m_HealthOut(healthOut) {}

    KsCmdResult ComponentHealthMonitor::ProcessEvent(const EventMessage& message) {
        switch (message.opcode) {
            case KS_EVENT_CODE_RATE_GROUP_TICK:
                PingComponents();
                break;
            case KS_EVENT_CODE_HEALTH_RESPONSE:
                auto* component = reinterpret_cast<ComponentActive*>(message.data);
                HandleComponentResponse(component);
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

    void ComponentHealthMonitor::RegisterActiveComponent(ComponentActive* component) {
        ComponentInfo tempInfo;
        if (m_ActiveComponentInfos.Peek(component, &tempInfo)) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }
        m_ActiveComponentInfos.Put(component, tempInfo);
    }

    void ComponentHealthMonitor::PingComponents() {
        Framework::LogDebug("Health ping");
        EventMessage message;
        message.opcode = KS_EVENT_CODE_HEALTH_PING;
        message.returnBus = m_HealthIn;
        m_HealthOut->Publish(message);

        for (auto entry : m_ActiveComponentInfos) {
            uint32_t time = xTaskGetTickCount();
            if (time - entry.GetValue().lastResponse >= KS_HEALTH_PING_RATE) {
                // TODO: Component has not responded
            }
        }
    }

    void ComponentHealthMonitor::HandleComponentResponse(ComponentActive* component) {
        m_ActiveComponentInfos[component].lastResponse = xTaskGetTickCount();
        Framework::LogDebug("Health response from " + component->GetName());
    }

}

