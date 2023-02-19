#ifndef LIB_LIBUTIL_INCLUDE_GS_UTIL_LINUX_DRIVERS_GPIO_GPIO_SYSFS_H_
#define LIB_LIBUTIL_INCLUDE_GS_UTIL_LINUX_DRIVERS_GPIO_GPIO_SYSFS_H_
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   @brief Linux GPIO driver based on sysfs.
          This driver needs to be registered in the generic GPIO linux driver @see 'gs/util/linux/drivers/gpio/gpio.h'
*/

#include <gs/util/linux/drivers/gpio/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   GPIO sysfs driver data.

   @note Driver takes no driver data, so a NULL pointer is valid
*/
typedef void * gs_gpio_sysfs_driver_data_t;

/**
   GPIO sysfs driver interface.
*/
extern const gs_gpio_driver_t gs_gpio_sysfs_driver;

/**
   GPIO sysfs initialize

   @param[in] gpio The gpio to initialize
   @param[in] output Direction of pin (True/False = Output/Input)
   @param[in] init_value Pin state if configured as output (True/False = High/Low)
   @param[in] active_low if set pin is configured as active low (so a gs_gpio_sysfs_set with 1 will actually set value low)
   @return_gs_error_t
 */
gs_error_t gs_gpio_sysfs_initialize(gs_gpio_t gpio, bool output, bool init_value, bool active_low);

/**
    GPIO sysfs get value

    @param[in] gpio The gpio to read
    @param[in] value Returned GPIO value (true/false = High/Low)
    @param[in] driver_data data to driver (not used)
    @return_gs_error_t
*/
gs_error_t gs_gpio_sysfs_get(gs_gpio_t gpio, bool *value, void * driver_data);

/**
    GPIO sysfs get value without error check

    @param[in] gpio The gpio to read
    @param[in] driver_data data to driver (not used)
    @return GPIO value (true/false = High/Low)
*/
bool gs_gpio_sysfs_get_nc(gs_gpio_t gpio, void * driver_data);

/**
    GPIO sysfs set value

    @param[in] gpio The gpio to set
    @param[in] value GPIO value (true/false = High/Low)
    @param[in] driver_data data to driver (not used)
    @return_gs_error_t
*/
gs_error_t gs_gpio_sysfs_set(gs_gpio_t gpio, bool value, void * driver_data);

/**
    GPIO sysfs set value without error check

    @param[in] gpio The gpio to set
    @param[in] value GPIO value (true/false = High/Low)
    @param[in] driver_data data to driver (not used)
*/
void gs_gpio_sysfs_set_nc(gs_gpio_t gpio, bool value, void * driver_data);

/**
    Initialize GPIO sysfs as an external interrupt pin

    @param[in] gpio The gpio to configure
    @param[in] conf Configuration of interrupt pin
    @param[in] driver_data data to driver (not used)
    @return_gs_error_t
 */
gs_error_t gs_gpio_sysfs_init_as_interrupt(gs_gpio_t gpio, const gs_interrupt_conf_t * conf, void * driver_data);

#ifdef __cplusplus
}
#endif
#endif
