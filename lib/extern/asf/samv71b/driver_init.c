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

struct usart_os_descriptor UART_MUTUAL;
uint8_t                    UART_MUTUAL_buffer[UART_MUTUAL_BUFFER_SIZE];

struct usart_os_descriptor USART_UHF;
uint8_t                    USART_UHF_buffer[USART_UHF_BUFFER_SIZE];

struct usart_os_descriptor UART_DSS;
uint8_t                    UART_DSS_buffer[UART_DSS_BUFFER_SIZE];

struct usart_os_descriptor UART_PL;
uint8_t                    UART_PL_buffer[UART_PL_BUFFER_SIZE];

struct usart_os_descriptor UART_ADCS;
uint8_t                    UART_ADCS_buffer[UART_ADCS_BUFFER_SIZE];

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

void UART_MUTUAL_PORT_init(void)
{

	gpio_set_pin_function(PA9, MUX_PA9A_UART0_URXD0);

	gpio_set_pin_function(PA10, MUX_PA10A_UART0_UTXD0);
}

void UART_MUTUAL_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_UART0);
}

void UART_MUTUAL_init(void)
{
	UART_MUTUAL_CLOCK_init();
	usart_os_init(&UART_MUTUAL, UART0, UART_MUTUAL_buffer, UART_MUTUAL_BUFFER_SIZE, (void *)_uart_get_usart_async());
	usart_os_enable(&UART_MUTUAL);
	UART_MUTUAL_PORT_init();
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

void UART_DSS_PORT_init(void)
{

	gpio_set_pin_function(PD25, MUX_PD25C_UART2_URXD2);

	gpio_set_pin_function(PD26, MUX_PD26C_UART2_UTXD2);
}

void UART_DSS_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_UART2);
}

void UART_DSS_init(void)
{
	UART_DSS_CLOCK_init();
	usart_os_init(&UART_DSS, UART2, UART_DSS_buffer, UART_DSS_BUFFER_SIZE, (void *)_uart_get_usart_async());
	usart_os_enable(&UART_DSS);
	UART_DSS_PORT_init();
	NVIC_SetPriority(UART2_IRQn, PERIPHERAL_INTERRUPT_PRIORITY);
}

void UART_PL_PORT_init(void)
{

	gpio_set_pin_function(PD28, MUX_PD28A_UART3_URXD3);

	gpio_set_pin_function(PD30, MUX_PD30A_UART3_UTXD3);
}

void UART_PL_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_UART3);
}

void UART_PL_init(void)
{
	UART_PL_CLOCK_init();
	usart_os_init(&UART_PL, UART3, UART_PL_buffer, UART_PL_BUFFER_SIZE, (void *)_uart_get_usart_async());
	usart_os_enable(&UART_PL);
	UART_PL_PORT_init();
	NVIC_SetPriority(UART3_IRQn, PERIPHERAL_INTERRUPT_PRIORITY);
}

void UART_ADCS_PORT_init(void)
{

	gpio_set_pin_function(PD18, MUX_PD18C_UART4_URXD4);

	gpio_set_pin_function(PD19, MUX_PD19C_UART4_UTXD4);
}

void UART_ADCS_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_UART4);
}

void UART_ADCS_init(void)
{
	UART_ADCS_CLOCK_init();
	usart_os_init(&UART_ADCS, UART4, UART_ADCS_buffer, UART_ADCS_BUFFER_SIZE, (void *)_uart_get_usart_async());
	usart_os_enable(&UART_ADCS);
	UART_ADCS_PORT_init();
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

	gpio_set_pin_function(PB3, MUX_PB3A_MCAN0_CANRX0);

	gpio_set_pin_function(PB2, MUX_PB2A_MCAN0_CANTX0);
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

	_pmc_enable_periph_clock(ID_PIOB);

	_pmc_enable_periph_clock(ID_PIOC);

	_pmc_enable_periph_clock(ID_PIOD);

	/* Disable Watchdog */
	hri_wdt_set_MR_WDDIS_bit(WDT);

	/* GPIO on PA1 */

	// Set pin direction to input
	gpio_set_pin_direction(UMB_DETECT, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(UMB_DETECT,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(UMB_DETECT, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA2 */

	gpio_set_pin_level(MAG_M_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(MAG_M_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(MAG_M_EN, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA16 */

	gpio_set_pin_level(SPI_CS_CDH_ADC,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_CS_CDH_ADC, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_CS_CDH_ADC, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA19 */

	gpio_set_pin_level(MAG_T_Y_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(MAG_T_Y_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(MAG_T_Y_EN, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA21 */

	gpio_set_pin_level(MAG_T_X_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(MAG_T_X_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(MAG_T_X_EN, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA22 */

	gpio_set_pin_level(MAG_T_Z_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(MAG_T_Z_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(MAG_T_Z_EN, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA23 */

	gpio_set_pin_function(MAG_T_PWM, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA24 */

	gpio_set_pin_level(MAG_T_Z_DIR,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(MAG_T_Z_DIR, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(MAG_T_Z_DIR, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA25 */

	gpio_set_pin_level(SPI_CS_ADCS_ADC_1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_CS_ADCS_ADC_1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_CS_ADCS_ADC_1, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA26 */

	gpio_set_pin_level(I2C_PSS_IO_ADDR_1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(I2C_PSS_IO_ADDR_1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(I2C_PSS_IO_ADDR_1, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA27 */

	gpio_set_pin_level(HEATER_PWM_2,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(HEATER_PWM_2, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(HEATER_PWM_2, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA28 */

	gpio_set_pin_level(SPI_CS_PSS_ADC_1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_CS_PSS_ADC_1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_CS_PSS_ADC_1, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA30 */

	gpio_set_pin_level(SPI_CS_PSS_ADC_0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_CS_PSS_ADC_0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_CS_PSS_ADC_0, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PB0 */

	gpio_set_pin_level(HEATER_PWM_3,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(HEATER_PWM_3, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(HEATER_PWM_3, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PB1 */

	gpio_set_pin_level(RW_UART_SEL_ADDR1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(RW_UART_SEL_ADDR1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(RW_UART_SEL_ADDR1, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PB13 */

	gpio_set_pin_level(RWX_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(RWX_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(RWX_EN, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC0 */

	gpio_set_pin_level(RAD_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(RAD_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(RAD_EN, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC1 */

	gpio_set_pin_level(HEATER_PL_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(HEATER_PL_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(HEATER_PL_EN, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC2 */

	gpio_set_pin_level(RW_UART_SEL_ADDR0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(RW_UART_SEL_ADDR0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(RW_UART_SEL_ADDR0, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC3 */

	gpio_set_pin_level(MAG_T_Y_DIR,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(MAG_T_Y_DIR, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(MAG_T_Y_DIR, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC6 */

	gpio_set_pin_level(RWZ_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(RWZ_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(RWZ_EN, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC7 */

	gpio_set_pin_function(MAG_T_X_PWM, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC8 */

	// Set pin direction to input
	gpio_set_pin_direction(REQ_PSS, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(REQ_PSS,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(REQ_PSS, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC9 */

	// Set pin direction to input
	gpio_set_pin_direction(WATCHDOG_MRST, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(WATCHDOG_MRST,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(WATCHDOG_MRST, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC10 */

	gpio_set_pin_level(ERASE_OBC,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(ERASE_OBC, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(ERASE_OBC, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC11 */

	gpio_set_pin_level(PW_MUTUAL,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PW_MUTUAL, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PW_MUTUAL, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC12 */

	// Set pin direction to input
	gpio_set_pin_direction(PW_STATUS_FLAG_EXRAD, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PW_STATUS_FLAG_EXRAD,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PW_STATUS_FLAG_EXRAD, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC13 */

	gpio_set_pin_level(RESET_PDS,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(RESET_PDS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(RESET_PDS, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC14 */

	gpio_set_pin_level(RESET_PSS,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(RESET_PSS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(RESET_PSS, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC15 */

	gpio_set_pin_level(SPI_CS_ADCS_ADC_0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_CS_ADCS_ADC_0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_CS_ADCS_ADC_0, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC18 */

	gpio_set_pin_level(SPI_CS_PSS_ADC_2,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_CS_PSS_ADC_2, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_CS_PSS_ADC_2, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC19 */

	gpio_set_pin_level(SPI_CS_PDS_ADC_0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_CS_PDS_ADC_0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_CS_PDS_ADC_0, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC20 */

	gpio_set_pin_level(UHF_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(UHF_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(UHF_EN, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC21 */

	gpio_set_pin_level(SPI_CS_PDS_ADC_1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_CS_PDS_ADC_1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_CS_PDS_ADC_1, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC22 */

	gpio_set_pin_level(SPI_CS_UHF_ADC,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_CS_UHF_ADC, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_CS_UHF_ADC, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC23 */

	gpio_set_pin_level(DSS_EN_0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(DSS_EN_0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(DSS_EN_0, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC25 */

	gpio_set_pin_level(DSS_EN_1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(DSS_EN_1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(DSS_EN_1, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC28 */

	gpio_set_pin_level(I2C_PSS_IO_ADDR_0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(I2C_PSS_IO_ADDR_0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(I2C_PSS_IO_ADDR_0, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC29 */

	// Set pin direction to input
	gpio_set_pin_direction(PW_STATUS_FLAG_INRAD, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PW_STATUS_FLAG_INRAD,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PW_STATUS_FLAG_INRAD, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC30 */

	gpio_set_pin_level(RWY_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(RWY_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(RWY_EN, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC31 */

	gpio_set_pin_level(MAG_X_DIR,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(MAG_X_DIR, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(MAG_X_DIR, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PD1 */

	gpio_set_pin_level(SUNSENS_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SUNSENS_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SUNSENS_EN, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PD2 */

	gpio_set_pin_level(ANTENNA_RELEASE_0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(ANTENNA_RELEASE_0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(ANTENNA_RELEASE_0, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PD3 */

	gpio_set_pin_level(ANTENNA_RELEASE_1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(ANTENNA_RELEASE_1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(ANTENNA_RELEASE_1, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PD5 */

	gpio_set_pin_function(MAG_T_Y_PWM, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PD7 */

	// Set pin direction to input
	gpio_set_pin_direction(PW_STATUS_FLAG_MUTUAL, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PW_STATUS_FLAG_MUTUAL,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PW_STATUS_FLAG_MUTUAL, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PD10 */

	gpio_set_pin_level(HEATER_ADCS_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(HEATER_ADCS_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(HEATER_ADCS_EN, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PD11 */

	gpio_set_pin_level(HEATER_PWM_1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(HEATER_PWM_1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(HEATER_PWM_1, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PD15 */

	gpio_set_pin_level(HEATER_PWM_0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(HEATER_PWM_0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(HEATER_PWM_0, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PD16 */

	gpio_set_pin_level(WATCHDOG_FEED,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(WATCHDOG_FEED, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(WATCHDOG_FEED, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PD17 */

	gpio_set_pin_level(RESET_MUTUAL,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(RESET_MUTUAL, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(RESET_MUTUAL, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PD29 */

	// Set pin direction to input
	gpio_set_pin_direction(WATCHDOG_WARN, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(WATCHDOG_WARN,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(WATCHDOG_WARN, GPIO_PIN_FUNCTION_OFF);

	FLASH_0_init();

	QSPI_INSTANCE_init();

	CALENDAR_0_init();

	SPI_0_init();

	I2C_0_init();

	UART_MUTUAL_init();

	USART_UHF_init();

	UART_DSS_init();

	UART_PL_init();

	UART_ADCS_init();

	CAN_0_init();
}
