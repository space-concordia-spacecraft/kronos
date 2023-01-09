#pragma once

#include <stdint.h>
#include <ks_error_codes.h>

#include "hal_gpio.h"

namespace kronos {
    // MAKE SURE TO REDEFINE THE TYPES TO WHATEVER YOU'RE USING
    typedef gpio_pull_mode KsGpioPullMode;
    typedef gpio_direction KsGpioDirection;

    class Gpio {
    public:
        static inline void Toggle(uint8_t pin);
        static inline void SetDirection(uint8_t pin, KsGpioDirection direction);
        static inline void SetPullMode(uint8_t pin, KsGpioPullMode pullMode);
        static inline void SetLevel(uint8_t pin, bool level);
        static inline bool GetLevel(uint8_t pin);
    };
}
