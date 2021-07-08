#pragma once

#include "ks_component_queued.h"

namespace kronos {

    class ComponentLogger : public ComponentQueued {
    public:
        void ProcessCommand(const CommandMessage& message) override;

    private:

    };

}