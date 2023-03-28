#pragma once

#include "ks_file.h"
#include "ks_component_active.h"
#include "ks_clock.h"
#include "ks_framework.h"

#define KS_HOUSEKEEPING_FILE_ERROR             "/errors.log"
#define KS_HOUSEKEEPING_PONG_MAX_RESPONSE_TIME 15000

namespace kronos {
    enum KsLogSeverity {
        ks_log_trace,
        ks_log_debug,
        ks_log_info,
        ks_log_warn,
        ks_log_error
    };

    struct ComponentHealthInfo {
        uint32_t lastResponse = 0;
    };

    class HouseKeeping : public ComponentActive {
        KS_SINGLETON(HouseKeeping);

    public:
        HouseKeeping();
        ~HouseKeeping() override = default;

        KsResult Init() override;
        KsResult PostInit() override;
        KsResult ProcessEvent(const EventMessage& message) override;

    private:
        KsResult PingComponents();
        KsResult HandleComponentResponse(ComponentActive* component);
        KsResult FlushStackTrace();

    private:
        Map<ComponentActive*, ComponentHealthInfo> m_ActiveComponentInfos{};
        Bus* m_BusPong{};
        Bus* m_BusPing{};

        Ref<Clock> m_Clock;
        File m_File;
    };
}
