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

#include <hal_flash.h>

#include <hal_qspi_sync.h>

#include <hal_calendar.h>

#include <hal_spi_m_os.h>

#include <hal_i2c_m_os.h>

#include <hal_usart_os.h>

#include <hal_usart_os.h>

#include <hal_usart_os.h>

#include <hal_usart_os.h>

#include <hal_usart_os.h>

#include <hal_can_async.h>

#define USART_MUTUAL_BUFFER_SIZE 16

#define USART_UHF_BUFFER_SIZE 16

#define USART_DSS_BUFFER_SIZE 16

#define USART_PL_BUFFER_SIZE 16

#define USART_ADCS_BUFFER_SIZE 16

extern struct flash_descriptor FLASH_0;

extern struct qspi_sync_descriptor QSPI_INSTANCE;

extern struct calendar_descriptor CALENDAR_0;

extern struct spi_m_os_descriptor SPI_0;

extern struct i2c_m_os_desc I2C_0;

extern struct usart_os_descriptor USART_MUTUAL;
extern uint8_t                    USART_MUTUAL_buffer[];

extern struct usart_os_descriptor USART_UHF;
extern uint8_t                    USART_UHF_buffer[];

extern struct usart_os_descriptor USART_DSS;
extern uint8_t                    USART_DSS_buffer[];

extern struct usart_os_descriptor USART_PL;
extern uint8_t                    USART_PL_buffer[];

extern struct usart_os_descriptor  USART_ADCS;
extern uint8_t                     USART_ADCS_buffer[];
extern struct can_async_descriptor CAN_0;

void FLASH_0_init(void);
void FLASH_0_CLOCK_init(void);

void QSPI_INSTANCE_PORT_init(void);
void QSPI_INSTANCE_CLOCK_init(void);
void QSPI_INSTANCE_init(void);

void CALENDAR_0_CLOCK_init(void);
void CALENDAR_0_init(void);

void SPI_0_PORT_init(void);

void SPI_0_CLOCK_init(void);

void SPI_0_init(void);

void I2C_0_PORT_init(void);
void I2C_0_CLOCK_init(void);
void I2C_0_init(void);

void USART_MUTUAL_PORT_init(void);
void USART_MUTUAL_CLOCK_init(void);
void USART_MUTUAL_init(void);

void USART_UHF_PORT_init(void);
void USART_UHF_CLOCK_init(void);
void USART_UHF_init(void);

void USART_DSS_PORT_init(void);
void USART_DSS_CLOCK_init(void);
void USART_DSS_init(void);

void USART_PL_PORT_init(void);
void USART_PL_CLOCK_init(void);
void USART_PL_init(void);

void USART_ADCS_PORT_init(void);
void USART_ADCS_CLOCK_init(void);
void USART_ADCS_init(void);

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
