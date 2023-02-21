/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/linux/drivers/gpio/gpio_virtual.h>

#define MAX_VPINS 500

#define FALLING_EDGE_FLAG 0x1
#define RISING_EDGE_FLAG 0x2

typedef struct {
    gs_gpio_t gpio;
    bool output;
    bool value;
    bool in_use;
    gs_gpio_isr_t isr;
    uint8_t edge_flags;
    uint32_t transistions;
} gs_gpio_virtual_t;

static gs_gpio_virtual_t vpins[MAX_VPINS];

gs_error_t gs_gpio_virtual_initialize(gs_gpio_t gpio, bool output, bool value)
{
    gs_gpio_virtual_t * pin;
    for (uint16_t i = 0; i < MAX_VPINS; i++) {
        pin = &vpins[i];
        if ((!pin->in_use) || ((pin->gpio.pin == gpio.pin) && (pin->gpio.port == gpio.port))) {
            pin->gpio = gpio;
            pin->output = output;
            pin->value = value;
            pin->in_use = true;
            return GS_OK;
        }
    }
    return GS_ERROR_FULL;
}

static gs_gpio_virtual_t * find_vpin(gs_gpio_t * gpio)
{
    gs_gpio_virtual_t * pin;
    for (uint16_t i = 0; i < MAX_VPINS; i++) {
        pin = &vpins[i];
        if (pin->gpio.pin == gpio->pin) {
            if (pin->gpio.port == gpio->port) {
                if (pin->in_use) {
                    return pin;
                }
            }
        }
    }
    return NULL;
}

gs_error_t gs_gpio_virtual_get(gs_gpio_t gpio, bool *value, void * driver_data)
{
    gs_gpio_virtual_t * pin = find_vpin(&gpio);
    if (pin) {
        *value = pin->value;
        return GS_OK;
    }
    return GS_ERROR_NOT_FOUND;
}

bool gs_gpio_virtual_get_nc(gs_gpio_t gpio, void * driver_data)
{
    gs_gpio_virtual_t * pin = find_vpin(&gpio);
    if (pin) {
        return pin->value;
    }
    return false;
}

gs_error_t gs_gpio_virtual_set(gs_gpio_t gpio, bool value, void * driver_data)
{
    gs_gpio_virtual_t * pin = find_vpin(&gpio);
    if (pin) {
        if (pin->output) {
            if (pin->value != value) {
                pin->value = value;
                pin->transistions++;
            }
            return GS_OK;
        }
        return GS_ERROR_PERM;
    }
    return GS_ERROR_NOT_FOUND;
}

void gs_gpio_virtual_set_nc(gs_gpio_t gpio, bool value, void * driver_data)
{
    gs_gpio_virtual_t * pin = find_vpin(&gpio);
    if (pin) {
        if (pin->output) {
            if (pin->value != value) {
                pin->value = value;
                pin->transistions++;
            }
        }
    }
}

gs_error_t gs_gpio_virtual_init_as_interrupt(gs_gpio_t gpio, const gs_interrupt_conf_t * conf, void * driver_data)
{
    gs_gpio_virtual_t * pin;
    for (uint16_t i = 0; i < MAX_VPINS; i++) {
        pin = &vpins[i];
        if ((!pin->in_use) || ((pin->gpio.pin == gpio.pin) && (pin->gpio.port == gpio.port))) {
            pin->gpio = gpio;
            pin->output = false;
            pin->value = 0;
            pin->in_use = true;
            pin->isr = conf->isr;
            if (conf->falling_edge) {
                pin->edge_flags |= FALLING_EDGE_FLAG;
            }
            if (conf->rising_edge) {
                pin->edge_flags |= RISING_EDGE_FLAG;
            }
            return GS_OK;
        }
    }
    return GS_ERROR_FULL;
}

gs_error_t gs_gpio_virtual_force_set(gs_gpio_t gpio, bool value)
{
    gs_gpio_virtual_t * pin = find_vpin(&gpio);
    if (pin) {
        bool old_value = pin->value;
        if (old_value != value) {
            pin->value = value;
            pin->transistions++;
            if (pin->isr) {
                if ((old_value == false) && (pin->edge_flags & RISING_EDGE_FLAG)) {
                    pin->isr(NULL);
                } else if ((old_value == true) && (pin->edge_flags & FALLING_EDGE_FLAG)) {
                    pin->isr(NULL);
                }
            }
        }
        return GS_OK;
    }
    return GS_ERROR_NOT_FOUND;
}

gs_error_t gs_gpio_virtual_get_transistions(gs_gpio_t gpio, uint32_t * transitions)
{
    gs_gpio_virtual_t * pin = find_vpin(&gpio);
    if (pin) {
        *transitions = pin->transistions;
        pin->transistions = 0;
        return GS_OK;
    }
    return GS_ERROR_NOT_FOUND;
}

const gs_gpio_driver_t gs_gpio_virtual_driver = {
    .get_handler = gs_gpio_virtual_get,
    .get_nc_handler = gs_gpio_virtual_get_nc,
    .set_handler = gs_gpio_virtual_set,
    .set_nc_handler = gs_gpio_virtual_set_nc,
    .init_as_interrupt_handler = gs_gpio_virtual_init_as_interrupt,
};


const gs_gpio_driver_entry_t gs_gpio_virtual_driver_entry_all = {
    .port = GS_GPIO_ALL_PORTS,
    .pin = GS_GPIO_ALL_PINS,
    .driver = &gs_gpio_virtual_driver,
    .driver_data = NULL,
};
