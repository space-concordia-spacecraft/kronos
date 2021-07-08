#pragma once

// Microchip ASF
#include "asf.h"

// Kronos includes
#include "ks_vector.h"
#include "ks_string.h"
#include "ks_opcodes.h"

#include "ks_component_base.h"

namespace kronos {

    class ComponentPassive : public ComponentBase{
    public:
        explicit ComponentPassive(const String& name);

        virtual void Init() override;
        virtual void Destroy() override;

        virtual void ReceiveCommand(const CommandMessage& message) override;
    };

}

