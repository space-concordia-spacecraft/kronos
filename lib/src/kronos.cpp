#include "kronos.h"

#include <stdarg.h>
#include <stdio.h>

#include <fcntl.h>
#include <errno.h>

#include <vector>

extern "C" {

    void vApplicationMallocFailedHook(void) {
        configASSERT((volatile void*) nullptr);
    }

    void vApplicationStackOverflowHook(TaskHandle_t pxTask, char* pcTaskName) {
        configASSERT((volatile void*) nullptr);
    }

    void vApplicationIdleHook(void) {}

    void vApplicationTickHook(void) {}

    int printf(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        return vprintf(fmt, args);
    }

    int scanf(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        return vscanf(fmt, args);
    }

}

void console_init() {
    usart_serial_options_t usartOptions = {
            .baudrate = CONF_UART_BAUDRATE,
            .charlength = CONF_UART_CHAR_LENGTH,
            .paritytype = CONF_UART_PARITY,
            .stopbits = CONF_UART_STOP_BITS
    };
    stdio_serial_init(CONF_UART, &usartOptions);
}

