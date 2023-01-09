#pragma once

enum KS_GPIO_DIRECTION {
    dir_off,
    dir_in,
    dir_out
};

enum KS_GPIO_PULL_MODE {
    pull_off,
    pull_up,
    pull_down
};

namespace kronos {
    class Gpio {
    public:
        Gpio(
            uint32_t pin,
            KS_GPIO_DIRECTION direction = dir_off,
            KS_GPIO_PULL_MODE pullMode = pull_off,
            bool level = false
        );

        KS_RESULT Toggle();

        KsResult SetDirection(KS_GPIO_DIRECTION direction);
        KsResult SetPullMode(KS_GPIO_PULL_MODE pullMode);
        KsResult SetInitialLevel(bool level);

        KS_GPIO_DIRECTION GetDirection();
        KS_GPIO_PULL_MODE GetPullMode();
        bool GetInitialLevel();

    private:
        uint32_t m_Pin;
    };
}
