#pragma once

// Microchip ASF
#include "asf.h"

// Kronos includes
#include "ks_vector.h"
#include "ks_string.h"
#include "ks_codes.h"

#include "ks_component_base.h"

namespace kronos {

    /// Class implementation for Passive components. These components don't have a thread or a queue.
    class ComponentPassive : public ComponentBase {
    public:

        /// @copydoc
        explicit ComponentPassive(const String& name);

        /// @copydoc
        void Init() override;

        /// @copydoc
        void Destroy() override;

        /// @copydoc
        KsCmdResult ReceiveEvent(const EventMessage& message) override;
    };

}

