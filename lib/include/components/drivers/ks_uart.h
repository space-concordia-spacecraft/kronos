#pragma once

#include "ks_component_passive.h"
#include "usart.h"

namespace kronos {
    class ComponentUartDriver : public ComponentPassive {

    public:
        ComponentUartDriver(const std::string& name, Usart* usart = CONF_UART, uint32_t baudrate = CONF_UART_BAUDRATE,
                            uint32_t charLength = CONF_UART_CHAR_LENGTH, uint32_t parityType = CONF_UART_PARITY,
                            uint32_t stopBits = CONF_UART_STOP_BITS);

        KsCmdResult ProcessEvent(const EventMessage& message) override;

    private:
        KsResult Read();

        KsResult Write(const uint8_t* data, size_t len);

        Usart* m_UsartInstance;
    };
}