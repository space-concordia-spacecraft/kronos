#include "ks_telemetry_logger.h"
#include "ks_command_transmitter.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(TelemetryLogger);

    TelemetryLogger::TelemetryLogger()
        : ComponentQueued("CQ_TLM_LOGGER") {}

    void TelemetryLogger::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_scheduler_tick:
                Update();
                break;
            case ks_event_tlm_set_active_group:
                SetActiveTelemetryGroup(std::any_cast<uint8_t>(message.data));
                break;
            case ks_event_tlm_list_groups:
                ListTelemetryGroups();
                break;
            case ks_event_tlm_list_channels:
                ListTelemetryChannels(std::any_cast<uint8_t>(message.data));
                break;
        }
    }

    void TelemetryLogger::Update() {
        for (auto& rateGroup: m_TelemetryRateGroups) {
            // Retrieve telemetry data from each channel
            rateGroup.data.resize(rateGroup.channels.size());
            for (size_t i = 0; i < rateGroup.channels.size(); i++) {
                rateGroup.data[i] = rateGroup.channels[i].retrieveTelemetry();
            }

            // Write the vector to the ApolloExporter.
//            rateGroup.apolloExporter.WriteRow(rateGroup.data);
            if (rateGroup.echo) {
                CommandTransmitter::TransmitPayload(
                    KS_CMD_ECHO_TLM,
                    (uint8_t*) rateGroup.data.data(),
                    rateGroup.data.size() * sizeof(uint32_t)
                );
            }
        }
    }

    void TelemetryLogger::SetActiveTelemetryGroup(uint8_t grpIdx) {
        if (grpIdx != 0xFF && grpIdx >= m_TelemetryRateGroups.size())
            return;
        for (size_t i = 0; i < m_TelemetryRateGroups.size(); i++) {
            m_TelemetryRateGroups[i].echo = i == grpIdx;
        }
    }

    void TelemetryLogger::ListTelemetryGroups() {
        List <uint8_t> payload;
        size_t i = 0;
        for (const auto& group: m_TelemetryRateGroups) {
            payload.resize(payload.size() + group.name.size() + 1);
            memcpy(payload.data() + i, group.name.c_str(), group.name.size());
            payload[i + group.name.size()] = '\0';
            i = payload.size();
        }

        CommandTransmitter::TransmitPayload(KS_CMD_RES_TLM_GROUPS, payload.data(), payload.size());
    }

    void TelemetryLogger::ListTelemetryChannels(uint8_t grpIdx) {
        if (grpIdx >= m_TelemetryRateGroups.size())
            return;

        List <uint8_t> payload;
        size_t i = 0;
        for (const auto& channel: m_TelemetryRateGroups[grpIdx].channels) {
            payload.resize(payload.size() + channel.name.size() + 1);
            memcpy(payload.data() + i, channel.name.c_str(), channel.name.size());
            payload[i + channel.name.size()] = '\0';
            i = payload.size();
        }

        CommandTransmitter::TransmitPayload(KS_CMD_RES_TLM_CHANNELS, payload.data(), payload.size());
    }

    KsResult TelemetryLogger::_AddTelemetryGroup(
        const String& name,
        const List <TelemetryChannel>& channels
    ) {
        // Generate the headers for the file.
        List <ApolloHeader> headers;
        for (const auto& channel: channels) {
            headers.push_back(
                {
                    .name = channel.name,
                    .dataType = KS_APOLLO_FLOAT
                }
            );
        }

        // Initialize rate group.
        m_TelemetryRateGroups.push_back(
            {
                .name = name,
                .channels = channels,
                .apolloExporter = ApolloExporter("/" + name + ".apl", headers)
            }
        );

        return ks_success;
    }


}
