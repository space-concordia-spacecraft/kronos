#pragma once

#include "ks_module.h"

namespace kronos {

    class FsModule : public Module {

    public:
        FsModule();

        void Init() const override;

    };

}