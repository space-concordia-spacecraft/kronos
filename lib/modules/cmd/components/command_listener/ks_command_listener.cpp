#include "ks_command_listener.h"
#include "ks_logger.h"
#include "ks_framework.h"
#include "ks_packet_parser.h"

namespace kronos {

    CommandListener::CommandListener(const String& name, IoDriver* ioDriver) :
        ComponentActive(name, KS_QUEUE_DEFAULT_WAIT_TIME, KS_COMPONENT_STACK_SIZE_SMALL, KS_COMPONENT_PRIORITY_HIGH), m_IoDriver(ioDriver) {
    }

    void CommandListener::Listen() {
        static constexpr const char s_MagicNumber[] = { KSP_MAGIC_BYTES };
        static uint8_t s_TempBuffer[1024] = {};

        Packet packet{};
        if (m_IoDriver->ReadUntil(
            s_TempBuffer,
            sizeof(s_TempBuffer),
            (uint8_t*)s_MagicNumber,
            sizeof(s_MagicNumber)
        ) <= 0) return;

        packet.Header.Magic = KSP_MAGIC;
        m_IoDriver->Read((uint8_t*)&packet.Header.PacketId, sizeof(packet.Header) - sizeof(packet.Header.Magic));
        int32_t size = ValidatePacketHeader(packet.Header);
        if (size < 0) {
            return;
        }

        m_IoDriver->Read((uint8_t*)&packet.Payload, size);
        if (!ValidatePacket(packet)) {
            return;
        }

        Packet returnPacket{};
        EncodePacket(returnPacket, packet.Header.PacketId, PacketFlags::ack, packet.Header.CommandId, nullptr, 0);
        Framework::GetBus("B_CMD_TRANSMIT")->Publish(returnPacket, ks_event_comms_transmit);
        Framework::GetBus("B_CMD_DISPATCH")->Publish(packet, ks_event_comms_dispatch);
    }

    void CommandListener::Run() {
        while (true) {
            Listen();
            taskYIELD();
        }
    }

}