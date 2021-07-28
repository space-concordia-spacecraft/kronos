#pragma once
#include "ks_component_active.h"

using kronos::ComponentActive;

namespace orthus {
    struct CameraConfig {

    };

    class ComponentCamera : ComponentActive {

    private:
        CameraConfig m_CameraConfig;
    };
}