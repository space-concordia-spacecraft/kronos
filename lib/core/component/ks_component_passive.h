// ==================================================================================
// \title ks_component_passive.h
// \brief A component that implements the base for the passive component
// ==================================================================================

#pragma once

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
        ErrorOr<void> Init() override;

        /// @copydoc
        ErrorOr<void> PostInit() override;

        /// @copydoc
        ErrorOr<void> Destroy() override;

        /// @copydoc
        ErrorOr<void> ReceiveEvent(const EventMessage* message) override;

        //!
        ErrorOr<void> ProcessEvent(const EventMessage& message) override {};
    };

}

