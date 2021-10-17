#include "ks_csp.h"

namespace kronos {
    ComponentCspDriver::ComponentCspDriver(const String& name, uint32_t baudrate = CONF_UART_BAUDRATE, uint32_t charLength = CONF_UART_CHAR_LENGTH, uint32_t parityType = CONF_UART_PARITY, uint32_t stopBits = CONF_UART_STOP_BITS):ComponentPassive(name){

    }

    void ComponentCspDriver::Init() {
        // initialize conf information
        uint8_t address = 1;
        csp_debug_level_t debug_level = CSP_INFO;

        const char * kiss_device = NULL;
        const char * rtable = NULL;

        // enable/disable debug levels
        for (int i = 0; i <= CSP_LOCK; ++i) {
            csp_debug_set_level(static_cast<csp_debug_level_t>(i), (i <= debug_level) ? true : false);
        }

        /* Init CSP with address and default settings */
        csp_conf_t csp_conf;
        csp_conf_get_defaults(&csp_conf);
        csp_conf.address = address;
        int error = csp_init(&csp_conf);
        if (error != CSP_ERR_NONE) {
            csp_log_error("csp_init() failed, error: %d", error);
            exit(1);
        }

        /* Add interface(s) */
        csp_iface_t * default_iface = NULL;
        if (kiss_device) {
            csp_usart_conf_t conf = {
                    .device = kiss_device,
                    .baudrate = 115200, /* supported on all platforms */
                    .databits = 8,
                    .stopbits = 1,
                    .paritysetting = 0,
                    .checkparity = 0};
            error = csp_usart_open_and_add_kiss_interface(&conf, CSP_IF_KISS_DEFAULT_NAME,  &default_iface);
            if (error != CSP_ERR_NONE) {
                csp_log_error("failed to add KISS interface [%s], error: %d", kiss_device, error);
                exit(1);
            }
        }

        if (rtable) {
            error = csp_rtable_load(rtable);
            if (error < 1) {
                csp_log_error("csp_rtable_load(%s) failed, error: %d", rtable, error);
                exit(1);
            }
        } else if (default_iface) {
            csp_rtable_set(CSP_DEFAULT_ROUTE, 0, default_iface, CSP_NO_VIA_ADDRESS);
        } else {
            /* no interfaces configured - run server and client in process, using loopback interface */
            m_ServerAddress = address;
        }

        printf("Connection table\r\n");
        csp_conn_print_table();

        printf("Interfaces\r\n");
        csp_route_print_interfaces();

        printf("Route table\r\n");
        csp_route_print_table();

        /* Start server thread */
//        if ((m_ServerAddress == 255) ||  /* no server address specified, I must be server */
//            (default_iface == NULL)) {  /* no interfaces specified -> run server & client via loopback */
//            csp_thread_create(task_server, "SERVER", 1000, NULL, 0, NULL);
//        }

        /* Start client thread */
//        if ((m_ServerAddress != 255) ||  /* server address specified, I must be client */
//            (default_iface == NULL)) {  /* no interfaces specified -> run server & client via loopback */
//            csp_thread_create(task_client, "CLIENT", 1000, NULL, 0, NULL);
//        }

    }

    KsCmdResult ComponentCspDriver::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_READ:

                break;
            case KS_EVENT_CODE_WRITE:
                Write();
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }
}