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

struct spi_m_sync_descriptor SPI;
struct can_async_descriptor  CAN_P_N;

struct dac_sync_descriptor DAC_DINY;

struct mci_sync_desc IO_BUS;

struct usart_sync_descriptor UART_MUTUAL;

struct usart_sync_descriptor UART_UHF;

struct usart_sync_descriptor UART_DSS;

struct usart_sync_descriptor UART_PL;

struct usart_sync_descriptor UART_ADCS;

void DAC_DINY_PORT_init(void)
{

    gpio_set_pin_function(PD0, GPIO_PIN_FUNCTION_OFF);
}

void DAC_DINY_CLOCK_init(void)
{

    _pmc_enable_periph_clock(ID_DACC);
}

void DAC_DINY_init(void)
{
    DAC_DINY_CLOCK_init();
    dac_sync_init(&DAC_DINY, DACC);
    DAC_DINY_PORT_init();
}

void IO_BUS_PORT_init(void)
{

    gpio_set_pin_direction(PA28,
            // <y> Pin direction
            // <id> pad_direction
            // <GPIO_DIRECTION_OFF"> Off
            // <GPIO_DIRECTION_IN"> In
            // <GPIO_DIRECTION_OUT"> Out
                           GPIO_DIRECTION_OUT);

    gpio_set_pin_level(PA28,
            // <y> Initial level
            // <id> pad_initial_level
            // <false"> Low
            // <true"> High
                       false);

    gpio_set_pin_pull_mode(PA28,
            // <y> Pull configuration
            // <id> pad_pull_config
            // <GPIO_PULL_OFF"> Off
            // <GPIO_PULL_UP"> Pull-up
            // <GPIO_PULL_DOWN"> Pull-down
                           GPIO_PULL_OFF);

    gpio_set_pin_function(PA28,
            // <y> Pin function
            // <id> pad_function
            // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
            // <MUX_PA28C_HSMCI_MCCDA"> Auto
            // <GPIO_PIN_FUNCTION_OFF"> Off
            // <GPIO_PIN_FUNCTION_A"> A
            // <GPIO_PIN_FUNCTION_B"> B
            // <GPIO_PIN_FUNCTION_C"> C
            // <GPIO_PIN_FUNCTION_D"> D
                          MUX_PA28C_HSMCI_MCCDA);

    gpio_set_pin_direction(PA25,
            // <y> Pin direction
            // <id> pad_direction
            // <GPIO_DIRECTION_OFF"> Off
            // <GPIO_DIRECTION_IN"> In
            // <GPIO_DIRECTION_OUT"> Out
                           GPIO_DIRECTION_OUT);

    gpio_set_pin_level(PA25,
            // <y> Initial level
            // <id> pad_initial_level
            // <false"> Low
            // <true"> High
                       false);

    gpio_set_pin_pull_mode(PA25,
            // <y> Pull configuration
            // <id> pad_pull_config
            // <GPIO_PULL_OFF"> Off
            // <GPIO_PULL_UP"> Pull-up
            // <GPIO_PULL_DOWN"> Pull-down
                           GPIO_PULL_OFF);

    gpio_set_pin_function(PA25,
            // <y> Pin function
            // <id> pad_function
            // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
            // <MUX_PA25D_HSMCI_MCCK"> Auto
            // <GPIO_PIN_FUNCTION_OFF"> Off
            // <GPIO_PIN_FUNCTION_A"> A
            // <GPIO_PIN_FUNCTION_B"> B
            // <GPIO_PIN_FUNCTION_C"> C
            // <GPIO_PIN_FUNCTION_D"> D
                          MUX_PA25D_HSMCI_MCCK);

    gpio_set_pin_direction(PA30,
            // <y> Pin direction
            // <id> pad_direction
            // <GPIO_DIRECTION_OFF"> Off
            // <GPIO_DIRECTION_IN"> In
            // <GPIO_DIRECTION_OUT"> Out
                           GPIO_DIRECTION_OUT);

    gpio_set_pin_level(PA30,
            // <y> Initial level
            // <id> pad_initial_level
            // <false"> Low
            // <true"> High
                       false);

    gpio_set_pin_pull_mode(PA30,
            // <y> Pull configuration
            // <id> pad_pull_config
            // <GPIO_PULL_OFF"> Off
            // <GPIO_PULL_UP"> Pull-up
            // <GPIO_PULL_DOWN"> Pull-down
                           GPIO_PULL_OFF);

    gpio_set_pin_function(PA30,
            // <y> Pin function
            // <id> pad_function
            // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
            // <MUX_PA30C_HSMCI_MCDA0"> Auto
            // <GPIO_PIN_FUNCTION_OFF"> Off
            // <GPIO_PIN_FUNCTION_A"> A
            // <GPIO_PIN_FUNCTION_B"> B
            // <GPIO_PIN_FUNCTION_C"> C
            // <GPIO_PIN_FUNCTION_D"> D
                          MUX_PA30C_HSMCI_MCDA0);

    gpio_set_pin_direction(PA31,
            // <y> Pin direction
            // <id> pad_direction
            // <GPIO_DIRECTION_OFF"> Off
            // <GPIO_DIRECTION_IN"> In
            // <GPIO_DIRECTION_OUT"> Out
                           GPIO_DIRECTION_OUT);

    gpio_set_pin_level(PA31,
            // <y> Initial level
            // <id> pad_initial_level
            // <false"> Low
            // <true"> High
                       false);

    gpio_set_pin_pull_mode(PA31,
            // <y> Pull configuration
            // <id> pad_pull_config
            // <GPIO_PULL_OFF"> Off
            // <GPIO_PULL_UP"> Pull-up
            // <GPIO_PULL_DOWN"> Pull-down
                           GPIO_PULL_OFF);

    gpio_set_pin_function(PA31,
            // <y> Pin function
            // <id> pad_function
            // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
            // <MUX_PA31C_HSMCI_MCDA1"> Auto
            // <GPIO_PIN_FUNCTION_OFF"> Off
            // <GPIO_PIN_FUNCTION_A"> A
            // <GPIO_PIN_FUNCTION_B"> B
            // <GPIO_PIN_FUNCTION_C"> C
            // <GPIO_PIN_FUNCTION_D"> D
                          MUX_PA31C_HSMCI_MCDA1);

    gpio_set_pin_direction(PA26,
            // <y> Pin direction
            // <id> pad_direction
            // <GPIO_DIRECTION_OFF"> Off
            // <GPIO_DIRECTION_IN"> In
            // <GPIO_DIRECTION_OUT"> Out
                           GPIO_DIRECTION_OUT);

    gpio_set_pin_level(PA26,
            // <y> Initial level
            // <id> pad_initial_level
            // <false"> Low
            // <true"> High
                       false);

    gpio_set_pin_pull_mode(PA26,
            // <y> Pull configuration
            // <id> pad_pull_config
            // <GPIO_PULL_OFF"> Off
            // <GPIO_PULL_UP"> Pull-up
            // <GPIO_PULL_DOWN"> Pull-down
                           GPIO_PULL_OFF);

    gpio_set_pin_function(PA26,
            // <y> Pin function
            // <id> pad_function
            // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
            // <MUX_PA26C_HSMCI_MCDA2"> Auto
            // <GPIO_PIN_FUNCTION_OFF"> Off
            // <GPIO_PIN_FUNCTION_A"> A
            // <GPIO_PIN_FUNCTION_B"> B
            // <GPIO_PIN_FUNCTION_C"> C
            // <GPIO_PIN_FUNCTION_D"> D
                          MUX_PA26C_HSMCI_MCDA2);

    gpio_set_pin_direction(PA27,
            // <y> Pin direction
            // <id> pad_direction
            // <GPIO_DIRECTION_OFF"> Off
            // <GPIO_DIRECTION_IN"> In
            // <GPIO_DIRECTION_OUT"> Out
                           GPIO_DIRECTION_OUT);

    gpio_set_pin_level(PA27,
            // <y> Initial level
            // <id> pad_initial_level
            // <false"> Low
            // <true"> High
                       false);

    gpio_set_pin_pull_mode(PA27,
            // <y> Pull configuration
            // <id> pad_pull_config
            // <GPIO_PULL_OFF"> Off
            // <GPIO_PULL_UP"> Pull-up
            // <GPIO_PULL_DOWN"> Pull-down
                           GPIO_PULL_OFF);

    gpio_set_pin_function(PA27,
            // <y> Pin function
            // <id> pad_function
            // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
            // <MUX_PA27C_HSMCI_MCDA3"> Auto
            // <GPIO_PIN_FUNCTION_OFF"> Off
            // <GPIO_PIN_FUNCTION_A"> A
            // <GPIO_PIN_FUNCTION_B"> B
            // <GPIO_PIN_FUNCTION_C"> C
            // <GPIO_PIN_FUNCTION_D"> D
                          MUX_PA27C_HSMCI_MCDA3);
}

void IO_BUS_CLOCK_init(void)
{
    _pmc_enable_periph_clock(ID_HSMCI);
}

void IO_BUS_init(void)
{
    IO_BUS_CLOCK_init();
    mci_sync_init(&IO_BUS, HSMCI);
    IO_BUS_PORT_init();
}

void SPI_PORT_init(void)
{

    gpio_set_pin_function(PC26, MUX_PC26C_SPI1_MISO);

    gpio_set_pin_function(PC27, MUX_PC27C_SPI1_MOSI);

    gpio_set_pin_function(PC24, MUX_PC24C_SPI1_SPCK);
}

void SPI_CLOCK_init(void)
{
    _pmc_enable_periph_clock(ID_SPI1);
}

void SPI_init(void)
{
    SPI_CLOCK_init();
    spi_m_sync_set_func_ptr(&SPI, _spi_get_spi_m_sync());
    spi_m_sync_init(&SPI, SPI1);
    SPI_PORT_init();
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
    usart_sync_init(&UART_MUTUAL, UART0, _uart_get_usart_sync());
    UART_MUTUAL_PORT_init();
}

void UART_UHF_PORT_init(void)
{

    gpio_set_pin_function(PA5, MUX_PA5C_UART1_URXD1);

    gpio_set_pin_function(PA6, MUX_PA6C_UART1_UTXD1);
}

void UART_UHF_CLOCK_init(void)
{
    _pmc_enable_periph_clock(ID_UART1);
}

void UART_UHF_init(void)
{
    UART_UHF_CLOCK_init();
    usart_sync_init(&UART_UHF, UART1, _uart_get_usart_sync());
    UART_UHF_PORT_init();
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
    usart_sync_init(&UART_DSS, UART2, _uart_get_usart_sync());
    UART_DSS_PORT_init();
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
    usart_sync_init(&UART_PL, UART3, _uart_get_usart_sync());
    UART_PL_PORT_init();
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
    usart_sync_init(&UART_ADCS, UART4, _uart_get_usart_sync());
    UART_ADCS_PORT_init();
}

/**
 * \brief MCAN Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void CAN_P_N_CLOCK_init()
{
    _pmc_enable_periph_clock(ID_MCAN0);
}

/**
 * \brief MCAN pinmux initialization function
 *
 * Set each required pin to MCAN functionality
 */
void CAN_P_N_PORT_init(void)
{

    gpio_set_pin_function(PB3, MUX_PB3A_MCAN0_CANRX0);

    gpio_set_pin_function(PB2, MUX_PB2A_MCAN0_CANTX0);
}
/**
 * \brief CAN initialization function
 *
 * Enables CAN peripheral, clocks and initializes CAN driver
 */
void CAN_P_N_init(void)
{
    CAN_P_N_CLOCK_init();
    CAN_P_N_PORT_init();
    can_async_init(&CAN_P_N, MCAN0);
}

void system_init(void)
{
    init_mcu();

    /* Disable Watchdog */
    hri_wdt_set_MR_WDDIS_bit(WDT);

    DAC_DINY_init();

    IO_BUS_init();

    SPI_init();

    UART_MUTUAL_init();

    UART_UHF_init();

    UART_DSS_init();

    UART_PL_init();

    UART_ADCS_init();

    CAN_P_N_init();
}
