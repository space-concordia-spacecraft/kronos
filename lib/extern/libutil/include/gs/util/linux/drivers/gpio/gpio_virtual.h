#ifndef LIB_LIBUTIL_INCLUDE_GS_UTIL_LINUX_DRIVERS_GPIO_GPIO_VIRTUAL_H_
#define LIB_LIBUTIL_INCLUDE_GS_UTIL_LINUX_DRIVERS_GPIO_GPIO_VIRTUAL_H_
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
   @file

   @brief Linux GPIO driver to be used in unit tests.
          This driver needs to be registered in the generic GPIO linux driver @see 'gs/util/linux/drivers/gpio/gpio.h'
*/

#include <gs/util/linux/drivers/gpio/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   GPIO virtual driver data.

   @note Driver takes no driver data, so a NULL pointer is valid
*/
typedef void * gs_gpio_virtual_driver_data_t;

/**
   GPIO virtual driver interface.
*/
extern const gs_gpio_driver_t gs_gpio_virtual_driver;

/**
   GPIO virtual driver entry, where all ports and pins are routed to virtual driver
 */
extern const gs_gpio_driver_entry_t gs_gpio_virtual_driver_entry_all;

/**
   GPIO virtual initialize

   @param[in] gpio The gpio to initialize
   @param[in] output Direction of pin (True/False = Output/Input)
   @param[in] value Pin state if configured as output (True/False = High/Low)
   @return_gs_error_t
 */
gs_error_t gs_gpio_virtual_initialize(gs_gpio_t gpio, bool output, bool value);

/**
   GPIO virtual get value

   @param[in] gpio The gpio to read
   @param[in] value Returned GPIO value (true/false = High/Low)
   @param[in] driver_data data to driver (not used)
   @return_gs_error_t
*/
gs_error_t gs_gpio_virtual_get(gs_gpio_t gpio, bool *value, void * driver_data);

/**
   GPIO virtual get value without error check

   @param[in] gpio The gpio to read
   @param[in] driver_data data to driver (not used)
   @return GPIO value (true/false = High/Low)
*/
bool gs_gpio_virtual_get_nc(gs_gpio_t gpio, void * driver_data);

/**
   GPIO virtual set value

   @param[in] gpio The gpio to set
   @param[in] value GPIO value (true/false = High/Low)
   @param[in] driver_data data to driver (not used)
   @return_gs_error_t
*/
gs_error_t gs_gpio_virtual_set(gs_gpio_t gpio, bool value, void * driver_data);

/**
   GPIO virtual set value without error check

   @param[in] gpio The gpio to set
   @param[in] value GPIO value (true/false = High/Low)
   @param[in] driver_data data to driver (not used)
*/
void gs_gpio_virtual_set_nc(gs_gpio_t gpio, bool value, void * driver_data);

/**
   Initialize GPIO virtual as an external interrupt pin

   @param[in] gpio The gpio to configure
   @param[in] conf Configuration of interrupt pin
   @param[in] driver_data data to driver (not used)
   @return_gs_error_t
 */
gs_error_t gs_gpio_virtual_init_as_interrupt(gs_gpio_t gpio, const gs_interrupt_conf_t * conf, void * driver_data);

/**
   Force set a pin

   This sets a pin regardless if it is configured as input, output or interrupt
   If the pin is configured as interrupt, the registered ISR's will be called within this function,
   if the transition matches (rising/falling)

   @note This function is specific to this driver and is should not be registered.

   @param[in] gpio The gpio to set
   @param[in] value GPIO value (true/false = High/Low)
   @return_gs_error_t
 */
gs_error_t gs_gpio_virtual_force_set(gs_gpio_t gpio, bool value);

/**
   Get transitions

   This gives the number of transitions ((high -> low) + (low -> high)),
   since last time this function was called at this pin. This function resets the counter of the pin.
   An even number means, that the pin has the same state as it was initialized to.

   @note This function is specific to this driver and should not be registered

   @param[in] gpio The gpio, of which transitions are given
   @param[out] transitions Number of transitions
   @return
 */
gs_error_t gs_gpio_virtual_get_transistions(gs_gpio_t gpio, uint32_t * transitions);

#ifdef __cplusplus
}
#endif
#endif
