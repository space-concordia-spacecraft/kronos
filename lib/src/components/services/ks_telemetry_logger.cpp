#include "ks_telemetry_logger.h"

namespace kronos {

    KsCmdResult ComponentTelemetryLogger::ProcessEvent(const EventMessage& message) {
        switch(message.opcode) {
            case KS_EVENT_CODE_RATE_GROUP_TICK:
                for(TelemetryRateGroup i_RateGroup : m_TelemetryRateGroups) {
                    i_RateGroup.tickCount++;
                    if (i_RateGroup.tickCount >= i_RateGroup.tickRate) {
                        i_RateGroup.tickCount = 0;
                        // TODO: Log Telemetry
                    }
                }
                break;
        }
        return ComponentActive::ProcessEvent(message);
    }

    KsResult ComponentTelemetryLogger::AddTelemetryGroup(uint32_t rate, const Vector<TelemetryChannel>& channels) {
        TelemetryRateGroup newRateGroup;
        newRateGroup.tickCount = 0;
        newRateGroup.tickRate = rate;
        newRateGroup.channels = channels;

        m_TelemetryRateGroups.Add(newRateGroup);

        return KS_SUCCESS;
    }

}