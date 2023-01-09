#include "ks_gpio.h"

Gpio::Gpio(uint32_t pin, KS_GPIO_DIRECTION direction, KS_GPIO_PULL_MODE pullMode, bool level) : m_Pin(pin) {
    // TODO: Take care of direction and pull mode
}

KS_RESULT Toggle() {
    return ks_success;
}

KsResult SetDirection(KS_GPIO_DIRECTION direction) {
    return KS_SUCCESS;
}

KsResult SetPullMode(KS_GPIO_PULL_MODE pullMode) {
    return KS_SUCCESS;
}

KsResult SetInitialLevel(bool level) {
    return KS_SUCCESS;
}

KS_GPIO_DIRECTION Gpio::GetDirection() {
    return dir_off;
}

KS_GPIO_PULL_MODE Gpio::GetPullMode() {
    return pull_off;
}

bool GetInitialLevel() {
    return false;
}
