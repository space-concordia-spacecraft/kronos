#ifndef GS_UTIL_DRIVERS_HW_WATCHDOG_H
#define GS_UTIL_DRIVERS_HW_WATCHDOG_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Hardward watchdog (HWWD) device interface.

   Hardware Watchdog interface which provides a generic interface towards
   any HWWD. Most HWWD implementation should be able to fit behind
   this interface, with just a small "adaption" layer needed.
*/

#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Hardware watchdog driver interface.
*/
typedef struct gs_watchdog_dev_ops gs_watchdog_dev_ops_t;

/**
   Hardware watchdog (HWWD) device structure

   Structure that describes the HWWD device and holds
   the parameters needed for storing e.g. timeout values etc.
*/
typedef struct gs_watchdog_device {
    int id; /**< An ID for the HWWD device - This is currently not used. */
    const gs_watchdog_dev_ops_t *ops; /**< Pointer to ops struct defining the operations a HWWD device supports. */
    unsigned int timeout; /**< The timeout value that the HWWD device should be configured with. */
    unsigned int pretimeout; /**< The pretimeout (if supported) by the HWWD  device */
    unsigned int min_timeout; /**< Minimum timeout value supported by the HWWD device */
    unsigned int max_timeout; /**< Maximum timeout value supported by the HWWD device */
    void *driver_data; /**< Pointer to driver specific data can be used by the HWWD driver impl. */
} gs_watchdog_device_t;

/**
   Hardware watchdog driver interface.
*/
struct gs_watchdog_dev_ops
{
    /* mandatory operations */
    gs_error_t (*start)(gs_watchdog_device_t *); /**< Starts the HWWD device */
    gs_error_t (*stop)(gs_watchdog_device_t *); /**< Stops the HWWD device */
    gs_error_t (*ping)(gs_watchdog_device_t *); /**< Polls the HWWD device and restart count-down */
    /* optional operations */
    gs_error_t (*set_timeout)(gs_watchdog_device_t *, unsigned int); /**< (Optional) Set timeout of the HWWD device */
    gs_error_t (*set_pretimeout)(gs_watchdog_device_t *, unsigned int); /**< (Optional) Set Pre-timeout of the HWWD device */
    gs_error_t (*restart)(gs_watchdog_device_t *); /**< (Optional) Restart the HWWD device */
    unsigned int (*get_timeleft)(gs_watchdog_device_t *); /**< (Optional) Get time left until HWWD device times out. */
    int (*status)(gs_watchdog_device_t *); /**< (Optional) Reads status of the HWWD device */
};

#ifdef __cplusplus
}
#endif
#endif
