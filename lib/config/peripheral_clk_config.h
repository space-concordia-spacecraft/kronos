/* Auto-generated config file peripheral_clk_config.h */
#ifndef PERIPHERAL_CLK_CONFIG_H
#define PERIPHERAL_CLK_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> DAC Clock Settings
// <y> DAC Clock source
// <CONF_SRC_MCK"> Master Clock (MCK)
// <i> This defines the clock source for the DAC
// <id> dac_clock_source
#ifndef CONF_DACC_SRC
#define CONF_DACC_SRC CONF_SRC_MCK
#endif
// </h>

/**
 * \def DAC FREQUENCY
 * \brief DAC's Clock frequency
 */
#ifndef CONF_DACC_FREQUENCY
#define CONF_DACC_FREQUENCY 150000000
#endif

// <h> HSMCI Clock Settings
// <y> HSMCI Clock source
// <CONF_SRC_MCK"> Master Clock (MCK)
// <i> This defines the clock source for the HSMCI
// <id> hsmci_clock_source
#ifndef CONF_HSMCI_SRC
#define CONF_HSMCI_SRC CONF_SRC_MCK
#endif
// </h>

/**
 * \def HSMCI FREQUENCY
 * \brief HSMCI's Clock frequency
 */
#ifndef CONF_HSMCI_FREQUENCY
#define CONF_HSMCI_FREQUENCY 150000000
#endif

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
