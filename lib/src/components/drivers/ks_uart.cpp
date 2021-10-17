#include "ks_uart.h"

namespace kronos {
    ComponentUartDriver::ComponentUartDriver(const String& name, uint32_t baudrate, uint32_t charLength, uint32_t parityType, uint32_t stopBits):ComponentPassive(name) {
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
            case KS_EVENT_CODE_READ:

                break;
            case KS_EVENT_CODE_WRITE:
                Write();
                break;
            case KS_EVENT_CODE_RATE_GROUP_TICK:
                Write();
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

    KsResult ComponentUartDriver::Write() {
        usart_serial_putchar(CONF_UART, 'h');
        usart_serial_putchar(CONF_UART, 'e');
        usart_serial_putchar(CONF_UART, 'l');
        usart_serial_putchar(CONF_UART, 'l');
        usart_serial_putchar(CONF_UART, 'o');

        return KS_SUCCESS;
    }
}