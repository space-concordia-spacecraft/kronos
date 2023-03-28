#include "ks_housekeeping.h"
#include "ks_framework.h"
#include "ks_bus.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(HouseKeeping);

    KsResult HouseKeeping::Init() {
        // Store the busses necessary
        m_BusPong = Framework::GetBus(KS_BUS_HEALTH_PONG);
        m_BusPing = Framework::GetBus(KS_BUS_HEALTH_PING);

        m_BusPong->AddReceivingComponent(this);

        // Create the system clock
        m_Clock = CreateRef<Clock>(&CALENDAR_0);

        return ks_success;
    }

    KsResult HouseKeeping::PostInit() {
        auto list = Framework::GetInstance().m_ActiveComponents;

        std::erase_if(list, [](const String& name){
            return name == KS_COMPONENT_HOUSEKEEPING;
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
            KS_TRY(ks_error, m_BusPing->AddReceivingComponent(componentActive));
        }

        return ks_success;
    }

    HouseKeeping::HouseKeeping()
        : ComponentActive(
        KS_COMPONENT_HOUSEKEEPING,
        KS_QUEUE_DEFAULT_WAIT_TIME,
        KS_COMPONENT_STACK_SIZE_MEDIUM,
        KS_COMPONENT_PRIORITY_HIGH
    ), m_File(KS_HOUSEKEEPING_FILE_ERROR, KS_OPEN_MODE_WRITE_ONLY) {}

    KsResult HouseKeeping::ProcessEvent(const EventMessage& message) {
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

        return ComponentActive::ProcessEvent(message);
    }

    KsResult HouseKeeping::PingComponents() {
        KS_TRY(ks_error, m_BusPing->Publish(ks_event_health_ping, m_BusPong));

        for (auto [component, healthInfo]: m_ActiveComponentInfos) {
            uint32_t time = xTaskGetTickCount();
            if (time - healthInfo.lastResponse >= pdMS_TO_TICKS(KS_HOUSEKEEPING_PONG_MAX_RESPONSE_TIME)) {
                KS_DEBUGPRINT("Component '%s' has not responded.", component->GetName().c_str());
            }
        }

        return ks_success;
    }

    KsResult HouseKeeping::HandleComponentResponse(ComponentActive* component) {
        m_ActiveComponentInfos[component].lastResponse = xTaskGetTickCount();
        KS_DEBUGPRINT("Health response from component '%s'", component->GetName().c_str());

        return ks_success;
    }

    KsResult HouseKeeping::FlushStackTrace() {
        static char buf[200];
        while(!kronos::StackTrace::IsEmpty()) {
            kronos::ErrorInfo info;

            if(kronos::StackTrace::Pop(&info) != ks_success) return ks_error; // Throwing would add it back to the stack trace which we want to avoid
            snprintf(buf, sizeof(buf), "[ERROR:%ld] in %s at line %d", info.code, info.file.c_str(), info.line);

            // Print and write to file
            KS_DEBUGPRINT("%s", buf);
            KS_TRY(ks_error, m_File.Write(buf, sizeof(buf)));
        }

        return ks_success;
    }
}
