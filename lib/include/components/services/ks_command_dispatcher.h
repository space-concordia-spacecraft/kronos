#pragma once

#include "ks_component_active.h"

namespace kronos {
    class ComponentCommandDispatcher : public ComponentActive  {
    public:
        KsCmdResult ProcessEvent(const EventMessage& message) override;

    private:
    };
}