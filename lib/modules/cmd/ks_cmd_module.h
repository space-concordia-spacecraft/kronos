#pragma once

#include "ks_module.h"

#include "ks_command_dispatcher.h"
#include "ks_command_listener.h"
#include "ks_command_scheduler.h"

namespace kronos {

    class CmdModule : public Module {

    public:
        CmdModule();

        void Init() const override;

    };

}