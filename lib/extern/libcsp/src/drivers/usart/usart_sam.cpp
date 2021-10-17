#include <csp/drivers/usart.h>

#include "usart.h"

int csp_usart_open(const csp_usart_conf_t *conf, csp_usart_callback_t rx_callback, void * user_data, csp_usart_fd_t * fd) {
    // create usart options for same70
    usart_serial_options_t usartOptions = {
            .baudrate = conf->baudrate,
            .charlength = conf->databits,
            .paritytype = conf->paritysetting,
            .stopbits = conf->stopbits
    };

    // initialize the usart
    csp_usart_fd_t fd;
    usart_serial_init(fd, &usartOptions);

}

int csp_usart_write(csp_usart_fd_t fd, const void * data, size_t data_length) {
    // write the packet
    usart_serial_write_packet(fd, data, data_length);
}