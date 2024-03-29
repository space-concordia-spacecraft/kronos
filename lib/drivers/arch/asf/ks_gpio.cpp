#include "ks_gpio.h"

namespace kronos {

    void Gpio::Toggle(const uint8_t pin) {
        gpio_toggle_pin_level(pin);
    }

    void Gpio::SetDirection(const uint8_t pin, KsGpioDirection direction) {
        gpio_set_pin_direction(pin, direction);
    }

    void Gpio::SetPullMode(const uint8_t pin, KsGpioPullMode pullMode) {
        gpio_set_pin_pull_mode(pin, pullMode);
    }

    void Gpio::SetLevel(const uint8_t pin, bool level) {
        gpio_set_pin_level(pin, level);
    }

    bool Gpio::GetLevel(const uint8_t pin) {
        return gpio_get_pin_level(pin);
    }
}

