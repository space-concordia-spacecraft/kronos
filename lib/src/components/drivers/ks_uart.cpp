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
                uint8_t* data = reinterpret_cast<uint8_t*>(message.data);
                Write(data, sizeof(data));
                delete data;
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

    KsResult ComponentUartDriver::Write(const uint8_t *data, size_t len) {
        usart_serial_write_packet(CONF_UART, data, len);
        return KS_SUCCESS;
    }
}