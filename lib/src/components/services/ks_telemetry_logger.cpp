#include "ks_telemetry_logger.h"

namespace kronos {

    ComponentTelemetryLogger::ComponentTelemetryLogger(const std::string& name) : ComponentActive(name) {}

    ComponentTelemetryLogger::~ComponentTelemetryLogger() {
        for (auto& rateGroup: m_TelemetryRateGroups) {
            rateGroup.apolloExporter.Close();
        }
    }

    KsCmdResult ComponentTelemetryLogger::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_RATE_GROUP_TICK:
                for (auto& rateGroup: m_TelemetryRateGroups) {
                    rateGroup.tickCount++;
                    if (rateGroup.tickCount >= rateGroup.tickRate) {
                        rateGroup.tickCount = 0;

                        // Get the values from the telemetry into a vector.
                        std::vector<uint32_t> telemetryData;
                        for (TelemetryChannel i_Channel: rateGroup.channels) {
                            uint32_t telemetryValue = i_Channel.retrieveTelemetry();
                            telemetryData.push_back(telemetryValue);
                        }

                        // Write the vector to the ApolloExporter.
                        rateGroup.apolloExporter.WriteRow(telemetryData);
                    }
                }
                break;
        }
        return ComponentActive::ProcessEvent(message);
    }

    KsResult ComponentTelemetryLogger::AddTelemetryGroup(
        const std::string& name,
        uint32_t rate,
        const std::vector<TelemetryChannel>& channels) {
        // Initialize the file for logging this telemetry group.
        File* file = ComponentFileManager::Get().Open("/" + name + ".txt",
                                                      KS_OPEN_MODE_CREATE | KS_OPEN_MODE_WRITE_ONLY);

        // Generate the headers for the file.
        std::vector<ApolloHeader> headers;
        for (size_t i = 0; i < channels.size(); i++) {
            headers.push_back({
                .name = channels[i].name,
                .dataType = KS_APOLLO_FLOAT
            });
        }

        // Initialize rate group.
        m_TelemetryRateGroups.push_back({
            .name = name,
            .tickRate = rate,
            .channels = channels,
            .apolloExporter = ApolloExporter(file, headers)
        });

        return KS_SUCCESS;
    }

}