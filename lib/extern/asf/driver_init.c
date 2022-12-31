/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <hal_init.h>
#include <hpl_pmc.h>
#include <peripheral_clk_config.h>
#include <utils.h>

struct usart_sync_descriptor TARGET_IO;

struct usart_sync_descriptor USART_0;

void TARGET_IO_PORT_init(void)
{
}

void TARGET_IO_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_UART0);
}

void TARGET_IO_init(void)
{
	TARGET_IO_CLOCK_init();
	usart_sync_init(&TARGET_IO, UART0, _uart_get_usart_sync());
	TARGET_IO_PORT_init();
}

void USART_0_PORT_init(void)
{

	gpio_set_pin_function(PD25, MUX_PD25C_UART2_URXD2);

	gpio_set_pin_function(PD26, MUX_PD26C_UART2_UTXD2);
}

void USART_0_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_UART2);
}

void USART_0_init(void)
{
	USART_0_CLOCK_init();
	usart_sync_init(&USART_0, UART2, _uart_get_usart_sync());
	USART_0_PORT_init();
}

void system_init(void)
{
	init_mcu();

	_pmc_enable_periph_clock(ID_PIOC);

	/* Disable Watchdog */
	hri_wdt_set_MR_WDDIS_bit(WDT);

	/* GPIO on PC8 */

	gpio_set_pin_level(LED0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(LED0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LED0, GPIO_PIN_FUNCTION_OFF);

	TARGET_IO_init();

	USART_0_init();
}
