#pragma once

#include "ks_string.h"
#include "ks_opcodes.h"

namespace kronos {

    class BusBase;

    /// Contains information about an event. This information includes: opcode, the data, the size of the data, and the return bus if needed.
    struct EventMessage {
        /// Identifier for the event. This allows the user to process the event properly.
        KsEventCode eventCode = KS_EVENT_CODE_INVALID;

        /// The data being passed through the event.
        void * data = nullptr;

        /// The size of the data being passed if necessary.
        size_t dataSize = 0;

        /// The return bus. This is only used for asynchronous buses as synchronous buses allow you to return values right away.
        BusBase * returnBus = nullptr;
    };

    /// Base class for all components. Contains the definition of all base functions but they have no implementation.
    class ComponentBase {
    public:
        /**
         * Constructor for the class. All inheriting classes must make sure to pass in the component name.
         * @param name - Name for the component.
         */
        explicit ComponentBase(const String& name);

        /// Initializer for the component.
        virtual void Init() = 0;

        /// Destroyer for the component.
        virtual void Destroy() = 0;

        /**
         * Function called when an event is published into a bus. The component must be registered in the bus for this function to be called.
         * @param message The event message containing all the information.
         * @return KS_SUCCESS if there are no errors. Otherwise it returns an error.
         */
        virtual KsCmdResult ReceiveEvent(const EventMessage& message) = 0;

        /**
         * Function used to process an event.
         * @param message The event message containing all the information.
         * @return KS_SUCCESS if there are no errors. Otherwise it returns an error.
         */
        virtual KsCmdResult ProcessEvent(const EventMessage& message) = 0;

        /**
         * Getter for the name of the component.
         * @return The name of the component.
         */
        String GetName();
    protected:

        /// Name of the component.
        const String m_Name;
    };

}