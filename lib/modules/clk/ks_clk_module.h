#pragma once

#include "ks_module.h"
#include "ks_framework.h"

namespace kronos {

    class ClockModule : public Module {

    public:
        ClockModule();
        void Init() const override;
    };

}