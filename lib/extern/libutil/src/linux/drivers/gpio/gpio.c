/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/check.h>
#include <gs/util/linux/drivers/gpio/gpio.h>

#define MAX_DRIVERS 20

typedef struct {
    gs_gpio_driver_entry_t entry;
    bool in_use;
} gs_gpio_driver_handle_t;

static gs_gpio_driver_handle_t gpio_drivers[MAX_DRIVERS];
static uint8_t max_index_in_use = 0;


static inline gs_gpio_driver_entry_t * gs_find_driver_entry(gs_gpio_t * gpio)
{
    gs_gpio_driver_handle_t * handle;
    for (int i = max_index_in_use; i >= 0; i--) {
        handle = &gpio_drivers[i];
        if (((gpio->pin == handle->entry.pin) || (handle->entry.pin == GS_GPIO_ALL_PINS)) &&
            ((gpio->port == handle->entry.port) || (handle->entry.port == GS_GPIO_ALL_PORTS)) &&
            (handle->in_use == true)) {
            return &handle->entry;
        }
    }
    return NULL;
}

gs_error_t gs_gpio_get(gs_gpio_t gpio, bool *value)
{
    gs_gpio_driver_entry_t * driver_entry = gs_find_driver_entry(&gpio);
    if (driver_entry) {
        if (driver_entry->driver->get_handler) {
            return driver_entry->driver->get_handler(gpio, value, driver_entry->driver_data);
        }
    }
    return GS_ERROR_NOT_FOUND;
}

bool gs_gpio_get_nc(gs_gpio_t gpio)
{
    gs_gpio_driver_entry_t * driver_entry = gs_find_driver_entry(&gpio);
    if (driver_entry) {
        if (driver_entry->driver->get_nc_handler) {
            return driver_entry->driver->get_nc_handler(gpio, driver_entry->driver_data);
        }
    }
    return false;
}

gs_error_t gs_gpio_set(gs_gpio_t gpio, bool value)
{
    gs_gpio_driver_entry_t * driver_entry = gs_find_driver_entry(&gpio);
    if (driver_entry) {
        if (driver_entry->driver->set_handler) {
            return driver_entry->driver->set_handler(gpio, value, driver_entry->driver_data);
        }
    }
    return GS_ERROR_NOT_FOUND;
}

void gs_gpio_set_nc(gs_gpio_t gpio, bool value)
{
    gs_gpio_driver_entry_t * driver_entry = gs_find_driver_entry(&gpio);
    if (driver_entry) {
        if (driver_entry->driver->set_nc_handler) {
            driver_entry->driver->set_nc_handler(gpio, value, driver_entry->driver_data);
        }
    }
}

gs_error_t gs_gpio_init_as_interrupt(gs_gpio_t gpio, const gs_interrupt_conf_t * conf)
{
    gs_gpio_driver_entry_t * driver_entry = gs_find_driver_entry(&gpio);
    if (driver_entry) {
        if (driver_entry->driver->init_as_interrupt_handler) {
            return driver_entry->driver->init_as_interrupt_handler(gpio, conf, driver_entry->driver_data);
        }
    }
    return GS_ERROR_NOT_FOUND;
}

gs_error_t gs_gpio_register_driver(const gs_gpio_driver_entry_t * driver_entry)
{
    GS_CHECK_ARG(driver_entry != NULL);
    GS_CHECK_ARG(driver_entry->driver != NULL);

    gs_gpio_driver_handle_t * handle;
    for (uint8_t i = 0; i < MAX_DRIVERS; i++) {
        handle = &gpio_drivers[i];
        if (handle->in_use == false) {
            handle->entry = *driver_entry;
            handle->in_use = true;
            max_index_in_use = i;
            return GS_OK;
        }
    }
    /* Not enough space in buffer */
    return GS_ERROR_FULL;
}
