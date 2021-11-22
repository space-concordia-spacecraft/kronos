// ==================================================================================
// \title ks_component_passive.h
// \brief A component that implements the base for the passive component
// ==================================================================================

#pragma once

// Microchip ASF
#include "asf.h"

// Kronos includes
#include "ks_vector.h"
#include "ks_string.h"
#include "ks_codes.h"

#include "ks_component_base.h"

namespace kronos {

    //! \class ComponentPassive
    //! \brief A class that implements the base for all passive components
    //!
    //! This class is used as the base block for all passive components
    class ComponentPassive : public ComponentBase {
    public:
        //! \brief Creates a new passive component
        //!
        //! \param name the name of the component
        explicit ComponentPassive(const String& name);

        /// @copydoc
        KsResult Init() override;

        /// @copydoc
        KsResult Destroy() override;

        /// @copydoc
        KsCmdResult ReceiveEvent(const EventMessage& message) override;
    };

}

