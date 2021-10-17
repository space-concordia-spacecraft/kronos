#pragma once

#include "ks_component_passive.h"
#include "usart.h"

namespace kronos {
    class ComponentUartDriver : public ComponentPassive {

    public:
        ComponentUartDriver(const String& name, uint32_t baudrate, uint32_t charLength, uint32_t parityType, uint32_t stopBits);

        KsCmdResult ProcessEvent(const EventMessage& message) override;

    private:

        usart_serial_options_t usartOptions = {
                .baudrate = CONF_UART_BAUDRATE,
                .charlength = CONF_UART_CHAR_LENGTH,
                .paritytype = CONF_UART_PARITY,
                .stopbits = CONF_UART_STOP_BITS
        };


    };
}