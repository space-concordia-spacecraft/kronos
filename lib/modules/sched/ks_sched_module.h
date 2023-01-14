#pragma once

#include "ks_module.h"

namespace kronos {

    class SchedModule : public Module {

    public:
        SchedModule();

        void Init() const override;

    };

}