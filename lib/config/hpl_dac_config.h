/* Auto-generated config file hpl_dac_config.h */
#ifndef HPL_DAC_CONFIG_H
#define HPL_DAC_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Basic configuration

// <o> Channel0 Mode Select
// <0x0=> Free-running mode
// <0x1=> Trigger mode
// <0x2=> Max speed
// <i> Indicates channel0 run mode
// <id> dac_channel0_mode
#ifndef CONF_DAC_CHANNEL0_MODE
#define CONF_DAC_CHANNEL0_MODE 0x0
#endif

// <o> Channel0 Trigger Selection
// <0x0=> DATRG
// <0x1=> TC0 output
// <0x2=> TC1 output
// <0x3=> TC2 output
// <0x4=> PWM0 event 0
// <0x5=> PWM0 event 1
// <0x6=> PWM1 event 0
// <0x7=> PWM1 event 1
// <i> Indicates channel0 trigger selection
// <id> dac_trigger0_selection
#ifndef CONF_DAC_TRGSEL0
#define CONF_DAC_TRGSEL0 0x0
#endif

// <o> Channel0 Over Sampling Ratio
// <0x0=> OSR = 1
// <0x1=> OSR = 2
// <0x2=> OSR = 4
// <0x3=> OSR = 8
// <0x4=> OSR = 16
// <0x5=> OSR = 32
// <i> Indicates channel0 Over Sampling Ratio
// <id> dac_osr0
#ifndef CONF_DAC_OSR0
#define CONF_DAC_OSR0 0x0
#endif

// <o> Channel1 Mode Select
// <0x0=> Free-running mode
// <0x1=> Trigger mode
// <0x2=> Max speed
// <i> Indicates channel1 run mode
// <id> dac_channel1_mode
#ifndef CONF_DAC_CHANNEL1_MODE
#define CONF_DAC_CHANNEL1_MODE 0x0
#endif

// <o> Channel1 Trigger Selection
// <0x0=> DATRG
// <0x1=> TC0 output
// <0x2=> TC1 output
// <0x3=> TC2 output
// <0x4=> PWM0 event 0
// <0x5=> PWM0 event 1
// <0x6=> PWM1 event 0
// <0x7=> PWM1 event 1
// <i> Indicates channel1 trigger selection
// <id> dac_trigger1_selection
#ifndef CONF_DAC_TRGSEL1
#define CONF_DAC_TRGSEL1 0x0
#endif

// <o> Channel1 Over Sampling Ratio
// <0x0=> OSR = 1
// <0x1=> OSR = 2
// <0x2=> OSR = 4
// <0x3=> OSR = 8
// <0x4=> OSR = 16
// <0x5=> OSR = 32
// <i> Indicates channel1 Over Sampling Ratio
// <id> dac_osr1
#ifndef CONF_DAC_OSR1
#define CONF_DAC_OSR1 0x0
#endif

// </h>

// <e> Advanced configuration
// <id> dac_advanced
#ifndef DAC_ADVANCED_CONFIG
#define DAC_ADVANCED_CONFIG 0
#endif

// <o> PRESCALER <1-15>

// <i> Peripheral Clock to DAC Clock Ratio
// <id> dac_clock_ratio
#ifndef CONF_DAC_CLOCK_PRESCALER
#define CONF_DAC_CLOCK_PRESCALER 1
#endif

// <q> Enable Differential Mode
// <i> Indicates whether the DAC will running in Differential Mode
// <id> dac_diff
#ifndef CONF_DAC_DIFF
#define CONF_DAC_DIFF 0
#endif

// </e>

// <<< end of configuration section >>>

#endif // HPL_DAC_CONFIG_H
