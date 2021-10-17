#include "ks_csp.h"

namespace kronos {
    ComponentCspDriver::ComponentCspDriver(const String& name, uint32_t baudrate = CONF_UART_BAUDRATE, uint32_t charLength = CONF_UART_CHAR_LENGTH, uint32_t parityType = CONF_UART_PARITY, uint32_t stopBits = CONF_UART_STOP_BITS):ComponentPassive(name){

    }

    /* Server task - handles requests from clients */
    CSP_DEFINE_TASK(task_server) {

        csp_log_info("Server task started");

        /* Create socket with no specific socket options, e.g. accepts CRC32, HMAC, XTEA, etc. if enabled during compilation */
        csp_socket_t *sock = csp_socket(CSP_SO_NONE);

        /* Bind socket to all ports, e.g. all incoming connections will be handled here */
        csp_bind(sock, CSP_ANY);

        /* Create a backlog of 10 connections, i.e. up to 10 new connections can be queued */
        csp_listen(sock, 10);

        /* Wait for connections and then process packets on the connection */
        while (1) {

            /* Wait for a new connection, 10000 mS timeout */
            csp_conn_t *conn;
            if ((conn = csp_accept(sock, 10000)) == NULL) {
                /* timeout */
                continue;
            }

            /* Read packets on connection, timout is 100 mS */
            csp_packet_t *packet;
            while ((packet = csp_read(conn, 50)) != NULL) {
                switch (csp_conn_dport(conn)) {
                    case MY_SERVER_PORT:
                        /* Process packet here */
                    csp_log_info("Packet received on MY_SERVER_PORT: %s", (char *) packet->data);
                        csp_buffer_free(packet);
                        ++server_received;
                        break;

                    default:
                        /* Call the default CSP service handler, handle pings, buffer use, etc. */
                        csp_service_handler(conn, packet);
                        break;
                }
            }

            /* Close current connection */
            csp_close(conn);

        }

        return CSP_TASK_RETURN;

    }
/* End of server task */

/* Client task sending requests to server task */
    CSP_DEFINE_TASK(task_client) {

        csp_log_info("Client task started");

        unsigned int count = 0;

        while (1) {

            csp_sleep_ms(test_mode ? 200 : 1000);

            /* Send ping to server, timeout 1000 mS, ping size 100 bytes */
            int result = csp_ping(m_ServerAddress, 1000, 100, CSP_O_NONE);
            csp_log_info("Ping address: %u, result %d [mS]", m_ServerAddress, result);

            /* Send reboot request to server, the server has no actual implementation of csp_sys_reboot() and fails to reboot */
            csp_reboot(m_ServerAddress);
            csp_log_info("reboot system request sent to address: %u", m_ServerAddress);

            /* Send data packet (string) to server */

            /* 1. Connect to host on 'server_address', port MY_SERVER_PORT with regular UDP-like protocol and 1000 ms timeout */
            csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, m_ServerAddress, MY_SERVER_PORT, 1000, CSP_O_NONE);
            if (conn == NULL) {
                /* Connect failed */
                csp_log_error("Connection failed");
                return CSP_TASK_RETURN;
            }

            /* 2. Get packet buffer for message/data */
            csp_packet_t * packet = static_cast<csp_packet_t*>(csp_buffer_get(100));
            if (packet == NULL) {
                /* Could not get buffer element */
                csp_log_error("Failed to get CSP buffer");
                return CSP_TASK_RETURN;
            }

            /* 3. Copy data to packet */
            snprintf((char *) packet->data, csp_buffer_data_size(), "Hello World (%u)", ++count);

            /* 4. Set packet length */
            packet->length = (strlen((char *) packet->data) + 1); /* include the 0 termination */

            /* 5. Send packet */
            if (!csp_send(conn, packet, 1000)) {
                /* Send failed */
                csp_log_error("Send failed");
                csp_buffer_free(packet);
            }

            /* 6. Close connection */
            csp_close(conn);
        }

        return CSP_TASK_RETURN;
    }
/* End of client task */

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
        if ((m_ServerAddress == 255) ||  /* no server address specified, I must be server */
            (default_iface == NULL)) {  /* no interfaces specified -> run server & client via loopback */
            csp_thread_create(task_server, "SERVER", 1000, NULL, 0, NULL);
        }

        /* Start client thread */
        if ((m_ServerAddress != 255) ||  /* server address specified, I must be client */
            (default_iface == NULL)) {  /* no interfaces specified -> run server & client via loopback */
            csp_thread_create(task_client, "CLIENT", 1000, NULL, 0, NULL);
        }

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