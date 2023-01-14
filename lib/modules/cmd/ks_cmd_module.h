#pragma once

#include "ks_module.h"

namespace kronos {

    class CmdModule : public Module {

    public:
        CmdModule();

        void Init() const override;

    };

}