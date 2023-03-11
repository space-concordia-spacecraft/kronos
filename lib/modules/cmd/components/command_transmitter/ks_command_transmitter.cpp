#include "ks_command_transmitter.h"
#include "ks_framework.h"

namespace kronos {

    CommandTransmitter::CommandTransmitter(const std::string& name, IoDriver* ioDriver) :
        ComponentQueued(name),
        m_Bus(Framework::CreateBus("B_CMD_TRANSMIT")), m_IoDriver(ioDriver) {
        m_Bus->AddReceivingComponent(this);
    }

    void CommandTransmitter::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_comms_transmit:
                auto packet = message.Cast<Packet>();
                Transmit(packet);
                break;
        }
    }

    KsResultType CommandTransmitter::Transmit(const Packet& packet) {
        m_IoDriver->Write((uint8_t*)&packet, sizeof(packet.Header) + packet.Header.PayloadSize);
        return ks_success;
    }

    void CommandTransmitter::TransmitPayload(KsCommand cmd, const uint8_t* payload, size_t payloadSize) {
        Bus* transmitBus = Framework::GetBus("B_CMD_TRANSMIT");
        if (transmitBus == nullptr)
            return;

        KspPacketIdxType i_packet{ 0 };
        for (size_t i = 0; i < payloadSize; i += KSP_MAX_PAYLOAD_SIZE_PART) {
            Packet packet{};
            auto partSize = std::min<uint32_t>(
                    KSP_MAX_PAYLOAD_SIZE_PART,
                    payloadSize - i
            );
            auto flags = (i + partSize) >= payloadSize ?
                         PacketFlags::eof :
                         PacketFlags::none;
            EncodePacketPart(packet, flags, cmd, i_packet, (uint8_t*)(payload + i), payloadSize);
            transmitBus->Publish(
                    packet,
                    ks_event_comms_transmit
            );
            i_packet++;
        }
    }

}
