#ifndef LIBGSCSP_INCLUDE_GS_CSP_CSP_H
#define LIBGSCSP_INCLUDE_GS_CSP_CSP_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   Extensions to standard libcsp.
*/

#include <gs/csp/error.h>
#include <gs/csp/port.h>
#include <gs/csp/rtable.h>
#include <csp/csp.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Default CSP timeout (mS).

   Default timeout value for communicating with a satellite, based on round-trip time of approximately 500 mS.
*/
#define GS_CSP_TIMEOUT 3000

/**
   Default maximum payload size per CSP packet.

   Any application/service sending CSP packets with maximum payload, should base their maximum payload size on this define. Using this define as
   reference, will allow the maximum size to be adjusted by only changing this define.
   The define must never be used as a fixed size between end-points, as it may be changed in future releases.

   The maximum CSP packet size that can be sent between two end-points, depends on what options are enabled in the node (client) and any relay
   points on the way, e.g. the radio (AX100).

   Ax100: The total CSP packet size, which an AX100 can transmit, is 251 bytes (excluding the mandatory CSP header). The AX100 supports a number
   of options, which reduces the total size:

   - Reed-Solomon error correction: 32 bytes
   - CRC32: 4 bytes
   - HMAC:  4 bytes

   Client: When the client opens a CSP connection, it is possible to enable a number of options. Some of these options may add additional bytes to
   the packet, hence reducing the maximum payload size:

   - CRC32: 4 bytes
   - HMAC: 4 bytes (this is ignored, since HMAC in most cases are either enabled in radio or client - not both).
   - XTEA: 4 bytes
   - RDP:   5 bytes

   The maximum payload size is therefor calculated as:
   max = (251 - 32 - 4 - 4) - (4 - 4 - 5) = 198 bytes
*/
#ifndef GS_CSP_DEFAULT_MAX_PAYLOAD_SIZE
#define GS_CSP_DEFAULT_MAX_PAYLOAD_SIZE 198
#endif

/**
   Check if address is a valid CSP adddress.
   @param[in] address CSP address to verify
   @return \a true if address is valid.
*/
bool gs_csp_is_address_valid(uint8_t address);

/**
   gscsp and csp configuration.
*/
typedef struct {
    /**
       Forward CSP logs to GomSpace log (libutil).
       @deprecated version 3.0, ignored - as libcsp is compiled with GomSpace log macros (defining CSP_USE_EXTERNAL_DEBUG).
    */
    bool use_gs_log;

    /**
       Use command line options.
       Configure interfaces specified on the command line.
       @note Only supported on Linux - ignored on platforms without command line options.
    */
    bool use_command_line_options;

    /**
       Size of a CSP buffer's data part (in bytes).
       Corresponds to \a data_size in the csp_buffer_init().
    */
    size_t csp_buffer_size;
    /**
       Number of CSP buffers to allocate.
       Corresponds to \a buf_count in csp_buffer_init().
    */
    size_t csp_buffers;

    /**
       CSP address of the system
    */
    uint8_t address;
    /**
       Host name, returned by the #CSP_CMP_IDENT request.
       @note String must remain valid as long as the application is running.
    */
    const char *hostname;
    /**
       Model, returned by the #CSP_CMP_IDENT request.
       @note String must remain valid as long as the application is running.
    */
    const char *model;
    /**
       Revision, returned by the #CSP_CMP_IDENT request
       @note String must remain valid as long as the application is running.
    */
    const char *revision;
    /**
       Max number of connections.
       A fixed connection array is allocated by csp_init()
       Maps to \a conn_max in #csp_conf_t.
    */
    uint8_t max_connections;
    /**
       Rx queue length (max queued messages).
       Will be adjusted, if bigger than \a csp_buffers
       Maps to \a conn_queue_length in #csp_conf_t.
    */
    uint8_t rx_queue_length;
    /**
       Default (minimum) connection options.
       Options will always be or'ed onto new connections, see csp_connect().
       Maps to \a conn_dfl_so in #csp_conf_t.
    */
    uint32_t default_connection_options;
    /**
       Length of routers incoming message queue (max queue messages).
       Will be adjusted, if bigger than \a csp_buffers
       Maps to \a fifo_length in #csp_conf_t.
    */
    uint8_t router_queue_length;
    /**
       Max/highest port for use with csp_bind()
       Maps to \a port_max_bind in #csp_conf_t.
    */
    uint8_t max_bind_port;
    /**
       Max RDP window size.
       Maps to \a rdp_max_window in #csp_conf_t.
    */
    uint8_t max_rdp_window_size;
} gs_csp_conf_t;

/**
   Get default CSP configuration for server systems.
   @param[in] conf user supplied configuration struct.
*/
void gs_csp_conf_get_defaults_server(gs_csp_conf_t * conf);

/**
   Get default CSP configuration for embedded systems.
   @param[in] conf user supplied configuration struct.
*/
void gs_csp_conf_get_defaults_embedded(gs_csp_conf_t * conf);

/**
   Initialize CSP.

   Wraps initialization of libcsp, by calling following functions
   - hooks CSP log system into GomSpace log system.
   - initializes CSP buffers by calling csp_buffer_init()
   - initializes CSP by calling csp_init()
   - configure interfaces specificed on command line
   - configure routing table specificed on command line, or default if only one interface is present.

   @param[in] conf configuration.
   @return_gs_error_t
*/
gs_error_t gs_csp_init(const gs_csp_conf_t * conf);

/**
   Free allocated resorces in GSCSP and CSP (for testing).
   This is only to be called from automatic tests, to satisfy tools like valgrind.
*/
void gs_csp_free_resources(void);

/**
   Perform an entire request/reply transaction,

   Copies both input buffer and reply to output buffer.
   Also makes the connection and closes it again
   Differ from 'csp_transaction()' by limiting input buffer size when input length is unknown

   @param[in] prio CSP Prio
   @param[in] dest CSP Dest
   @param[in] port CSP Port
   @param[in] timeout timeout in ms
   @param[in] tx_buf pointer to outgoing data buffer
   @param[in] tx_len length of request to send
   @param[out] rx_buf pointer to incoming data buffer
   @param[in] rx_max_len length of expected reply (input buffer size)
   @param[out] rx_len pointer to length of reply
   @param[in] opts connection options.
   @return GS_ERROR_OVERFLOW when input length larger than buffer - it still updates rx_buf
   @return GS_ERROR_IO when 'csp_send()' or 'csp_read()' fails
   @return GS_ERROR_ALLOC when 'csp_get_buffer()' fails
   @return_gs_error_t
 */
gs_error_t gs_csp_transaction2(uint8_t prio,
                               uint8_t dest,
                               uint8_t port,
                               uint32_t timeout,
                               const void * tx_buf,
                               size_t tx_len,
                               void * rx_buf,
                               size_t rx_max_len,
                               size_t * rx_len,
                               uint32_t opts);

/**
   Perform an entire request/reply transaction,

   @param[in] prio CSP Prio
   @param[in] dest CSP Dest
   @param[in] port CSP Port
   @param[in] timeout timeout in ms
   @param[in] tx_buf pointer to outgoing data buffer
   @param[in] tx_len length of request to send
   @param[out] rx_buf pointer to incoming data buffer
   @param[in] rx_max_len length of expected reply (input buffer size)
   @param[out] rx_len pointer to length of reply
   @return GS_ERROR_OVERFLOW when input length larger than buffer - it still updates rx_buf
   @return GS_ERROR_IO when 'csp_send()' or 'csp_read()' fails
   @return GS_ERROR_ALLOC when 'csp_get_buffer()' fails
   @return_gs_error_t
   @see gs_csp_transaction2()
*/
static inline gs_error_t gs_csp_transaction(uint8_t prio,
                                            uint8_t dest,
                                            uint8_t port,
                                            uint32_t timeout,
                                            const void * tx_buf,
                                            size_t tx_len,
                                            void * rx_buf,
                                            size_t rx_max_len,
                                            size_t * rx_len)
{
    return gs_csp_transaction2(prio, dest, port, timeout, tx_buf, tx_len, rx_buf, rx_max_len, rx_len, 0);
}

/**
   Use an existing connection to perform a transaction.

   This is only possible if the next packet is on the same port and destination!
   Differ from 'csp_transaction_persistent()' by limiting input buffer size when input length is unknown

   @param[in] conn pointer to connection structure
   @param[in] timeout timeout in ms
   @param[in] tx_buf pointer to outgoing data buffer
   @param[in] tx_len length of request to send
   @param[out] rx_buf pointer to incoming data buffer
   @param[in] rx_max_len length of expected reply (input buffer size)
   @param[out] rx_len pointer to length of reply
   @return GS_ERROR_OVERFLOW when input length larger than buffer - it still updates rx_buf
   @return GS_ERROR_IO when 'csp_send()' or 'csp_read()' fails
   @return GS_ERROR_ALLOC when 'csp_get_buffer()' fails
   @return_gs_error_t
 */
gs_error_t gs_csp_transaction_persistent(csp_conn_t * conn,
                                         uint32_t timeout,
                                         const void * tx_buf,
                                         size_t tx_len,
                                         void * rx_buf,
                                         size_t rx_max_len,
                                         size_t * rx_len);

/**
   Get host name.
   @note Legacy CSP function.
   @return host name.
*/
const char *csp_get_hostname(void);
    
/**
   Get model name.
   @note Legacy CSP function.
   @return model name
*/
const char *csp_get_model(void);

/**
   Get revision.
   @note Legacy CSP function.
   @return revision.
*/
const char *csp_get_revision(void);

/**
   Perform an entire request/reply transaction
   Copies both input buffer and reply to output buffeer.
   Also makes the connection and closes it again

   @deprecated Use csp_transaction_w_opts()
   @param prio CSP Prio
   @param dest CSP Dest
   @param port CSP Port
   @param timeout timeout in ms
   @param outbuf pointer to outgoing data buffer
   @param outlen length of request to send
   @param inbuf pointer to incoming data buffer
   @param inlen length of expected reply, -1 for unknown size (note inbuf MUST be large enough)
   @param opts Connection options.
   @return Return 1 or reply size if successful, 0 if error or incoming length does not match or -1 if timeout was reached
*/
static inline int csp_transaction2(uint8_t prio, uint8_t dest, uint8_t port, uint32_t timeout, void *outbuf, int outlen, void *inbuf, int inlen, uint32_t opts) {
    return csp_transaction_w_opts(prio, dest, port, timeout, outbuf, outlen, inbuf, inlen, opts);
}
    
#ifdef __cplusplus
}
#endif
#endif
