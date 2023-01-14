#pragma once

#include "ks_module.h"

namespace kronos {

    class LogModule : public Module {

    public:
        LogModule();

        void Init() const override;

    };

}