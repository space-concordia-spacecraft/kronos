#include <csp/drivers/usart.h>

#include "status_codes.h"

#include "uart_serial.h"

int csp_usart_open(const csp_usart_conf_t *conf, csp_usart_callback_t rx_callback, void * user_data, csp_usart_fd_t * fd) {
    // create usart options for same70
    usart_serial_options_t usartOptions = {
            .baudrate = conf->baudrate,
            .charlength = conf->databits,
            .paritytype = conf->paritysetting,
            .stopbits = conf->stopbits
    };

    *fd = (int)USART0;

    // initialize the usart
    usart_serial_init(USART0, &usartOptions);

    return CSP_ERR_NONE;
}

int csp_usart_write(csp_usart_fd_t fd, const void * data, size_t data_length) {
    // write the packet
    usart_serial_write_packet((usart_if) fd, data, data_length);
    return CSP_ERR_NONE;
}