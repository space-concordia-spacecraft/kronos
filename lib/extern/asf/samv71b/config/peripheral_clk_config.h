/* Auto-generated config file peripheral_clk_config.h */
#ifndef PERIPHERAL_CLK_CONFIG_H
#define PERIPHERAL_CLK_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

/**
 * \def CONF_HCLK_FREQUENCY
 * \brief HCLK's Clock frequency
 */
#ifndef CONF_HCLK_FREQUENCY
#define CONF_HCLK_FREQUENCY 300000000
#endif

/**
 * \def CONF_FCLK_FREQUENCY
 * \brief FCLK's Clock frequency
 */
#ifndef CONF_FCLK_FREQUENCY
#define CONF_FCLK_FREQUENCY 300000000
#endif

/**
 * \def CONF_CPU_FREQUENCY
 * \brief CPU's Clock frequency
 */
#ifndef CONF_CPU_FREQUENCY
#define CONF_CPU_FREQUENCY 300000000
#endif

/**
 * \def CONF_SLCK_FREQUENCY
 * \brief Slow Clock frequency
 */
#define CONF_SLCK_FREQUENCY 32000

/**
 * \def CONF_MCK_FREQUENCY
 * \brief Master Clock frequency
 */
#define CONF_MCK_FREQUENCY 150000000

/**
 * \def CONF_PCK6_FREQUENCY
 * \brief Programmable Clock Controller 6 frequency
 */
#define CONF_PCK6_FREQUENCY 12000000

// <h> QSPI Clock Settings
// <y> QSPI Clock source
// <CONF_SRC_MCK"> Master Clock (MCK)
// <i> This defines the clock source for the QSPI
// <id> qspi_clock_source
#ifndef CONF_QSPI_SRC
#define CONF_QSPI_SRC CONF_SRC_MCK
#endif
// </h>

/**
 * \def QSPI FREQUENCY
 * \brief QSPI's Clock frequency
 */
#ifndef CONF_QSPI_FREQUENCY
#define CONF_QSPI_FREQUENCY 150000000
#endif

// <o> RTC Clock source
// <0=> SLCK for Peripheral
// <i> This defines the clock source for the RTC
// <id> rtc_clock_source
#ifndef CONF_RTC_SRC
#define CONF_RTC_SRC 0
#endif

/**
 * \def CONF_CLK_RTC_FREQUENCY
 * \brief RTC's Clock frequency
 */
#ifndef CONF_CLK_RTC_FREQUENCY
#define CONF_CLK_RTC_FREQUENCY 32000
#endif

// <h> SPI1 Clock Settings
// <y> SPI1 Clock source
// <CONF_SRC_MCK"> Master Clock (MCK)
// <i> This defines the clock source for the SPI1
// <id> spi_clock_source
#ifndef CONF_SPI1_SRC
#define CONF_SPI1_SRC CONF_SRC_MCK
#endif
// </h>

/**
 * \def SPI1 FREQUENCY
 * \brief SPI1's Clock frequency
 */
#ifndef CONF_SPI1_FREQUENCY
#define CONF_SPI1_FREQUENCY 150000000
#endif

// <h> TWIHS Clock Settings
// <y> TWIHS Clock source
// <CONF_SRC_MCK"> Master Clock (MCK)
// <i> This defines the clock source for the TWIHS
// <id> twihs_clock_source
#ifndef CONF_TWIHS0_SRC
#define CONF_TWIHS0_SRC CONF_SRC_MCK
#endif
// </h>

/**
 * \def TWIHS FREQUENCY
 * \brief TWIHS's Clock frequency
 */
#ifndef CONF_TWIHS0_FREQUENCY
#define CONF_TWIHS0_FREQUENCY 150000000
#endif

// <h> UART Clock Settings
// <o> UART Clock source
// <0=> Master Clock (MCK)
// <1=> Programmable Clock Controller 4 (PMC_PCK4)
// <i> This defines the clock source for the UART
// <id> uart_clock_source
#ifndef CONF_UART0_CK_SRC
#define CONF_UART0_CK_SRC 0
#endif
// </h>

/**
 * \def UART FREQUENCY
 * \brief UART's Clock frequency
 */
#ifndef CONF_UART0_FREQUENCY
#define CONF_UART0_FREQUENCY 150000000
#endif

// <h> UART Clock Settings
// <o> UART Clock source
// <0=> Master Clock (MCK)
// <1=> Programmable Clock Controller 4 (PMC_PCK4)
// <i> This defines the clock source for the UART
// <id> uart_clock_source
#ifndef CONF_UART1_CK_SRC
#define CONF_UART1_CK_SRC 0
#endif
// </h>

/**
 * \def UART FREQUENCY
 * \brief UART's Clock frequency
 */
#ifndef CONF_UART1_FREQUENCY
#define CONF_UART1_FREQUENCY 150000000
#endif

// <h> UART Clock Settings
// <o> UART Clock source
// <0=> Master Clock (MCK)
// <1=> Programmable Clock Controller 4 (PMC_PCK4)
// <i> This defines the clock source for the UART
// <id> uart_clock_source
#ifndef CONF_UART2_CK_SRC
#define CONF_UART2_CK_SRC 0
#endif
// </h>

/**
 * \def UART FREQUENCY
 * \brief UART's Clock frequency
 */
#ifndef CONF_UART2_FREQUENCY
#define CONF_UART2_FREQUENCY 150000000
#endif

// <h> UART Clock Settings
// <o> UART Clock source
// <0=> Master Clock (MCK)
// <1=> Programmable Clock Controller 4 (PMC_PCK4)
// <i> This defines the clock source for the UART
// <id> uart_clock_source
#ifndef CONF_UART3_CK_SRC
#define CONF_UART3_CK_SRC 0
#endif
// </h>

/**
 * \def UART FREQUENCY
 * \brief UART's Clock frequency
 */
#ifndef CONF_UART3_FREQUENCY
#define CONF_UART3_FREQUENCY 150000000
#endif

// <h> UART Clock Settings
// <o> UART Clock source
// <0=> Master Clock (MCK)
// <1=> Programmable Clock Controller 4 (PMC_PCK4)
// <i> This defines the clock source for the UART
// <id> uart_clock_source
#ifndef CONF_UART4_CK_SRC
#define CONF_UART4_CK_SRC 0
#endif
// </h>

/**
 * \def UART FREQUENCY
 * \brief UART's Clock frequency
 */
#ifndef CONF_UART4_FREQUENCY
#define CONF_UART4_FREQUENCY 150000000
#endif

// <h> MCAN Clock Settings
// <y> MCAN Clock source
// <CONF_SRC_PCK5"> Programmable Clock Controller 5 (PMC_PCK5)
// <i> This defines the clock source for the MCAN
// <id> mcan_clock_source
#ifndef CONF_MCAN0_SRC
#define CONF_MCAN0_SRC CONF_SRC_PCK5
#endif
// </h>

/**
 * \def MCAN FREQUENCY
 * \brief MCAN's Clock frequency
 */
#ifndef CONF_MCAN0_FREQUENCY
#define CONF_MCAN0_FREQUENCY 12000000
#endif

// <<< end of configuration section >>>

#endif // PERIPHERAL_CLK_CONFIG_H
