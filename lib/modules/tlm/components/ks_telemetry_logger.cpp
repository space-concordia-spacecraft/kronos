#include "ks_telemetry_logger.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(TelemetryLogger);

    void TelemetryLogger::_Update() {
        for (auto& rateGroup: m_TelemetryRateGroups) {
            rateGroup.tickCount++;
            if (rateGroup.tickCount >= rateGroup.tickRate) {
                rateGroup.tickCount = 0;

                // Get the values from the tlm into a vector.
                std::vector<uint32_t> telemetryData;
                for (const auto& i_Channel: rateGroup.channels) {
                    uint32_t telemetryValue = i_Channel.retrieveTelemetry();
                    telemetryData.push_back(telemetryValue);
                }

                // Write the vector to the ApolloExporter.
                rateGroup.apolloExporter.WriteRow(telemetryData);
            }
        }
    }

    KsResult TelemetryLogger::_AddTelemetryGroup(
        const std::string& name,
        uint32_t rate,
        const std::vector<TelemetryChannel>& channels
    ) {
        // Generate the headers for the file.
        std::vector<ApolloHeader> headers;
        for (const auto& channel: channels) {
            headers.push_back(
                {
                    .name = channel.name,
                    .dataType = KS_APOLLO_FLOAT
                }
            );
        }

        auto apolloExporter = ApolloExporter("/" + name + ".txt", headers);

        // Initialize rate group.
        m_TelemetryRateGroups.push_back(
            {
                .name = name,
                .tickRate = rate,
                .channels = channels,
                .apolloExporter = apolloExporter
            }
        );

        return ks_success;
    }

}