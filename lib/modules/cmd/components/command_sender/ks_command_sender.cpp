#include "ks_command_sender.h"

namespace kronos {
    CommandSender::CommandSender(const std::string& name, IoDriver* ioDriver) :
        ComponentQueued(name), m_IoDriver(ioDriver){
    }

    KsResultType CommandSender::Init() {
//        usart_async_register_callback(m_Driver->GetDescriptor(), USART_ASYNC_RXC_CB, DriverReceiveCallbackStub);
    }

    KsCmdResult CommandSender::ProcessEvent(const EventMessage& message) {
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
        return KS_CMDRESULT_NORETURN;
    }

    KsResultType CommandSender::Send(const char* buf, uint8_t bufSize) {
        // Should we just have all the commands here????? as a private functions

        return ks_success;
    }
}
