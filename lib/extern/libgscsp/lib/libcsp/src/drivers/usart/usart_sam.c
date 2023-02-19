#include <csp/drivers/usart.h>

#include <driver_init.h>

#include <csp/csp.h>
#include <csp/arch/csp_malloc.h>
#include <csp/arch/csp_thread.h>

typedef struct {
    csp_usart_callback_t rx_callback;
} usart_context_t;

static usart_context_t* UHF_CONTEXT;
static usart_context_t* PAYLOAD_CONTEXT;

static void usart_rx_callback(struct usart_os_descriptor* uart_descriptor) {
//    struct io_descriptor* io;
//    usart_os_get_io(uart_descriptor, &io);
//
//    usart_context_t* ctx;
//
//    if (uart_descriptor == &USART_0)
//        ctx = UHF_CONTEXT;
//    else if (uart_descriptor == &USART_0)
//        ctx = PAYLOAD_CONTEXT;
//
//    uint8_t buff[10];
//    io_read(io, buff, sizeof(buff));
//
//    // TODO: Figure out if the last param has to be NULL or the reference to the task (this is called from an ISR)
//    // Figure out wtf the first param is
//    ctx->rx_callback(NULL, buff, sizeof(buff), NULL);
}

int csp_usart_write(csp_usart_fd_t fd, const void* data, size_t data_length) {
    // I have no clue what the fuck to write from fd so for now Im just making it always output it to UHF
    struct io_descriptor* io;
    usart_os_get_io(&USART_UHF, &io);
    io_write(io, (const uint8_t*)data, data_length);
    return 0;
}

int csp_usart_open(
    const csp_usart_conf_t* conf, csp_usart_callback_t rx_callback, void* user_data, csp_usart_fd_t* return_fd
) {

    struct usart_os_descriptor* device = &USART_UHF;

    usart_context_t* ctx = csp_calloc(1, sizeof(*ctx));

    if (ctx == NULL) {
        return CSP_ERR_NOMEM;
    }

    usart_os_set_baud_rate(device, conf->baudrate);
//    usart_os_set_character_size(device, conf->databits);
//    usart_os_set_stopbits(device, conf->stopbits);
//    usart_os_set_parity(device, conf->paritysetting);

//    usart_os_(device, USART_ASYNC_RXC_CB, usart_rx_callback);
//    usart_async_enable(device);
//
//    if (device == &USART_UHF)
//        UHF_CONTEXT = ctx;
//    else if (device == &USART_PL)
//        PAYLOAD_CONTEXT = ctx;

    return CSP_ERR_NONE;
}