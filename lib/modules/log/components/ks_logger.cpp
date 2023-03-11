#include "ks_logger.h"
#include "ks_framework.h"
#include "ks_packet_parser.h"
#include "ks_command_ids.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(Logger);

    Logger::Logger() :
        ComponentQueued("CQ_LOGGER"),
        m_File(KS_LOGGING_FILE_PATH),
        m_Bus(Framework::CreateBus("B_LOGGER")) {
        m_Bus->AddReceivingComponent(this);
    };

    void Logger::ProcessEvent(const EventMessage& eventMessage) {
        switch (eventMessage.eventCode) {
            case ks_event_log_message:
                ProcessMessage(eventMessage.Cast<LogEventMessage>());
                break;
            case ks_event_log_toggle_echo:
                SetEcho(!m_ShouldEcho);
                break;
        }
    }

    String Logger::ConvertSeverity(KsLogSeverity severity) {
        switch (severity) {
            case ks_log_debug:
                return "[DEBUG]";
            case ks_log_info:
                return "[INFO]";
            case ks_log_warn:
                return "[WARNING]";
            case ks_log_error:
                return "[ERROR]";
            default:
                return "";
        }
    }

    void Logger::ProcessMessage(const LogEventMessage& eventMessage) {
        // Only write to a file if the severity is over debug
        if (eventMessage.severity > ks_log_debug)
            m_File.Write(eventMessage.message.c_str(), eventMessage.message.size());

        // If we're not echoing we should just return out of the function
        if (!m_ShouldEcho)
            return;

        Bus* transmitBus = Framework::GetBus("B_CMD_TRANSMIT");
        if (transmitBus == nullptr)
            return;

        // Split the message into packets
        for (size_t i = 0; i < eventMessage.message.size(); i += KSP_MAX_PAYLOAD_SIZE) {
            Packet packet{};
            auto payloadSize = std::min<uint32_t>(KSP_MAX_PAYLOAD_SIZE, eventMessage.message.size() - i);
            auto offset = eventMessage.message.c_str() + i;
            auto flags = PacketFlags::none;

            memcpy(
                packet.Payload,
                offset,
                payloadSize
            );
            packet.Header.PayloadSize = payloadSize;

            if ((i + payloadSize) >= eventMessage.message.size()) {
                flags = PacketFlags::eof;
            }

            EncodePacket(packet, flags, KS_CMD_ECHO, (uint8_t*)offset, payloadSize);

            transmitBus->Publish(
                packet,
                ks_event_comms_transmit
            );
        }
    }

    void Logger::SetEcho(bool shouldEcho) {
        m_ShouldEcho = shouldEcho;
    }

}