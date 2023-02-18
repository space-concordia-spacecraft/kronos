/* Auto-generated config file hpl_rtc_config.h */
#ifndef HPL_RTC_CONFIG_H
#define HPL_RTC_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Basic settings

#ifndef CONF_RTC_ENABLE
#define CONF_RTC_ENABLE 1
#endif

/* 24-hour Mode */
#ifndef RTC_HRMOD
#define RTC_HRMOD 0
#endif

/* Gregorian calendar */
#ifndef RTC_PERSIAN
#define RTC_PERSIAN 0
#endif

// <e> Waveform Generation Configuration
// <id> rtc_waveform_enable
#ifndef RTC_WAVEFORM_ENABLE
#define RTC_WAVEFORM_ENABLE 0x0
#endif

// <o> RTCOUT0 Output Source Selection
// <0x0=> No waveform
// <0x1=> 1 Hz square wave
// <0x2=> 32 Hz square wave
// <0x3=> 64 Hz square wave
// <0x4=> 512 Hz square wave
// <0x5=> Output toggles when alarm flag rises
// <0x6=> Output is a copy of the alarm flag
// <0x7=> Duty cycle programmable pulse
// <i> These bit define RTCOUT0 output source.
// <id> rtc_out0
#ifndef RTC_OUT0
#define RTC_OUT0 0x0
#endif

// <o> RTCOUT1 Output Source Selection
// <0x0=> No waveform
// <0x1=> 1 Hz square wave
// <0x2=> 32 Hz square wave
// <0x3=> 64 Hz square wave
// <0x4=> 512 Hz square wave
// <0x5=> Output toggles when alarm flag rises
// <0x6=> Output is a copy of the alarm flag
// <0x7=> Duty cycle programmable pulse
// <i> These bit define RTCOUT1 output source.
// <id> rtc_out1
#ifndef RTC_OUT1
#define RTC_OUT1 0x0
#endif
// <o> High Duration of the Output Pulse
// <0x0=> 31.2ms
// <0x1=> 15.6ms
// <0x2=> 3.91ms
// <0x3=> 976us
// <0x4=> 488us
// <0x5=> 122us
// <0x6=> 30.5us
// <0x7=> 15.2us
// <i> These bit define high duration of the output pulse.
// <id> rtc_thigh
#ifndef RTC_THIGH
#define RTC_THIGH 0x0
#endif

// <o> Period of the Output Pulse
// <0x0=> 1s
// <0x1=> 500ms
// <0x2=> 250ms
// <0x3=> 125ms
// <i> These bit define period of the output pulse.
// <id> rtc_tperiod
#ifndef RTC_TPERIOD
#define RTC_TPERIOD 0x0
#endif
// </e>
// </h>

// <e> Advanced Configuration
// <id> rtc_advanced_settings
#ifndef RTC_ADVANCED
#define RTC_ADVANCED 0
#endif

// <o> NEGative PPM Correction
// <0x0=> Positive correction (the divider will be slightly higher than 32768).
// <0x1=> Negative correction (the divider will be slightly lower than 32768).
// <i> NEGPPM must be cleared to correct a crystal slower than 32.768 kHz.
// <id> rtc_negppm
#ifndef RTC_NEGPPM
#define RTC_NEGPPM 0x0
#endif

// <o> Slow Clock Correction <0-127>
// <i> If this value is 0, it shows no correction.
// <i> Else, The slow clock will be corrected according to the formula given in HIGHPPM description.
// <id> rtc_correction
#ifndef RTC_CORRECTION
#define RTC_CORRECTION 0x0
#endif

// <o> HIGH PPM Correction
// <0x0=> Lower range ppm correction with accurate correction.
// <0x1=> Higher range ppm correction with accurate correction.
// <i> If the absolute value of the correction to be applied is lower than 30 ppm, it is recommended to clear HIGHPPM.
// <i> HIGHPPM set to 1 is recommended for 30 ppm correction and above.
// <id> rtc_highppm
#ifndef RTC_HIGHPPM
#define RTC_HIGHPPM 0x0
#endif

// </e>

// <<< end of configuration section >>>

#endif // HPL_RTC_CONFIG_H
