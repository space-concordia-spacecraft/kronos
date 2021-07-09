#include "console.h"

void console_init() {
    usart_serial_options_t usartOptions = {
            .baudrate = CONF_UART_BAUDRATE,
            .charlength = CONF_UART_CHAR_LENGTH,
            .paritytype = CONF_UART_PARITY,
            .stopbits = CONF_UART_STOP_BITS
    };
    stdio_serial_init(CONF_UART, &usartOptions);
}