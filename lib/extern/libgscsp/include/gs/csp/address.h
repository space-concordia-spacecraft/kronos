#ifndef LIBGSCSP_INCLUDE_GS_CSP_ADDRESS_H
#define LIBGSCSP_INCLUDE_GS_CSP_ADDRESS_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Default CSP addresses for nodes in the satellite - often changed for each project.
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
   Default address for OBC (On Board Computer).
   Example: a3200-sdk.
*/
#define GS_CSP_ADDR_OBC        1
/**
   Power supply.
   Example: nano-power.
*/
#define GS_CSP_ADDR_EPS        2
/**
   Payload address.
*/
#define GS_CSP_ADDR_PAYLOAD_3  3
/**
   Default address for ADCS.
   Example: a3200-adcs.
*/
#define GS_CSP_ADDR_ADCS       4
/**
   Default address for radio.
   Example: nanocom-ax.
*/
#define GS_CSP_ADDR_NANOCOM    5
/**
   Payload address.
*/
#define GS_CSP_ADDR_PAYLOAD_6  6
/**
   Battery pack.
   Example: nano-power-bpx.
*/
#define GS_CSP_ADDR_BPX        7
/**
   Payload address.
*/
#define GS_CSP_ADDR_PAYLOAD_8  8
/**
   Reaction wheels.
   Example: gsw-600.
*/
#define GS_CSP_ADDR_GSW600     9
/**
   Antenna module.
   Example: ant2150.
*/
#define GS_CSP_ADDR_ANT2150   10

#ifdef __cplusplus
}
#endif
#endif
