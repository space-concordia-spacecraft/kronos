#include "ks_telemetry_logger.h"
#include "ks_command_transmitter.h"
#include "ks_command_ids.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(TelemetryLogger);

    TelemetryLogger::TelemetryLogger()
        : ComponentQueued("CQ_TLM_LOGGER") {}

    KsResult TelemetryLogger::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_scheduler_tick:
                KS_TRY(ks_error_component_process_event, Update());
                break;
            case ks_event_tlm_set_active_group:
                KS_TRY(ks_error_component_process_event, SetActiveTelemetryGroup(std::any_cast<uint8_t>(message.data)));
                break;
            case ks_event_tlm_list_groups:
                KS_TRY(ks_error_component_process_event, ListTelemetryGroups());
                break;
            case ks_event_tlm_list_channels:
                KS_TRY(ks_error_component_process_event, ListTelemetryChannels(std::any_cast<uint8_t>(message.data)));
                break;
        }

        return ComponentQueued::ProcessEvent(message);
    }

    KsResult TelemetryLogger::Update() {
        for (auto& rateGroup: m_TelemetryRateGroups) {
            // Retrieve telemetry data from each channel
            rateGroup.data.resize(rateGroup.channels.size());
            for (size_t i = 0; i < rateGroup.channels.size(); i++) {
                rateGroup.data[i] = rateGroup.channels[i].retrieveTelemetry();
            }

            // Write the vector to the ApolloExporter.
            rateGroup.apolloExporter.WriteRow(rateGroup.data);
            if (rateGroup.echo) {
                CommandTransmitter::TransmitPayload(
                    KS_CMD_ECHO_TLM,
                    (uint8_t*) rateGroup.data.data(),
                    rateGroup.data.size() * sizeof(uint32_t)
                );
            }
        }

        return ks_success;
    }

    KsResult TelemetryLogger::SetActiveTelemetryGroup(uint8_t grpIdx) {
        if (grpIdx != 0xFF && grpIdx >= m_TelemetryRateGroups.size()) KS_THROW(ks_error);

        for (size_t i = 0; i < m_TelemetryRateGroups.size(); i++) {
            m_TelemetryRateGroups[i].echo = i == grpIdx;
        }

        return ks_success;
    }

    KsResult TelemetryLogger::ListTelemetryGroups() {
        List <uint8_t> payload;
        size_t i = 0;
        for (const auto& group: m_TelemetryRateGroups) {
            payload.resize(payload.size() + group.name.size() + 1);
            memcpy(payload.data() + i, group.name.c_str(), group.name.size());
            payload[i + group.name.size()] = '\0';
            i = payload.size();
        }

        CommandTransmitter::TransmitPayload(KS_CMD_RES_TLM_GROUPS, payload.data(), payload.size());
        return ks_success;
    }

    KsResult TelemetryLogger::ListTelemetryChannels(uint8_t grpIdx) {
        if (grpIdx >= m_TelemetryRateGroups.size()) KS_THROW(ks_error);

        List <uint8_t> payload;
        size_t i = 0;
        for (const auto& channel: m_TelemetryRateGroups[grpIdx].channels) {
            payload.resize(payload.size() + channel.name.size() + 1);
            memcpy(payload.data() + i, channel.name.c_str(), channel.name.size());
            payload[i + channel.name.size()] = '\0';
            i = payload.size();
        }

        CommandTransmitter::TransmitPayload(KS_CMD_RES_TLM_CHANNELS, payload.data(), payload.size());
        return ks_success;
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

        ApolloExporter exporter{};
        KS_TRY(ks_error, exporter.Export("/" + name + ".apl", headers));

        // Initialize rate group.
        m_TelemetryRateGroups.push_back(
            {
                .name = name,
                .channels = channels,
                .data = {},
                .apolloExporter = exporter
            }
        );

        return ks_success;
    }


}
