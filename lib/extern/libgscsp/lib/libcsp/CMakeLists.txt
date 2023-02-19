set(LIBCSP_SRC
        "src/csp_bridge.c"
        "src/csp_buffer.c"
        "src/csp_conn.c"
        "src/csp_crc32.c"
        "src/csp_debug.c"
        "src/csp_dedup.c"
        "src/csp_endian.c"
        "src/csp_hex_dump.c"
        "src/csp_iflist.c"
        "src/csp_init.c"
        "src/csp_io.c"
        "src/csp_port.c"
        "src/csp_promisc.c"
        "src/csp_qfifo.c"
        "src/csp_route.c"
        "src/csp_service_handler.c"
        "src/csp_services.c"
        "src/csp_sfp.c"

        "src/crypto/csp_hmac.c"
        "src/crypto/csp_sha1.c"
        "src/crypto/csp_xtea.c"

        "src/drivers/usart/usart_sam.c"

        "src/external/csp_debug.c"

        "src/interfaces/csp_if_can.c"
        "src/interfaces/csp_if_can_pbuf.c"
        "src/interfaces/csp_if_i2c.c"
        "src/interfaces/csp_if_kiss.c"
        "src/interfaces/csp_if_lo.c"
        "src/interfaces/csp_if_zmqhub.c"

        "src/rtable/csp_rtable.c"
        "src/rtable/csp_rtable_cidr.c"
        "src/rtable/csp_rtable_static.c"

        "src/transport/csp_rdp.c"
        "src/transport/csp_udp.c"
        )

add_mcu_library("libcsp" ${LIBCSP_SRC})

target_link_libraries("libcsp" PUBLIC asf)
target_include_directories("libcsp" PUBLIC include)