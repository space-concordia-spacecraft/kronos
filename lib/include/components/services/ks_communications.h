#pragma once

#include "ks_component_passive.h"

#include "ks_framework.h"

#include "csp/csp.h"
#include "csp/arch/csp_thread.h"
#include "csp/drivers/usart.h"

/* Server port, the port the server listens on for incoming connections from the client. */
#define MY_SERVER_PORT        10

namespace kronos {
    class ComponentCommunications : public ComponentPassive {
    public:

        ComponentCommunications(const std::string& name, uint32_t baudrate = CONF_UART_BAUDRATE,
                           uint32_t charLength = CONF_UART_CHAR_LENGTH, uint32_t parityType = CONF_UART_PARITY,
                           uint32_t stopBits = CONF_UART_STOP_BITS);

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        KsResult Init() override;

    private:
        KsResult Read();
        KsResult Write();

        /* Commandline options */
        uint8_t m_ServerAddress = 255;
        csp_socket_t* m_Socket;
        uint16_t m_ServerReceived = 0;
        uint16_t m_Count = 0;
        bool m_Initialized = false;
    };
}