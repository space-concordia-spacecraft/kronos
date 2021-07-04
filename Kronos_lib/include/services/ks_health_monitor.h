#pragma once

#include "ks_component_queued.h"

namespace kronos {
    class HealthMonitor : public ComponentQueued {
    public:
        void ProcessCommand(const CommandMessage& message) override;
        void RegisterActiveComponent(String & name);
    private:

    };
}