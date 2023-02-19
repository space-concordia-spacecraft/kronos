#pragma once

#include "ks_component_queued.h"

namespace kronos {

    class LedBlinker : public ComponentQueued {
    public:
        explicit LedBlinker(const std::string& name);

        KsResultType Init() override;

    private:
        uint32_t m_Toggles = 0;

        void ProcessEvent(const EventMessage& message) override;
        void ToggleLed();
    };

}