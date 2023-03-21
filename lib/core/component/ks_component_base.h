// ==================================================================================
// \title ks_component_base.h
// \brief The base for the component classes.
// ==================================================================================
#pragma once

namespace kronos {

    class Bus;

    //! \struct EventMessage
    //! \brief A struct that holds information about an event message
    //!
    //! This struct holds information about the event message such as the code, the data, and the return bus
    struct EventMessage {
        //! Identifier for the event. This allows the user to process the event properly
        KsEventCodeType eventCode = ks_event_invalid;
        //! The data being passed through the event
        std::any data{};
        //! The return bus. This is only used for asynchronous buses as synchronous buses allow you to return values right away
        Bus* returnBus = nullptr;

        template<typename T>
        T Cast() const {
            return std::any_cast<T>(data);
        }
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
        explicit ComponentBase(String name);

        //! \brief Virtual destructor
        virtual ~ComponentBase() = default;

        //! \brief initializes the component
        //!
        //! \return KS_SUCCESS if there was no errors
        virtual ErrorOr<void> Init() = 0;

        //! \brief This runs after all components have been initialized
        //!
        //! \return KS_SUCCESS if there was no errors
        virtual ErrorOr<void> PostInit() = 0;

        //! \brief destroys the component
        //!
        //! \return KS_SUCCESS if there was no errors
        virtual ErrorOr<void> Destroy() = 0;

        //! \brief Receives the event from the publishing bus
        //!
        //! \param message the event message containing the information being published on the bus
        virtual ErrorOr<void> ReceiveEvent(const EventMessage* message) = 0;

        //! \brief Processes the event message
        //!
        //! \param message the event message containing the information that was published to the bus
        virtual ErrorOr<void> ProcessEvent(const EventMessage& message) = 0;

        //! \brief gets the name of the component
        //! \return
        [[nodiscard]] const String& GetName() const;

    protected:
        //! The name of the component
        const String m_Name;
    };

}