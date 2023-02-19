#ifndef GS_UTIL_DRIVERS_GPIO_GPIO_H
#define GS_UTIL_DRIVERS_GPIO_GPIO_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
    @file

    GPIO interface provides a generic interface toward hardware GPIO's.
*/

#include <gs/util/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   GPIO definition.
*/
typedef struct {
    //! Chip/group/port number which the GPIO belongs to.
    uint16_t port;
    //! The pin number of the GPIO.
    uint16_t pin;
} gs_gpio_t;

/**
   GPIO interrupt function.
*/
typedef void (*gs_gpio_isr_t)(gs_context_switch_t * cswitch);

/**
   Configuration for interrupt related to a GPIO.
*/
typedef struct {
    //! True if it shall trigger on rising edge.
    bool rising_edge;
    //! True if it shall trigger on falling edge.
    bool falling_edge;
    //! True if it shall have high priority (if nested isr supported).
    bool high_priority;
    //! ISR to be called on trigger.
    gs_gpio_isr_t isr;
} gs_interrupt_conf_t;

/**
    GPIO get value

    @param[in] gpio The gpio to read
    @param[in] value Returned GPIO value (true/false = High/Low)
    @return_gs_error_t
*/
gs_error_t gs_gpio_get(gs_gpio_t gpio, bool *value);

/**
    GPIO get value without error check

    @param[in] gpio The gpio to read
    @return GPIO value (true/false = High/Low)
*/
bool gs_gpio_get_nc(gs_gpio_t gpio);

/**
    GPIO set value

    @param[in] gpio The gpio to set
    @param[in] value GPIO value (true/false = High/Low)
    @return_gs_error_t
*/
gs_error_t gs_gpio_set(gs_gpio_t gpio, bool value);

/**
    GPIO set value without error check

    @param[in] gpio The gpio to set
    @param[in] value GPIO value (true/false = High/Low)
*/
void gs_gpio_set_nc(gs_gpio_t gpio, bool value);

/**
    Initialize GPIO as an external interrupt pin.

    @param[in] gpio The gpio to configure
    @param[in] conf Configuration of interrupt pin
    @return_gs_error_t
 */
gs_error_t gs_gpio_init_as_interrupt(gs_gpio_t gpio, const gs_interrupt_conf_t * conf);

#ifdef __cplusplus
}
#endif
#endif
