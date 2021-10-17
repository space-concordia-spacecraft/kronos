#include "ks_uart.h"

namespace kronos {
    ComponentUartDriver::ComponentUartDriver(const String& name, uint32_t baudrate = CONF_UART_BAUDRATE, uint32_t charLength = CONF_UART_CHAR_LENGTH, uint32_t parityType = CONF_UART_PARITY, uint32_t stopBits = CONF_UART_STOP_BITS):ComponentPassive(name) {
        usart_serial_options_t usartOptions = {
                .baudrate = baudrate,
                .charlength = charLength,
                .paritytype = parityType,
                .stopbits = stopBits
        };

        usart_serial_init(CONF_UART, &usartOptions);
    }

    KsCmdResult ComponentUartDriver::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_LOG_MESSAGE:

                break;
        }
        return ComponentPassive::ProcessEvent(message);
    }
}