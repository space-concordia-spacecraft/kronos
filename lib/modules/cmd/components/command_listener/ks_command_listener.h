#pragma once

#include "ks_component_active.h"
#include "ks_packet_parser.h"
#include "ks_io.h"

/* Server port, the port the server listens on for incoming connections from the client. */
#define MY_SERVER_PORT        10

namespace kronos {
    class CommandListener : public ComponentActive {
    public:
        explicit CommandListener(const std::string& name, IoDriver* ioDriver);

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        KsResultType Init() override;

    private:
        /* Commandline options */
        uint8_t m_ServerAddress = 255;
        uint16_t m_ServerReceived = 0;
        uint16_t m_Count = 0;
        bool m_Initialized = false;

        IoDriver* m_IoDriver;

        KsResultType Listen();
        KsResultType Send();

        static void DriverReceiveCallbackStub(const struct usart_async_descriptor *const io_descr);
        void DriverReceiveCallback(const struct usart_async_descriptor *const io_descr);
    };
}