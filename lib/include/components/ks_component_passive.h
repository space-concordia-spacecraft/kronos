#pragma once

// Microchip ASF
#include "asf.h"

// Kronos includes
#include "ks_vector.h"
#include "ks_string.h"
#include "ks_opcodes.h"

#include "ks_component_base.h"

namespace kronos {

    class ComponentPassive : public ComponentBase {
    public:
        explicit ComponentPassive(const String& name);

        void Init() override;
        void Destroy() override;

        KsCmdResult ReceiveCommand(const CommandMessage& message) override;
    };

}

