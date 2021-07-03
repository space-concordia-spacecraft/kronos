#pragma once

#include "ks_component.h"

namespace kronos {

    class PassiveComponent : private Component {

    public:
        PassiveComponent();
        ~PassiveComponent();

        virtual void onReceive(int opcode, void * data) override;
    };
}

