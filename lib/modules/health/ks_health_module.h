#pragma once

#include "ks_module.h"

namespace kronos {

    class HealthModule : public Module {

    public:
        HealthModule();

        void Init() const override;

    };

}