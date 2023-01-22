#include "ks_command_listener.h"
#include "ks_logger.h"
#include "ks_framework.h"

namespace kronos {
    CommandListener::CommandListener(const String& name, IoDriver* ioDriver) :
        ComponentQueued(name), m_IoDriver(ioDriver) {
    }

    void CommandListener::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_scheduler_tick:
                Listen();
                break;
        }
    }

    KsResultType CommandListener::Listen() {
        Packet packet{};
        m_IoDriver->Read((uint8_t*) &packet.Header, sizeof(packet.Header));

        int32_t size = ValidatePacketHeader(&packet.Header);

        if (size < 0) {
            return ks_error_invalid_packet_header;
        }

        m_IoDriver->Read((uint8_t*) &packet.Payload, size);

        if (!ValidatePacket(&packet)) {
            return ks_error_invalid_packet;
        }

        Framework::GetBus("B_CMD_DISPATCHER")->Publish(packet.Header.CommandId);

        Packet returnPacket{};
        EncodePacket(&returnPacket, PacketFlags::ack, packet.Header.CommandId, nullptr, 0);

        Framework::GetBus("B_CMD_TRANSMIT")->Publish(returnPacket, ks_event_comms_transmit);

        return ks_success;
    }
}