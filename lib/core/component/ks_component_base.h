// ==================================================================================
// \title ks_component_base.h
// \brief The base for the component classes.
// ==================================================================================

#pragma once

#include "ks_error_codes.h"
#include "ks_event_codes.h"
#include "ks_types.h"

#include <string>

namespace kronos {

    class BusBase;

    //! \struct EventMessage
    //! \brief A struct that holds information about an event message
    //!
    //! This struct holds information about the event message such as the code, the data, and the return bus
    struct EventMessage {
        //! Identifier for the event. This allows the user to process the event properly
        KsEventCodeType eventCode = ks_event_invalid;
        //! The data being passed through the event
        void* data = nullptr;
        //! The size of the data being passed if necessary
        size_t dataSize = 0;
        //! The return bus. This is only used for asynchronous buses as synchronous buses allow you to return values right away
        BusBase* returnBus = nullptr;
    };

    //! \class ComponentBase
    //! \brief A class that implements the base of all components
    //!
    //! This class is used as the basic block for all components
    class ComponentBase {
    public:
        //! \brief Creates a new base component
        //!
        //! \param name the name of the component
        explicit ComponentBase(const std::string& name);

        //! \brief initializes the component
        //!
        //! \return KS_SUCCESS if there was no errors
        virtual KsResultType Init() = 0;

        //! \brief destroys the component
        //!
        //! \return KS_SUCCESS if there was no errors
        virtual KsResultType Destroy() = 0;

        //! \brief Receives the event from the publishing bus
        //!
        //! \param message the event message containing the information being published on the bus
        //! \return KS_SUCCESS if there are no errors.
        virtual KsCmdResult ReceiveEvent(const EventMessage& message) = 0;

        //! \brief Processes the event message
        //!
        //! \param message the event message containing the information that was published to the bus
        //! \return KS_SUCCESS if there are no errors
        virtual KsCmdResult ProcessEvent(const EventMessage& message) = 0;

        //! \brief gets the name of the component
        //! \return
        std::string GetName();

    protected:
        //! The name of the component
        const std::string m_Name;
    };

}