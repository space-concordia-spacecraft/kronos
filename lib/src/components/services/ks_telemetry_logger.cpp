#include "ks_telemetry_logger.h"

namespace kronos {

    KsCmdResult ComponentTelemetryLogger::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_RATE_GROUP_TICK:
                for (TelemetryRateGroup i_RateGroup: m_TelemetryRateGroups) {
                    i_RateGroup.tickCount++;
                    if (i_RateGroup.tickCount >= i_RateGroup.tickRate) {
                        i_RateGroup.tickCount = 0;

                        // Get the values from the telemetry into a vector.
                        Vector<uint32_t> telemetryData;
                        for(TelemetryChannel i_Channel : i_RateGroup.channels) {
                            telemetryData.Add(i_Channel.retrieveTelemetry());
                        }

                        // Write the vector to the ApolloExporter.
                        i_RateGroup.apolloExporter->WriteRow(telemetryData);
                    }
                }
                break;
        }
        return ComponentActive::ProcessEvent(message);
    }

    KsResult ComponentTelemetryLogger::AddTelemetryGroup(const String& name, uint32_t rate, const Vector<TelemetryChannel>& channels) {
        if(m_FileManager == nullptr) {
            m_FileManager = new ComponentFileManager("File Manager", "C:");
        }

        TelemetryRateGroup newRateGroup;
        newRateGroup.name = name;
        newRateGroup.tickCount = 0;
        newRateGroup.tickRate = rate;
        newRateGroup.channels = channels;

        // Initialize the file for logging this telemetry group.
        File* file = m_FileManager->Open("/" + name + ".txt", KS_OPEN_MODE_CREATE | KS_OPEN_MODE_WRITE_ONLY);

        Vector<ApolloHeader> headers;
        for(size_t i = 0; i < channels.Size(); i++) {
            headers[i].name = channels[i].name;
            headers[i].dataType = KS_APOLLO_FLOAT;
        }

        newRateGroup.apolloExporter = new ApolloExporter(file, headers);

        m_TelemetryRateGroups.Add(newRateGroup);

        return KS_SUCCESS;
    }

}