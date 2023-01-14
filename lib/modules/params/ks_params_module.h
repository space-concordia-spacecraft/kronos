#pragma once

#include "ks_module.h"

namespace kronos {

    class ParamsModule : public Module {

    public:
        ParamsModule();

        void Init() const override;

    };

}