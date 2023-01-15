#pragma once

namespace kronos {
    class Gpio {
    public:
        static void Toggle(uint8_t pin);
        static void SetDirection(uint8_t pin, KsGpioDirection direction);
        static void SetPullMode(uint8_t pin, KsGpioPullMode pullMode);
        static void SetLevel(uint8_t pin, bool level);
        static bool GetLevel(uint8_t pin);
    };
}
