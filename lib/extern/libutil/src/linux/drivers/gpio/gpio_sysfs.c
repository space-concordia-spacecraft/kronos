/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/**
    @file

    @brief GPIO Implementation for Linux of the GPIO API in libutil.

    The GPIO driver provides a simple interface toward driving HW GPIO's.
*/

#include <gs/util/linux/drivers/gpio/gpio_sysfs.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>

#include <gs/util/log.h>
#include <gs/util/thread.h>
#include <gs/util/linux/sysfs_helper.h>

gs_error_t gs_gpio_sysfs_initialize(gs_gpio_t gpio, bool output,bool init_value, bool active_low)
{
    char gpio_pin_str[6];
    snprintf(gpio_pin_str, sizeof(gpio_pin_str), "%d", gpio.pin);

    /* Try to unexport first */
    gs_sysfs_write_file("/sys/class/gpio/unexport", gpio_pin_str);

    if (gs_sysfs_write_file("/sys/class/gpio/export", gpio_pin_str) != GS_OK) {
        log_warning("failed to export GPIO %s: %s", gpio_pin_str, strerror(errno));
        return GS_ERROR_NOT_SUPPORTED;
    }

    char gpio_sys_fname[128];
    snprintf(gpio_sys_fname, sizeof(gpio_sys_fname), "/sys/class/gpio/gpio%d/active_low", gpio.pin);
    const char * active_low_str = active_low ? "1" : "0";

    if (gs_sysfs_write_file(gpio_sys_fname, active_low_str) != GS_OK) {
        log_warning("failed to set GPIO %d active_low: %s", gpio.pin, strerror(errno));
        return GS_ERROR_NOT_SUPPORTED;
    }

    snprintf(gpio_sys_fname, sizeof(gpio_sys_fname), "/sys/class/gpio/gpio%d/direction", gpio.pin);

    /* Glitch-free output set (high/low makes pin an output and sets value to 1/0 respectively)*/
    const char * dir = output ? (init_value ? "high" : "low") : "in";

    if (gs_sysfs_write_file(gpio_sys_fname, dir) != GS_OK) {
        log_warning("failed to set GPIO %d direction: %s", gpio.pin, strerror(errno));
        return GS_ERROR_NOT_SUPPORTED;
    }

    return GS_OK;
}

gs_error_t gs_gpio_sysfs_get(gs_gpio_t gpio, bool *value, void * driver_data)
{
    char gpio_sys_fname[128];
    snprintf(gpio_sys_fname, sizeof(gpio_sys_fname), "/sys/class/gpio/gpio%d/value", gpio.pin);

    char file_value = '0';
    gs_error_t ret = gs_sysfs_read_file(gpio_sys_fname, &file_value, 1);
    if (ret == GS_OK) {
        if (file_value == '1') {
            *value = true;
        } else {
            *value = false;
        }
    }

    return ret;
}

bool gs_gpio_sysfs_get_nc(gs_gpio_t gpio, void * driver_data)
{
    bool value;
    gs_error_t ret = gs_gpio_sysfs_get(gpio, &value, driver_data);
    if (ret == GS_OK) {
        return value;
    }

    return false;
}

gs_error_t gs_gpio_sysfs_set(gs_gpio_t gpio, bool value, void * driver_data)
{
    const char *value_str = value ? "1" : "0";

    char gpio_sys_fname[128];
    snprintf(gpio_sys_fname, sizeof(gpio_sys_fname), "/sys/class/gpio/gpio%d/value", gpio.pin);
    if (access(gpio_sys_fname, W_OK) == 0) {
        return gs_sysfs_write_file(gpio_sys_fname, value_str);
    }

    log_error("GPIO %d not initialized - Can't set the output.", gpio.pin);
    return GS_ERROR_ACCESS;
}

void gs_gpio_sysfs_set_nc(gs_gpio_t gpio, bool value, void * driver_data)
{
    const char *value_str = value ? "1" : "0";

    char gpio_sys_fname[128];
    snprintf(gpio_sys_fname, sizeof(gpio_sys_fname), "/sys/class/gpio/gpio%d/value", gpio.pin);
    if (access(gpio_sys_fname, W_OK) == 0) {
        gs_sysfs_write_file(gpio_sys_fname, value_str);
        return;
    }
    log_error("GPIO %d not initialized - Can't set the output.", gpio.pin);
}

typedef struct {
    gs_gpio_isr_t isr;
    gs_gpio_t gpio;
}gs_gpio_sysfs_irq_t;

static void *gs_gpio_irq_thread(void * parm)
{
    gs_gpio_sysfs_irq_t *arg = (gs_gpio_sysfs_irq_t*)parm;
    char gpio_sys_fname[128];
    snprintf(gpio_sys_fname, sizeof(gpio_sys_fname), "/sys/class/gpio/gpio%d/value", arg->gpio.pin);
    int fd = open(gpio_sys_fname, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        log_error("Failed to open value file");
        log_error("GPIO %d open value file returned error %s", arg->gpio.pin, strerror(errno));
        return NULL;
    }
    struct pollfd fdset[1];
    char buf[16];
    fdset[0].fd = fd;
    fdset[0].events = POLLPRI;
    while(true) {
        if (poll(fdset, 1, -1) < 0) {
            log_error("GPIO %d Poll returned error %s", arg->gpio.pin, strerror(errno));
        }
        if (fdset[0].revents & POLLPRI) {
            if (arg->isr) {
                arg->isr(NULL);
            }
            lseek(fdset[0].fd, 0, SEEK_SET);
            if (read(fdset[0].fd, buf, sizeof(buf)) < 0) {
                log_error("GPIO %d read returned error %s", arg->gpio.pin, strerror(errno));
	    }
        }
    }
    return NULL;
}

gs_error_t gs_gpio_sysfs_init_as_interrupt(gs_gpio_t gpio, const gs_interrupt_conf_t * conf, void * driver_data)
{
    gs_error_t err = gs_gpio_sysfs_initialize(gpio, false, false, false);
    if (err) {
        log_error("Failed to initializse GPIO %d as interrupt", gpio.pin);
        return err;
    }

    char gpio_sys_fname[128];
    snprintf(gpio_sys_fname, sizeof(gpio_sys_fname), "/sys/class/gpio/gpio%d/edge", gpio.pin);

    if (access(gpio_sys_fname, W_OK) != 0) {
        log_error("GPIO %d not initialized - Can't set the edge input.", gpio.pin);
        return GS_ERROR_IO;
    }
    const char *value_str;
    if (conf->rising_edge && conf->falling_edge) {
        value_str = "both";
    } else if (conf->rising_edge) {
        value_str = "rising";
    } else if (conf->falling_edge) {
        value_str = "falling";
    } else {
        log_error("GPIO %d Neither falling nor rising edge specified", gpio.pin);
        return GS_ERROR_ARG;
    }
    gs_sysfs_write_file(gpio_sys_fname, value_str);

    gs_gpio_sysfs_irq_t *arg = malloc(sizeof(*arg));
    if (arg == NULL) {
        log_error("GPIO %d, %s Out of memory", gpio.pin, __FUNCTION__);
        return GS_ERROR_ALLOC;
    }
    arg->isr = conf->isr;
    arg->gpio = gpio;
    gs_error_t error = gs_thread_create("GPIO_IRQ",
                                        gs_gpio_irq_thread,
                                        arg,
                                        0, /* Stack size not used on linux */
                                        GS_THREAD_PRIORITY_HIGH,
                                        0,
                                        NULL);
    return error;
}

const gs_gpio_driver_t gs_gpio_sysfs_driver = {
    .get_handler = gs_gpio_sysfs_get,
    .get_nc_handler = gs_gpio_sysfs_get_nc,
    .set_handler = gs_gpio_sysfs_set,
    .set_nc_handler = gs_gpio_sysfs_set_nc,
    .init_as_interrupt_handler = gs_gpio_sysfs_init_as_interrupt,
};
