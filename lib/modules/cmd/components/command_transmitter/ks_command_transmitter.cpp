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
                Transmit(message.Cast<Packet>());
                break;
        }
    }

    KsResultType CommandTransmitter::Transmit(Packet packet) {
        m_IoDriver->Write((uint8_t*) &packet, sizeof(packet.Header) + packet.Header.PayloadSize);
        return ks_success;
    }
}
