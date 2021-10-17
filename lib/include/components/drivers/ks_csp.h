#pragma once

#include "ks_component_passive.h"

#include <csp/csp.h>
#include <csp/arch/csp_thread.h>
#include <csp/drivers/usart.h>

/* Server port, the port the server listens on for incoming connections from the client. */
#define MY_SERVER_PORT		10


namespace kronos {
    class ComponentCspDriver : public ComponentPassive {
    public:

        ComponentCspDriver(const String& name, uint32_t baudrate = CONF_UART_BAUDRATE, uint32_t charLength = CONF_UART_CHAR_LENGTH, uint32_t parityType = CONF_UART_PARITY, uint32_t stopBits = CONF_UART_STOP_BITS);

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        void Init() override;
    private:
        KsResult Read();
        KsResult Write();

        /* Commandline options */
        uint8_t m_ServerAddress = 255;
    };
}