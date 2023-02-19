#ifndef LIBGSCSP_INCLUDE_GS_CSP_PORT_H
#define LIBGSCSP_INCLUDE_GS_CSP_PORT_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Port definitions for standard CSP and GomSpace services.
*/

#include <csp/csp_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Port definitions for standard CSP and GomSpace services.
*/
typedef enum {
    /**
       CSP Management Protocol - standard CSP service.
    */
    GS_CSP_CMP = CSP_CMP,           // 0
    /**
       Ping - standard CSP service.
    */
    GS_CSP_PING = CSP_PING,         // 1
    /**
       Show process status - standard CSP service.
    */
    GS_CSP_PS = CSP_PS,             // 2
    /**
       Show memory free - standard CSP service.
    */
    GS_CSP_MEM_FREE = CSP_MEMFREE,   // 3
    GS_CSP_MEMFREE = GS_CSP_MEM_FREE,
    /**
       Reboot/reset request - standard CSP service.
    */
    GS_CSP_REBOOT = CSP_REBOOT,     // 4
    /**
       Show number of free CSP buffers - standard CSP service.
    */
    GS_CSP_BUF_FREE = CSP_BUF_FREE, // 5
    /**
       Show uptime (time since last reset) - standard CSP service.
    */
    GS_CSP_UPTIME = CSP_UPTIME,     // 6
    /**
       Parameter service (libparam)
    */
    GS_CSP_PORT_RPARAM =               7,
    /**
       File Transfer Service (libftp)
    */
    GS_CSP_PORT_FTP =                  9,
    /**
       Remote log service (liblog)
    */
    GS_CSP_PORT_RLOG =                11,
    /**
       Remote GOSH service (librgosh)
    */
    GS_CSP_PORT_RGOSH =               12,
    /**
       AIS command port (libais).
    */
    GS_CSP_PORT_AIS =                 13,
    /**
       ADS-B command port (libadsb).
    */
    GS_CSP_PORT_ADSB =                14,

    /**
       GomSpace Sensor Bus (libgssb).
    */
    GS_CSP_PORT_GSSB =                16,
    /**
       Flight Planner (libfp).
    */
    GS_CSP_PORT_FP =                  18,
    /**
       ADCS (libadcs).
    */
    GS_CSP_PORT_ADCS =                20,
    /**
       House Keeping (libhk).
    */
    GS_CSP_PORT_HK =                  21,
    /**
       G(omSpace) script service (libgosh)
    */
    GS_CSP_PORT_GSCRIPT =             22,
    /**
       Gomspace Monitor application command port
    */
    GS_CSP_PORT_MONITOR =             26,
    /**
       Remote shell (libgosh).
       Executes shell commands (linux server only).
       Requires CSP_O_RDP.
    */
    GS_CSP_PORT_REMOTE_SHELL =        27,
    /**
       House keeping beacon port (libhk).
       Default port for sending beacons from satellite to ground (configurable).
    */
    GS_CSP_PORT_HK_BEACON =           30,

} gs_csp_port_t;

#ifdef __cplusplus
}
#endif
#endif
