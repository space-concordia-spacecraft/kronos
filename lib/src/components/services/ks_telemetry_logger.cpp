#include "ks_telemetry_logger.h"

namespace kronos {

    KsCmdResult ComponentTelemetryLogger::ProcessEvent(const EventMessage& message) {
        return nullptr;
    }

    KsResult ComponentTelemetryLogger::AddTelemetryGroup(uint32_t rate, const Vector<TelemetryChannel>& channels) {
        return 0;
    }

}