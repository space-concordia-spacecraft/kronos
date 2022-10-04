/* Auto-generated config file hpl_uart_config.h */
#ifndef HPL_UART_CONFIG_H
#define HPL_UART_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

#include <peripheral_clk_config.h>

#ifndef CONF_UART_0_ENABLE
#define CONF_UART_0_ENABLE 1
#endif

// <h> Basic Configuration

// <o> Clock Selection
// <0x0=>Peripheral clock is selected
// <0x1=>PMC programmable clock (PCK4) is selected.
// <i> Clock Selection for UART frame
// <id> uart_usclks
#ifndef CONF_UART_0_USCLKS
#define CONF_UART_0_USCLKS 0x0
#endif

// <o> Frame parity
// <0x0=>Even parity
// <0x1=>Odd parity
// <0x2=>Parity forced to 0
// <0x3=>Parity forced to 1
// <0x4=>No parity
// <i> Parity bit mode for UART frame
// <id> uart_parity
#ifndef CONF_UART_0_PARITY
#define CONF_UART_0_PARITY 0x4
#endif

// <o> Baud rate <1-3000000>
// <i> UART baud rate setting
// <id> uart_baud_rate
#ifndef CONF_UART_0_BAUD
#define CONF_UART_0_BAUD 38400
#endif

// </h>

// <e> Advanced configuration
// <id> uart_advanced
#ifndef CONF_UART_0_ADVANCED_CONFIG
#define CONF_UART_0_ADVANCED_CONFIG 0
#endif

// <o> Channel Mode
// <0=>Normal Mode
// <1=>Automatic Echo
// <2=>Local Loopback
// <3=>Remote Loopback
// <i> Channel mode in UART frame
// <id> uart_channel_mode
#ifndef CONF_UART_0_CHMODE
#define CONF_UART_0_CHMODE 0
#endif

// <q> Receive Line Filter enable
// <i> whether the UART filters the receive line using a three-sample filter
// <id> uart_receive_filter_enable
#ifndef CONF_UART_0_FILTER
#define CONF_UART_0_FILTER 0
#endif

// </e>

// Calculate BAUD register value in UART mode
#ifndef CONF_UART_0_BAUD_CD
#define CONF_UART_0_BAUD_CD ((CONF_UART0_FREQUENCY) / CONF_UART_0_BAUD / 16)
#endif

#include <peripheral_clk_config.h>

#ifndef CONF_UART_1_ENABLE
#define CONF_UART_1_ENABLE 1
#endif

// <h> Basic Configuration

// <o> Clock Selection
// <0x0=>Peripheral clock is selected
// <0x1=>PMC programmable clock (PCK4) is selected.
// <i> Clock Selection for UART frame
// <id> uart_usclks
#ifndef CONF_UART_1_USCLKS
#define CONF_UART_1_USCLKS 0x0
#endif

// <o> Frame parity
// <0x0=>Even parity
// <0x1=>Odd parity
// <0x2=>Parity forced to 0
// <0x3=>Parity forced to 1
// <0x4=>No parity
// <i> Parity bit mode for UART frame
// <id> uart_parity
#ifndef CONF_UART_1_PARITY
#define CONF_UART_1_PARITY 0x4
#endif

// <o> Baud rate <1-3000000>
// <i> UART baud rate setting
// <id> uart_baud_rate
#ifndef CONF_UART_1_BAUD
#define CONF_UART_1_BAUD 38400
#endif

// </h>

// <e> Advanced configuration
// <id> uart_advanced
#ifndef CONF_UART_1_ADVANCED_CONFIG
#define CONF_UART_1_ADVANCED_CONFIG 0
#endif

// <o> Channel Mode
// <0=>Normal Mode
// <1=>Automatic Echo
// <2=>Local Loopback
// <3=>Remote Loopback
// <i> Channel mode in UART frame
// <id> uart_channel_mode
#ifndef CONF_UART_1_CHMODE
#define CONF_UART_1_CHMODE 0
#endif

// <q> Receive Line Filter enable
// <i> whether the UART filters the receive line using a three-sample filter
// <id> uart_receive_filter_enable
#ifndef CONF_UART_1_FILTER
#define CONF_UART_1_FILTER 0
#endif

// </e>

// Calculate BAUD register value in UART mode
#ifndef CONF_UART_1_BAUD_CD
#define CONF_UART_1_BAUD_CD ((CONF_UART1_FREQUENCY) / CONF_UART_1_BAUD / 16)
#endif

#include <peripheral_clk_config.h>

#ifndef CONF_UART_2_ENABLE
#define CONF_UART_2_ENABLE 1
#endif

// <h> Basic Configuration

// <o> Clock Selection
// <0x0=>Peripheral clock is selected
// <0x1=>PMC programmable clock (PCK4) is selected.
// <i> Clock Selection for UART frame
// <id> uart_usclks
#ifndef CONF_UART_2_USCLKS
#define CONF_UART_2_USCLKS 0x0
#endif

// <o> Frame parity
// <0x0=>Even parity
// <0x1=>Odd parity
// <0x2=>Parity forced to 0
// <0x3=>Parity forced to 1
// <0x4=>No parity
// <i> Parity bit mode for UART frame
// <id> uart_parity
#ifndef CONF_UART_2_PARITY
#define CONF_UART_2_PARITY 0x4
#endif

// <o> Baud rate <1-3000000>
// <i> UART baud rate setting
// <id> uart_baud_rate
#ifndef CONF_UART_2_BAUD
#define CONF_UART_2_BAUD 38400
#endif

// </h>

// <e> Advanced configuration
// <id> uart_advanced
#ifndef CONF_UART_2_ADVANCED_CONFIG
#define CONF_UART_2_ADVANCED_CONFIG 0
#endif

// <o> Channel Mode
// <0=>Normal Mode
// <1=>Automatic Echo
// <2=>Local Loopback
// <3=>Remote Loopback
// <i> Channel mode in UART frame
// <id> uart_channel_mode
#ifndef CONF_UART_2_CHMODE
#define CONF_UART_2_CHMODE 0
#endif

// <q> Receive Line Filter enable
// <i> whether the UART filters the receive line using a three-sample filter
// <id> uart_receive_filter_enable
#ifndef CONF_UART_2_FILTER
#define CONF_UART_2_FILTER 0
#endif

// </e>

// Calculate BAUD register value in UART mode
#ifndef CONF_UART_2_BAUD_CD
#define CONF_UART_2_BAUD_CD ((CONF_UART2_FREQUENCY) / CONF_UART_2_BAUD / 16)
#endif

#include <peripheral_clk_config.h>

#ifndef CONF_UART_3_ENABLE
#define CONF_UART_3_ENABLE 1
#endif

// <h> Basic Configuration

// <o> Clock Selection
// <0x0=>Peripheral clock is selected
// <0x1=>PMC programmable clock (PCK4) is selected.
// <i> Clock Selection for UART frame
// <id> uart_usclks
#ifndef CONF_UART_3_USCLKS
#define CONF_UART_3_USCLKS 0x0
#endif

// <o> Frame parity
// <0x0=>Even parity
// <0x1=>Odd parity
// <0x2=>Parity forced to 0
// <0x3=>Parity forced to 1
// <0x4=>No parity
// <i> Parity bit mode for UART frame
// <id> uart_parity
#ifndef CONF_UART_3_PARITY
#define CONF_UART_3_PARITY 0x4
#endif

// <o> Baud rate <1-3000000>
// <i> UART baud rate setting
// <id> uart_baud_rate
#ifndef CONF_UART_3_BAUD
#define CONF_UART_3_BAUD 38400
#endif

// </h>

// <e> Advanced configuration
// <id> uart_advanced
#ifndef CONF_UART_3_ADVANCED_CONFIG
#define CONF_UART_3_ADVANCED_CONFIG 0
#endif

// <o> Channel Mode
// <0=>Normal Mode
// <1=>Automatic Echo
// <2=>Local Loopback
// <3=>Remote Loopback
// <i> Channel mode in UART frame
// <id> uart_channel_mode
#ifndef CONF_UART_3_CHMODE
#define CONF_UART_3_CHMODE 0
#endif

// <q> Receive Line Filter enable
// <i> whether the UART filters the receive line using a three-sample filter
// <id> uart_receive_filter_enable
#ifndef CONF_UART_3_FILTER
#define CONF_UART_3_FILTER 0
#endif

// </e>

// Calculate BAUD register value in UART mode
#ifndef CONF_UART_3_BAUD_CD
#define CONF_UART_3_BAUD_CD ((CONF_UART3_FREQUENCY) / CONF_UART_3_BAUD / 16)
#endif

#include <peripheral_clk_config.h>

#ifndef CONF_UART_4_ENABLE
#define CONF_UART_4_ENABLE 1
#endif

// <h> Basic Configuration

// <o> Clock Selection
// <0x0=>Peripheral clock is selected
// <0x1=>PMC programmable clock (PCK4) is selected.
// <i> Clock Selection for UART frame
// <id> uart_usclks
#ifndef CONF_UART_4_USCLKS
#define CONF_UART_4_USCLKS 0x0
#endif

// <o> Frame parity
// <0x0=>Even parity
// <0x1=>Odd parity
// <0x2=>Parity forced to 0
// <0x3=>Parity forced to 1
// <0x4=>No parity
// <i> Parity bit mode for UART frame
// <id> uart_parity
#ifndef CONF_UART_4_PARITY
#define CONF_UART_4_PARITY 0x4
#endif

// <o> Baud rate <1-3000000>
// <i> UART baud rate setting
// <id> uart_baud_rate
#ifndef CONF_UART_4_BAUD
#define CONF_UART_4_BAUD 38400
#endif

// </h>

// <e> Advanced configuration
// <id> uart_advanced
#ifndef CONF_UART_4_ADVANCED_CONFIG
#define CONF_UART_4_ADVANCED_CONFIG 0
#endif

// <o> Channel Mode
// <0=>Normal Mode
// <1=>Automatic Echo
// <2=>Local Loopback
// <3=>Remote Loopback
// <i> Channel mode in UART frame
// <id> uart_channel_mode
#ifndef CONF_UART_4_CHMODE
#define CONF_UART_4_CHMODE 0
#endif

// <q> Receive Line Filter enable
// <i> whether the UART filters the receive line using a three-sample filter
// <id> uart_receive_filter_enable
#ifndef CONF_UART_4_FILTER
#define CONF_UART_4_FILTER 0
#endif

// </e>

// Calculate BAUD register value in UART mode
#ifndef CONF_UART_4_BAUD_CD
#define CONF_UART_4_BAUD_CD ((CONF_UART4_FREQUENCY) / CONF_UART_4_BAUD / 16)
#endif

// <<< end of configuration section >>>

#endif // HPL_UART_CONFIG_H
