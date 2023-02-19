Gomspace CSP extension (libgscsp)
=================================

**3.0.1 (2020-05-15)**

- Breaking: Updated to libcsp v1.6 - see libcsp CHANGELOG for details. APIs are primarily broken in regards to csp_init() and interfaces impl.

**2.7.4 (2020-03-18)**

- Improvement: Reduced GS_CSP_DEFAULT_MAX_PAYLOAD_SIZE to make room for the RDP header.

**2.7.3 (2019-11-13)**

- Improvement: Updated libcsp: Always install all header files.

**2.7.2 (2019-10-16)**

- Feature: Added default value for max CSP packet payload, GS_CSP_DEFAULT_MAX_PAYLOAD_SIZE.
- Feature: Added 'count' option on ping command, pings destination 'count' times and shows average ping time.

**2.7.1 (2019-07-03)**

- Feature: Added support for ping without CRC32.
- Improvement: Changed formatting of 'uptime' command.
- Improvement: Changed CAN logging to match CAN logging from libembed.
- Improvement: Updated libcsp: CAN driver: get time correctly depending on context - ISR or task.

**2.6.2 (2019-05-16)**

- Improvement: Include CAN support in .deb package.

**2.6.1 (2019-04-08)**

- Improvement: Support longer device names on command line.

**2.5.1 (2019-03-10)**

- Bug: Made CAN driver use the chosen CAN device in both initialization and TX and RX.
- Feature: Added support for multiple csp_zmq interfaces in process.
- Bug: libcsp, fixed bug in linux usart driver, didn't detect read error as expected.
- Breaking: libcsp, removed static "csp_if_zmqhub" interface.
- Improvement: added error handling in usart driver (linux), fail if device cannot be opened.

**2.4.3 (2019-01-29)**

- Improvement: Register all used log groups.
- Improvement: libcsp: Support changing MTU on CAN interface.
- Bug: libcsp: Fixed race condition when allocating dynamic port (client connection).

**2.4.2 (2018-12-19)**

- Improvement: Added defines for GS_CSP_PORT_AIS(13) and GS_CSP_PORT_ADSB(14) to prevent future clashing
- Improvement: Updated documentation.

**2.4.1 (2018-11-21)**

- Improvement: Added gs_csp_init(), gs_csp_rtable_load() and command line support.

**2.3.2 (2018-09-19)**

- Improvement: Remove a few logs from ISR context, could cause crashes.
- Improvement: RDP - limit pending messages to window size (not windows size +1).
- Bug: Fixed possible RDP csp_send() deadlock issue (LIBGSCSP-6).
- Breaking: Updated gs_service_dispatcher and gs_router APIs to improve automatic testing.
- Feature: updated libcsp to public version 1.5 (release-1.5 branch).

**2.3.1 (2018-08-13)**

- Feature: Added gs_csp_service_dispatcher().
- Feature: Enable CRC32 on all connections as default.

**2.2.2 (2018-05-30)**

- Breaking: Updated internal use of reset API from libembed, which now takes a reset cause.
- Feature: Added support for CSP logging through libutil::log.

**2.1.2 (2018-04-17)**

- Breaking: Changed name of dynamic library: libgsgscsp -> libgscsp

**2.0.0 (2018-03-07)**

.. toctree::
   CSP_CHANGELOG
