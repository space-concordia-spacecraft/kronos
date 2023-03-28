#include "ks_command_transmitter.h"
#include "ks_framework.h"

namespace kronos {

    CommandTransmitter::CommandTransmitter(const std::string& name, IoDescriptor* ioDriver) :
        ComponentQueued(name), m_IoDriver(ioDriver) {
    }

    KsResult CommandTransmitter::Init() {
        Framework::GetBus(KS_BUS_CMD_TRANSMIT)->AddReceivingComponent(this);

        return ks_success;
    }

    KsResult CommandTransmitter::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_comms_transmit:
                auto packet = message.Cast<Packet>();
                Transmit(packet);
                break;
        }

        return ks_success;
    }

    KsResult CommandTransmitter::Transmit(const Packet& packet) {
        m_IoDriver->Write((uint8_t*)&packet, sizeof(packet.Header) + packet.Header.PayloadSize);
        return ks_success;
    }

    KsResult CommandTransmitter::TransmitPayload(KsCommand cmd, const uint8_t* payload, size_t payloadSize, bool setEOF) {
        Bus* transmitBus = Framework::GetBus(KS_BUS_CMD_TRANSMIT);

        KspPacketIdxType i_packet{ 0 };
        for (size_t i = 0; i < payloadSize; i += KSP_MAX_PAYLOAD_SIZE_PART) {
            Packet packet{};
            auto partSize = std::min<uint32_t>(
                    KSP_MAX_PAYLOAD_SIZE_PART,
                    payloadSize - i
            );
            auto flags = (setEOF && (i + partSize) >= payloadSize) ?
                         PacketFlags::eof :
                         PacketFlags::none;
            EncodePacketPart(packet, flags, cmd, i_packet, (uint8_t*)(payload + i), partSize);
            KS_TRY(ks_error, transmitBus->Publish(
                    packet,
                    ks_event_comms_transmit
            ));
            i_packet++;
        }

        return ks_success;
    }

}
