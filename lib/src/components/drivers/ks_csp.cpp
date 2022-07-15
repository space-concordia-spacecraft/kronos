#include "ks_csp.h"

namespace kronos {
    ComponentCspDriver::ComponentCspDriver(const std::string& name, uint32_t baudrate, uint32_t charLength,
                                           uint32_t parityType, uint32_t stopBits) : ComponentPassive(name) {

    }

    KsResult ComponentCspDriver::Init() {
        uint8_t address = 1;
        csp_debug_level_t debug_level = CSP_INFO;

        /* enable/disable debug levels */
        for (int i = 0; i <= CSP_LOCK; ++i) {
            csp_debug_set_level(static_cast<csp_debug_level_t>(i), i <= debug_level);
        }

        Framework::LogInfo("Initialising CSP");

        /* Init CSP with address and default settings */
        csp_conf_t csp_conf;
        csp_conf_get_defaults(&csp_conf);
        csp_conf.address = address;
        int error = csp_init(&csp_conf);
        if (error != CSP_ERR_NONE) {
            Framework::LogInfo("csp_init() failed, error:");
            exit(1);
        }

        /* Start router task with 10000 bytes of stack (priority is only supported on FreeRTOS) */
        csp_route_start_task(500, 0);

        m_ServerAddress = 1;

//        m_Socket = csp_socket(CSP_SO_NONE);
//        csp_bind(m_Socket, CSP_ANY);
//        csp_listen(m_Socket, 10);

        return KS_SUCCESS;
    }

    KsCmdResult ComponentCspDriver::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_WRITE:
                Read();
                break;
            case KS_EVENT_CODE_RATE_GROUP_TICK:
                Write();
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

    KsResult ComponentCspDriver::Read() {
        /* Wait for a new connection, 10000 mS timeout */

        csp_conn_t* conn;
        if ((conn = csp_accept(m_Socket, 1000)) == NULL) {
            /* timeout */
            // TODO: in the example this continues the loop. Meaning it does nothing.
            Framework::LogInfo("Not Connected");
            return KS_SUCCESS;
        }

        /* Read packets on connection, timout is 100 mS */
        csp_packet_t* packet;
        while ((packet = csp_read(conn, 50)) != NULL) {
            switch (csp_conn_dport(conn)) {
                case MY_SERVER_PORT:
                    /* Process packet here */
                    Framework::LogInfo((char*) packet->data);
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

        return KS_SUCCESS;
    }

    KsResult ComponentCspDriver::Write() {
        /* Send ping to server, timeout 1000 mS, ping size 100 bytes */
        Framework::LogInfo("Ping address: %u, result %d [mS]");

        /* Send reboot request to server, the server has no actual implementation of csp_sys_reboot() and fails to reboot */
        csp_reboot(m_ServerAddress);
        Framework::LogInfo("reboot system request sent to address: %u");

        /* Send data packet (string) to server */

        /* 1. Connect to host on 'server_address', port MY_SERVER_PORT with regular UDP-like protocol and 1000 ms timeout */
        csp_conn_t* conn = csp_connect(CSP_PRIO_NORM, m_ServerAddress, MY_SERVER_PORT, 1000, CSP_O_NONE);
        if (conn == NULL) {
            /* Connect failed */
            Framework::LogInfo("Failed to connect");
            return KS_ERROR;
        }

        /* 2. Get packet buffer for message/data */
        csp_packet_t* packet = static_cast<csp_packet_t*>(csp_buffer_get(100));
        if (packet == NULL) {
            /* Could not get buffer element */
            Framework::LogInfo("Failed to get CSP buffer");
            return KS_ERROR;
        }

        /* 3. Copy data to packet */
        snprintf((char*) packet->data, csp_buffer_data_size(), "Hello World (%u)", ++m_Count);

        /* 4. Set packet length */
        packet->length = (strlen((char*) packet->data) + 1); /* include the 0 termination */

        /* 5. Send packet */
        if (!csp_send(conn, packet, 1000)) {
            /* Send failed */
            Framework::LogInfo("Send failed");
            csp_buffer_free(packet);
        }

        /* 6. Close connection */
        csp_close(conn);

        return KS_SUCCESS;
    }
}