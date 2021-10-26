#include "ks_csp.h"

namespace kronos {
    ComponentCspDriver::ComponentCspDriver(const String& name, uint32_t baudrate = CONF_UART_BAUDRATE, uint32_t charLength = CONF_UART_CHAR_LENGTH, uint32_t parityType = CONF_UART_PARITY, uint32_t stopBits = CONF_UART_STOP_BITS):ComponentPassive(name){

    }

    KsResult ComponentCspDriver::Init() {
        m_Socket = csp_socket(CSP_SO_NONE);

        csp_bind(m_Socket, CSP_ANY);

        csp_listen(m_Socket, 10);
    }

    KsCmdResult ComponentCspDriver::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_READ:
                Read();
                break;
            case KS_EVENT_CODE_WRITE:
                Write();
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

    KsResult ComponentCspDriver::Read() {
        /* Wait for a new connection, 10000 mS timeout */
        csp_conn_t *conn;
        if ((conn = csp_accept(m_Socket, 1000)) == NULL) {
            /* timeout */
            // TODO: in the example this continues the loop. Meaning it does nothing.
            return KS_SUCCESS;
        }

        /* Read packets on connection, timout is 100 mS */
        csp_packet_t *packet;
        while ((packet = csp_read(conn, 50)) != NULL) {
            switch (csp_conn_dport(conn)) {
                case MY_SERVER_PORT:
                    /* Process packet here */
                csp_log_info("Packet received on MY_SERVER_PORT: %s", (char *) packet->data);
                    csp_buffer_free(packet);
                    ++m_ServerReceived;
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

    KsResult ComponentCspDriver::Write() {
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
            return KS_ERROR;
        }

        /* 2. Get packet buffer for message/data */
        csp_packet_t * packet = static_cast<csp_packet_t*>(csp_buffer_get(100));
        if (packet == NULL) {
            /* Could not get buffer element */
            csp_log_error("Failed to get CSP buffer");
            return KS_ERROR;
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

        return KS_SUCCESS;
    }
}