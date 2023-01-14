#pragma once

#include "ks_module.h"

namespace kronos {

    class TlmModule : Module {

    public:
        TlmModule();

        void Init() const override;

    };

}