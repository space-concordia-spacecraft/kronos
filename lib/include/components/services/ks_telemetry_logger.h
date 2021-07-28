#pragma once

#include "ks_component_active.h"

#include "ks_vector.h"

namespace kronos {

    struct TelemetryChannel {
        String name;
        uint32_t (*telemetry)();
    };

    struct TelemetryRateGroup {
        uint32_t tickCount = 0;
        uint32_t tickRate;
        Vector<TelemetryChannel> channels;
    };

    class ComponentTelemetryLogger : ComponentActive {
    public:
        KsCmdResult ProcessEvent(const EventMessage& message) override;

        KsResult AddTelemetryGroup(uint32_t rate, const Vector<TelemetryChannel>& channels);
    private:
        Vector<TelemetryRateGroup> m_TelemetryRateGroups;
        KsResult Log();
    };

}