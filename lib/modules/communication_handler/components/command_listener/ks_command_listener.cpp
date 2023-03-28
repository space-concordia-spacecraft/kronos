#include "ks_command_listener.h"
#include "ks_framework.h"
#include "ks_packet_parser.h"
#include "ks_bus.h"

namespace kronos {

    CommandListener::CommandListener(const String& name, IoDescriptor* ioDriver) :
        ComponentActive(name, KS_QUEUE_DEFAULT_WAIT_TIME, KS_COMPONENT_STACK_SIZE_MEDIUM, KS_COMPONENT_PRIORITY_HIGH), m_IoDriver(ioDriver) {
    }

    KsResult CommandListener::Listen() {
        static constexpr const char s_MagicNumber[] = { KSP_MAGIC_BYTES };
        static uint8_t s_TempBuffer[1024] = {};

        Packet packet{};
        if (m_IoDriver->ReadUntil(
            s_TempBuffer,
            sizeof(s_TempBuffer),
            (uint8_t*)s_MagicNumber,
            sizeof(s_MagicNumber)
        ) <= 0) return ks_success;

        packet.Header.Magic = KSP_MAGIC;
        m_IoDriver->Read((uint8_t*)&packet.Header.PacketId, sizeof(packet.Header) - sizeof(packet.Header.Magic));
        int32_t size = ValidatePacketHeader(packet.Header);
        if (size < 0)
            return ks_success;

        m_IoDriver->Read((uint8_t*)&packet.Payload, size);
        if (!ValidatePacket(packet))
            return ks_success;

        Packet returnPacket{};
        EncodePacket(returnPacket, packet.Header.PacketId, PacketFlags::ack, packet.Header.CommandId, nullptr, 0);
        Bus* bus = Framework::GetBus(KS_BUS_CMD_TRANSMIT);
        bus->Publish(returnPacket, ks_event_comms_transmit);

        Framework::GetBus(KS_BUS_CMD_DISPATCH)->Publish(packet, ks_event_comms_dispatch);

        return ks_success;
    }

    void CommandListener::Run() {
        while (true) {
            Listen();
            taskYIELD();
        }
    }

}