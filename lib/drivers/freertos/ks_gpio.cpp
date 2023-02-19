#include "ks_gpio.h"

namespace kronos {

    void Gpio::Toggle(const uint8_t pin) {}

    void Gpio::SetDirection(const uint8_t pin, KsGpioDirection direction) {}

    void Gpio::SetPullMode(const uint8_t pin, KsGpioPullMode pullMode) {}

    void Gpio::SetLevel(const uint8_t pin, bool level) {}

    bool Gpio::GetLevel(const uint8_t pin) {}
}

