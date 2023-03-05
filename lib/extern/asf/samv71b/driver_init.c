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

#include <hpl_spi_base.h>

/* The priority of the peripheral should be between the low and high interrupt priority set by chosen RTOS,
 * Otherwise, some of the RTOS APIs may fail to work inside interrupts
 * In case of FreeRTOS,the Lowest Interrupt priority is set by configLIBRARY_LOWEST_INTERRUPT_PRIORITY and
 * Maximum interrupt priority by configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, So Interrupt Priority of the peripheral
 * should be between configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY and configLIBRARY_LOWEST_INTERRUPT_PRIORITY
 */
#define PERIPHERAL_INTERRUPT_PRIORITY (configLIBRARY_LOWEST_INTERRUPT_PRIORITY - 1)

struct can_async_descriptor CAN_0;

struct flash_descriptor FLASH_0;

struct qspi_sync_descriptor QSPI_INSTANCE;

struct calendar_descriptor CALENDAR_0;

struct spi_m_os_descriptor SPI_0;

struct i2c_m_os_desc I2C_0;

struct usart_os_descriptor USART_MUTUAL;
uint8_t                    USART_MUTUAL_buffer[USART_MUTUAL_BUFFER_SIZE];

struct usart_os_descriptor USART_UHF;
uint8_t                    USART_UHF_buffer[USART_UHF_BUFFER_SIZE];

struct usart_os_descriptor USART_DSS;
uint8_t                    USART_DSS_buffer[USART_DSS_BUFFER_SIZE];

struct usart_os_descriptor USART_PL;
uint8_t                    USART_PL_buffer[USART_PL_BUFFER_SIZE];

struct usart_os_descriptor USART_ADCS;
uint8_t                    USART_ADCS_buffer[USART_ADCS_BUFFER_SIZE];

void FLASH_0_CLOCK_init(void)
{
}

void FLASH_0_init(void)
{
	FLASH_0_CLOCK_init();
	flash_init(&FLASH_0, EFC);
}

void QSPI_INSTANCE_PORT_init(void)
{

	// Set pin direction to input
	gpio_set_pin_direction(PA11, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PA11,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA11, MUX_PA11A_QSPI_QCS);

	gpio_set_pin_direction(PA13,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PA13,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(PA13,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA13,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <MUX_PA13A_QSPI_QIO0"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      MUX_PA13A_QSPI_QIO0);

	gpio_set_pin_direction(PA12,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PA12,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(PA12,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA12,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <MUX_PA12A_QSPI_QIO1"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      MUX_PA12A_QSPI_QIO1);

	gpio_set_pin_direction(PA17,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PA17,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(PA17,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA17,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <MUX_PA17A_QSPI_QIO2"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      MUX_PA17A_QSPI_QIO2);

	gpio_set_pin_direction(PD31,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PD31,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(PD31,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PD31,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <MUX_PD31A_QSPI_QIO3"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      MUX_PD31A_QSPI_QIO3);

	// Set pin direction to input
	gpio_set_pin_direction(PA14, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PA14,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA14, MUX_PA14A_QSPI_QSCK);
}

void QSPI_INSTANCE_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_QSPI);
}

void QSPI_INSTANCE_init(void)
{
	QSPI_INSTANCE_CLOCK_init();
	qspi_sync_init(&QSPI_INSTANCE, QSPI);
	QSPI_INSTANCE_PORT_init();
}

void CALENDAR_0_CLOCK_init(void)
{
}

void CALENDAR_0_init(void)
{
	CALENDAR_0_CLOCK_init();
	calendar_init(&CALENDAR_0, RTC);
}

void SPI_0_PORT_init(void)
{

	gpio_set_pin_function(PC26, MUX_PC26C_SPI1_MISO);

	gpio_set_pin_function(PC27, MUX_PC27C_SPI1_MOSI);

	gpio_set_pin_function(PC24, MUX_PC24C_SPI1_SPCK);
}

void SPI_0_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_SPI1);
}

void SPI_0_init(void)
{

	SPI_0_CLOCK_init();
	spi_m_os_set_func_ptr(&SPI_0, _spi_get_spi_m_async());
	spi_m_os_init(&SPI_0, SPI1);
	NVIC_SetPriority(SPI1_IRQn, PERIPHERAL_INTERRUPT_PRIORITY);
	spi_m_os_enable(&SPI_0);
	SPI_0_PORT_init();
}

void I2C_0_PORT_init(void)
{

	gpio_set_pin_function(PA4, MUX_PA4A_TWIHS0_TWCK0);

	gpio_set_pin_function(PA3, MUX_PA3A_TWIHS0_TWD0);
}

void I2C_0_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_TWIHS0);
}

void I2C_0_init(void)
{
	I2C_0_CLOCK_init();

	i2c_m_os_init(&I2C_0, TWIHS0);

	NVIC_SetPriority(TWIHS0_IRQn, PERIPHERAL_INTERRUPT_PRIORITY);
	I2C_0_PORT_init();
}

void USART_MUTUAL_PORT_init(void)
{

	gpio_set_pin_function(PA9, MUX_PA9A_UART0_URXD0);

	gpio_set_pin_function(PA10, MUX_PA10A_UART0_UTXD0);
}

void USART_MUTUAL_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_UART0);
}

void USART_MUTUAL_init(void)
{
	USART_MUTUAL_CLOCK_init();
	usart_os_init(&USART_MUTUAL, UART0, USART_MUTUAL_buffer, USART_MUTUAL_BUFFER_SIZE, (void *)_uart_get_usart_async());
	usart_os_enable(&USART_MUTUAL);
	USART_MUTUAL_PORT_init();
	NVIC_SetPriority(UART0_IRQn, PERIPHERAL_INTERRUPT_PRIORITY);
}

void USART_UHF_PORT_init(void)
{

	gpio_set_pin_function(PA5, MUX_PA5C_UART1_URXD1);

	gpio_set_pin_function(PA6, MUX_PA6C_UART1_UTXD1);
}

void USART_UHF_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_UART1);
}

void USART_UHF_init(void)
{
	USART_UHF_CLOCK_init();
	usart_os_init(&USART_UHF, UART1, USART_UHF_buffer, USART_UHF_BUFFER_SIZE, (void *)_uart_get_usart_async());
	usart_os_enable(&USART_UHF);
	USART_UHF_PORT_init();
	NVIC_SetPriority(UART1_IRQn, PERIPHERAL_INTERRUPT_PRIORITY);
}

void USART_DSS_PORT_init(void)
{

	gpio_set_pin_function(PD25, MUX_PD25C_UART2_URXD2);

	gpio_set_pin_function(PD26, MUX_PD26C_UART2_UTXD2);
}

void USART_DSS_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_UART2);
}

void USART_DSS_init(void)
{
	USART_DSS_CLOCK_init();
	usart_os_init(&USART_DSS, UART2, USART_DSS_buffer, USART_DSS_BUFFER_SIZE, (void *)_uart_get_usart_async());
	usart_os_enable(&USART_DSS);
	USART_DSS_PORT_init();
	NVIC_SetPriority(UART2_IRQn, PERIPHERAL_INTERRUPT_PRIORITY);
}

void USART_PL_PORT_init(void)
{

	gpio_set_pin_function(PD28, MUX_PD28A_UART3_URXD3);

	gpio_set_pin_function(PD30, MUX_PD30A_UART3_UTXD3);
}

void USART_PL_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_UART3);
}

void USART_PL_init(void)
{
	USART_PL_CLOCK_init();
	usart_os_init(&USART_PL, UART3, USART_PL_buffer, USART_PL_BUFFER_SIZE, (void *)_uart_get_usart_async());
	usart_os_enable(&USART_PL);
	USART_PL_PORT_init();
	NVIC_SetPriority(UART3_IRQn, PERIPHERAL_INTERRUPT_PRIORITY);
}

void USART_ADCS_PORT_init(void)
{

	gpio_set_pin_function(PD18, MUX_PD18C_UART4_URXD4);

	gpio_set_pin_function(PD19, MUX_PD19C_UART4_UTXD4);
}

void USART_ADCS_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_UART4);
}

void USART_ADCS_init(void)
{
	USART_ADCS_CLOCK_init();
	usart_os_init(&USART_ADCS, UART4, USART_ADCS_buffer, USART_ADCS_BUFFER_SIZE, (void *)_uart_get_usart_async());
	usart_os_enable(&USART_ADCS);
	USART_ADCS_PORT_init();
	NVIC_SetPriority(UART4_IRQn, PERIPHERAL_INTERRUPT_PRIORITY);
}

/**
 * \brief MCAN Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void CAN_0_CLOCK_init()
{
	_pmc_enable_periph_clock(ID_MCAN0);
}

/**
 * \brief MCAN pinmux initialization function
 *
 * Set each required pin to MCAN functionality
 */
void CAN_0_PORT_init(void)
{
}
/**
 * \brief CAN initialization function
 *
 * Enables CAN peripheral, clocks and initializes CAN driver
 */
void CAN_0_init(void)
{
	CAN_0_CLOCK_init();
	CAN_0_PORT_init();
	can_async_init(&CAN_0, MCAN0);
}

void system_init(void)
{
	init_mcu();

	_pmc_enable_periph_clock(ID_PIOA);

	/* Disable Watchdog */
	hri_wdt_set_MR_WDDIS_bit(WDT);

	/* GPIO on PA16 */

	gpio_set_pin_level(SPI_CS_ADC,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_CS_ADC, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_CS_ADC, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA25 */

	gpio_set_pin_level(SPI_CS_C,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_CS_C, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_CS_C, GPIO_PIN_FUNCTION_OFF);

	FLASH_0_init();

	QSPI_INSTANCE_init();

	CALENDAR_0_init();

	SPI_0_init();

	I2C_0_init();

	USART_MUTUAL_init();

	USART_UHF_init();

	USART_DSS_init();

	USART_PL_init();

	USART_ADCS_init();

	CAN_0_init();
}
