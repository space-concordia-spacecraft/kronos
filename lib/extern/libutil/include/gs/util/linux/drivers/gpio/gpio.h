#ifndef GS_UTIL_LINUX_DRIVERS_GPIO_H
#define GS_UTIL_LINUX_DRIVERS_GPIO_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
    @file

    @brief GPIO interface

    GPIO interface provides a generic interface where specific GPIO drivers can be plugged in.
*/

#include <gs/util/error.h>
#include <gs/util/drivers/gpio/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
    GomSpace linux driver GPIO get value

    @param[in] gpio The gpio to read
    @param[in] value Returned GPIO value (true/false = High/Low)
    @param[in] driver_data data to specific driver

    @return_gs_error_t
*/
typedef gs_error_t (*gs_gpio_get_t)(gs_gpio_t gpio, bool *value, void * driver_data);

/**
    GomSpace linux driver GPIO get value without error check

    @param[in] gpio The gpio to read
    @param[in] driver_data data to specific driver

    @return GPIO value (true/false = High/Low)
*/
typedef bool (*gs_gpio_get_nc_t)(gs_gpio_t gpio, void * driver_data);

/**
    GomSpace linux driver GPIO set value

    @param[in] gpio The gpio to set
    @param[in] value GPIO value (true/false = High/Low)
    @param[in] driver_data data to specific driver

    @return_gs_error_t
*/
typedef gs_error_t (*gs_gpio_set_t)(gs_gpio_t gpio, bool value, void * driver_data);

/**
    GomSpace linux driver GPIO set value without error check

    @param[in] gpio The gpio to set
    @param[in] value GPIO value (true/false = High/Low)
    @param[in] driver_data data to specific driver
*/
typedef void (*gs_gpio_set_nc_t)(gs_gpio_t gpio, bool value, void * driver_data);

/**
    GomSpace linux driver initialize GPIO as an external interrupt pin

    @param[in] gpio The gpio to configure
    @param[in] conf Configuration of interrupt pin
    @param[in] driver_data data to specific driver

    @return_gs_error_t
 */
typedef gs_error_t (*gs_gpio_init_as_interrupt_t)(gs_gpio_t gpio, const gs_interrupt_conf_t * conf, void * driver_data);


/**
   Every port.
 */
#define GS_GPIO_ALL_PORTS UINT16_MAX

/**
   Every pin.
 */
#define GS_GPIO_ALL_PINS UINT16_MAX

/**
   GPIO driver.
 */
typedef struct {
    /**
       Function for handling GPIO get.
     */
    gs_gpio_get_t get_handler;
    /**
       Function for handling GPIO get no check.
     */
    gs_gpio_get_nc_t get_nc_handler;
    /**
       Function for handling GPIO set.
     */
    gs_gpio_set_t set_handler;
    /**
       Function for handling GPIO set no check.
     */
    gs_gpio_set_nc_t set_nc_handler;
    /**
       Function for handling GPIO initialize as interrupt.
     */
    gs_gpio_init_as_interrupt_t init_as_interrupt_handler;
} gs_gpio_driver_t;


/**
   GPIO driver entry
 */
typedef struct {
    /**
       GPIO port, to which the driver is used (if GS_GPIO_ALL_PORTS, then all ports uses this driver).
     */
    uint16_t port;
    /**
       GPIO pin, to which the driver is used (if GS_GPIO_ALL_PINS, then all pins uses this driver).
     */
    uint16_t pin;
    /**
       GPIO driver.
     */
    const gs_gpio_driver_t * driver;
    /**
       Driver specific data.
     */
    void * driver_data;
} gs_gpio_driver_entry_t;

/**
   Register a driver.

   A specific driver can be assigned to a port and pin or it can be assigned to all pins and/or all ports.

   The latest registered driver, which fit the GPIO, is the one used.

   @param[in] driver_entry driver and configuration to be registered
   @return_gs_error_t
 */
gs_error_t gs_gpio_register_driver(const gs_gpio_driver_entry_t * driver_entry);

#ifdef __cplusplus
}
#endif
#endif
