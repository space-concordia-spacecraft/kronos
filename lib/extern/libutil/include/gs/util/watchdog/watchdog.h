#ifndef GS_UTIL_WATCHDOG_WATCHDOG_H
#define GS_UTIL_WATCHDOG_WATCHDOG_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Software watchdog client interface.

   The software watchdog (SWWD) enables having multiple instances of a Watchdog.
   The software watchdog manages the HW watchdog, and will ultimately
   trigger the HW watchdog, if one or more clients are not servicing the
   software watchdog.
*/

#include <gs/util/drivers/watchdog/device.h>
#include <gs/util/log.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Software Watchdog handle
*/
typedef struct gs_swwd_hdl gs_swwd_hdl_t;

/**
   Software watchdog callback function.

   Called by the SWWD upon timeout.
   @param[in] userdata user data provided on gs_swwd_register()
*/
typedef void (*gs_swwd_callback_function_t)(void * userdata);

/**
   Watchdog timeout action.
*/
typedef enum {
    /**
       Reset system on timeout (stops touching the hardware watchdog).
       Once the watchdog has timeout, the watchdog cannot be re-activated.
    */
    GS_SWWD_TIMEOUT_ACTION_RESET = 0,
    /**
       Log 'warning' on timeout, but otherwise ignore the timeout.
       The watchdog can re-activated by touching the watchdog again.
    */
    GS_SWWD_TIMEOUT_ACTION_LOG = 1,
} gs_swwd_timeout_action_t;

/**
   Create the software watchdog back-end.

   Only one SWWD back-end can exist at any given time.

   @param[in] max_clients The maximum number of Software Watchog clients supported.
   @param[in] dev The HW Watchdog device to use.
   @return_gs_error_t
*/
gs_error_t gs_swwd_create(uint32_t max_clients, gs_watchdog_device_t *dev);

/**
   Destroy the Software Watchdog back-end (and stop the SWWD monitor task if started).

   @param[in] timeout_s Maximum number of seconds to allow this operation to complete.
   @return_gs_error_t
*/
gs_error_t gs_swwd_destroy(uint32_t timeout_s);

/**
   Check for expired software watchdog clients. This function is only to be used if the
   SWWD monitor task is not started. Otherwise the SWWD task will handle this in the back-
   ground. I.e:
   - In passive mode this function must be called periodically to check for expired
   clients, and service the HW watchdog.
   - In active mode this function is called in background by the SWWD monitor task.

   The interval between these checks should be much less that the HW watchdog
   timeout period, to ensure that the HW Watchdog is correctly serviced.
   Calling this e.g. every 1-3 seconds will be a good default.

   @param[out] num_expired The number of SW Watchog clients currently expired.
   @return_gs_error_t
*/
gs_error_t gs_swwd_check_expired_clients(uint32_t *num_expired);

/**
   Register/create a new software watchdog instance

   @param[out] wdt_handle A reference to software watchdog handle
   @param[in] timeout Timeout in seconds.
   @param[in] callback Callback function which is called on timeout. NULL if unused.
   @param[in] userdata Pointer to user data used in the callback function. Ignored if callback is NULL.
   @param[in] client_name A descriptive name given by the user in order to identify the watchdog/client - the pointer must remain valid as long as the watchdog is registered.
   @param[in] action what action to take, when/if the watchdog times out.
   @return_gs_error_t
*/
gs_error_t gs_swwd_register_with_action(gs_swwd_hdl_t ** wdt_handle, uint32_t timeout, gs_swwd_callback_function_t callback, void * userdata, const char *client_name, gs_swwd_timeout_action_t action);

/**
   Register/create a software watchdog with action \a reset on timeout.

   @param[out] wdt_handle A reference to software watchdog handle
   @param[in] timeout Timeout in seconds before the software watchdog fires.
   @param[in] callback Callback function which is called on timeout. NULL if unused.
   @param[in] userdata Pointer to user data used in the callback function. Ignored if callback is NULL.
   @param[in] client_name A descriptive name given by the user in order to identify the watchdog/client - the pointer must remain valid as long as the watchdog is registered.
   @return_gs_error_t
*/
static inline gs_error_t gs_swwd_register(gs_swwd_hdl_t ** wdt_handle, uint32_t timeout, gs_swwd_callback_function_t callback, void * userdata, const char *client_name)
{
    return gs_swwd_register_with_action(wdt_handle, timeout, callback, userdata, client_name, GS_SWWD_TIMEOUT_ACTION_RESET);
}
    
/**
   De-Register a Software Watchdog instance

   @param[in] wdt_handle A software watchdog handle
   @return_gs_error_t
*/
gs_error_t gs_swwd_deregister(gs_swwd_hdl_t ** wdt_handle);

/**
   Touch Software Watchdog to reset the timer

   @param[in] wdt_handle A software watchdog handle
   @return_gs_error_t
*/
gs_error_t gs_swwd_touch(gs_swwd_hdl_t * wdt_handle);

/**
   Set timeout of the Software Watchdog.

   @param[in] wdt_handle A software watchdog handle
   @param[in] timeout Timeout in seconds before the SWWD fires.
   @return_gs_error_t
*/
gs_error_t gs_swwd_set_timeout(gs_swwd_hdl_t * wdt_handle, uint32_t timeout);

#ifdef __cplusplus
}
#endif
#endif
