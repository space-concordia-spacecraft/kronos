#include "ks_telemetry_logger.h"

namespace kronos {
    ComponentTelemetryLogger::ComponentTelemetryLogger(const String& name) : ComponentActive(name) {}

    KsCmdResult ComponentTelemetryLogger::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_RATE_GROUP_TICK:
                for (TelemetryRateGroup i_RateGroup: m_TelemetryRateGroups) {
                    i_RateGroup.tickCount++;
                    if (i_RateGroup.tickCount >= i_RateGroup.tickRate) {
                        i_RateGroup.tickCount = 0;

                        // Get the values from the telemetry into a vector.
                        Vector <uint32_t> telemetryData;
                        for (TelemetryChannel i_Channel: i_RateGroup.channels) {
                            uint32_t telemetryValue = i_Channel.retrieveTelemetry();
                            telemetryData.Add(telemetryValue);
                        }

                        // Write the vector to the ApolloExporter.
                        i_RateGroup.apolloExporter.WriteRow(telemetryData);
                    }
                }
                break;
        }
        return ComponentActive::ProcessEvent(message);
    }

    KsResult ComponentTelemetryLogger::AddTelemetryGroup(const String& name, uint32_t rate,
                                                         const Vector <TelemetryChannel>& channels) {



        // Initialize the file for logging this telemetry group.
        File* file = ComponentFileManager::Get().Open("/" + name + ".txt",
                                                      KS_OPEN_MODE_CREATE | KS_OPEN_MODE_WRITE_ONLY);

        // Generate the headers for the file.
        Vector <ApolloHeader> headers;
        for (size_t i = 0; i < channels.Size(); i++) {
            headers[i].name = channels[i].name;
            headers[i].dataType = KS_APOLLO_FLOAT;
        }

        // Initialize rate group.
        TelemetryRateGroup newRateGroup;
        newRateGroup.name = name;
        newRateGroup.tickRate = rate;
        newRateGroup.channels = channels;
        newRateGroup.apolloExporter.Open(file, headers);

        m_TelemetryRateGroups.Add(newRateGroup);

        return KS_SUCCESS;
    }
}