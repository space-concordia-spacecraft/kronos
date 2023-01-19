#include "ks_command_listener.h"
#include "ks_logger.h"

namespace kronos {
    CommandListener::CommandListener(const std::string& name, IoDriver* ioDriver) :
        ComponentActive(name), m_IoDriver(ioDriver){
    }

    KsResultType CommandListener::Init() {
//        usart_async_register_callback(m_Driver->GetDescriptor(), USART_ASYNC_RXC_CB, DriverReceiveCallbackStub);
    }

    KsCmdResult CommandListener::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
//            case KS_EVENT_CODE_RATE_GROUP_TICK:
//                break;
//            case KS_EVENT_CODE_READ:
//                Read();
//                break;
//            case KS_EVENT_CODE_WRITE:
//                Write();
//                break;
        }
        return ComponentActive::ProcessEvent(message);
    }

    KsResultType CommandListener::Listen() {
        Packet packet{};
        m_IoDriver->Read((uint8_t*)&packet.Header, sizeof(packet.Header));

        int32_t size = ValidatePacketHeader(&packet.Header);

        if(size < 0) {
            return ks_error_invalid_packet_header;
        }

        m_IoDriver->Read((uint8_t*) &packet.Payload, size);

        if(!ValidatePacket(&packet)) {
            return ks_error_invalid_packet;
        }

        // TODO: Let'use the payload as a flag connected to a bus

        return ks_success;
    }
}