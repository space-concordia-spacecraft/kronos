/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_H_INCLUDED
#define DRIVER_INIT_H_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_mci_sync.h>

#include <hal_calendar.h>

#include <hal_i2c_m_os.h>

#include <hal_usart_os.h>

#include <hal_usart_os.h>

#include <hal_can_async.h>

#define USART_0_BUFFER_SIZE 32

#define TARGET_IO_BUFFER_SIZE 32

extern struct mci_sync_desc MCI_0;

extern struct calendar_descriptor CALENDAR_0;

extern struct i2c_m_os_desc I2C_0;

extern struct usart_os_descriptor USART_0;
extern uint8_t                    USART_0_buffer[];

extern struct usart_os_descriptor  TARGET_IO;
extern uint8_t                     TARGET_IO_buffer[];
extern struct can_async_descriptor CAN_0;

void MCI_0_PORT_init(void);
void MCI_0_CLOCK_init(void);
void MCI_0_init(void);

void CALENDAR_0_CLOCK_init(void);
void CALENDAR_0_init(void);

void I2C_0_PORT_init(void);
void I2C_0_CLOCK_init(void);
void I2C_0_init(void);

void USART_0_PORT_init(void);
void USART_0_CLOCK_init(void);
void USART_0_init(void);

void TARGET_IO_PORT_init(void);
void TARGET_IO_CLOCK_init(void);
void TARGET_IO_init(void);

void CAN_0_PORT_init(void);
void CAN_0_CLOCK_init(void);
void CAN_0_init(void);
void CAN_0_example(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_H_INCLUDED
