/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_H_INCLUDED
#define DRIVER_INIT_H_INCLUDED

#include "pinout.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_dac_sync.h>

#include <hal_mci_sync.h>

#include <hal_spi_m_sync.h>

#include <hal_usart_sync.h>
#include <hpl_uart_base.h>

#include <hal_usart_sync.h>
#include <hpl_uart_base.h>

#include <hal_usart_sync.h>
#include <hpl_uart_base.h>

#include <hal_usart_sync.h>
#include <hpl_uart_base.h>

#include <hal_usart_sync.h>
#include <hpl_uart_base.h>

#include <hal_can_async.h>

extern struct dac_sync_descriptor DAC_DINY;

extern struct mci_sync_desc         IO_BUS;
extern struct spi_m_sync_descriptor SPI;

extern struct usart_sync_descriptor UART_MUTUAL;

extern struct usart_sync_descriptor UART_UHF;

extern struct usart_sync_descriptor UART_DSS;

extern struct usart_sync_descriptor UART_PL;

extern struct usart_sync_descriptor UART_ADCS;
extern struct can_async_descriptor  CAN_P_N;

void DAC_DINY_PORT_init(void);
void DAC_DINY_CLOCK_init(void);
void DAC_DINY_init(void);

void IO_BUS_PORT_init(void);
void IO_BUS_CLOCK_init(void);
void IO_BUS_init(void);

void SPI_PORT_init(void);
void SPI_CLOCK_init(void);
void SPI_init(void);

void UART_MUTUAL_PORT_init(void);
void UART_MUTUAL_CLOCK_init(void);
void UART_MUTUAL_init(void);
void UART_MUTUAL_example(void);

void UART_UHF_PORT_init(void);
void UART_UHF_CLOCK_init(void);
void UART_UHF_init(void);
void UART_UHF_example(void);

void UART_DSS_PORT_init(void);
void UART_DSS_CLOCK_init(void);
void UART_DSS_init(void);
void UART_DSS_example(void);

void UART_PL_PORT_init(void);
void UART_PL_CLOCK_init(void);
void UART_PL_init(void);
void UART_PL_example(void);

void UART_ADCS_PORT_init(void);
void UART_ADCS_CLOCK_init(void);
void UART_ADCS_init(void);
void UART_ADCS_example(void);

void CAN_P_N_PORT_init(void);
void CAN_P_N_CLOCK_init(void);
void CAN_P_N_init(void);
void CAN_P_N_example(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_H_INCLUDED
