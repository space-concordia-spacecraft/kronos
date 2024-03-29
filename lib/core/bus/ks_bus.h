#pragma once

#include "ks_component_base.h"
#include "ks_framework.h"

namespace kronos {

    //! \class Bus
    //! \brief Implementation of busses used for cross-task communication.
    //!
    //! This class implements all the base functions used in both Async and Sync buses.
    class Bus {

    public:
        //! \brief Constructor to create a new bus
        //!
        //! \param name the name of the bus
        explicit Bus(String name);

        //! \brief Virtual destructor to be invoked for proper destruction of child classes.
        ~Bus() = default;

        //! \brief Adds a new subscriber to the bus
        //!
        //! \param component pointer to the component that is subscribing to the bus
        KsResult AddReceivingComponent(ComponentBase* component);

        //! \brief
        //!
        //! \tparam T
        //! \param data
        //! \param returnBus
        template<typename T>
        KsResult Publish(const T& data, KsEventCodeType eventCode, Bus* returnBus = nullptr) {
            if (m_ReceivingComponents.empty())
                KS_THROW(ks_error_bus_no_subscribers);

            for (auto component: m_ReceivingComponents) {
                EventMessage* message = Framework::CreateEventMessage<T>(data, eventCode, returnBus);
                KS_TRY(ks_error_bus_publish, component->ReceiveEvent(message));
            }

            return ks_success;
        }

        //! \brief
        //!
        //! \tparam T
        //! \param data
        //! \param returnBus
        template<typename T>
        KsResult Publish(T&& data, KsEventCodeType eventCode, Bus* returnBus = nullptr) {
            if (m_ReceivingComponents.empty()) KS_THROW(ks_error_bus_no_subscribers);

            for (auto component: m_ReceivingComponents) {
                EventMessage* message = Framework::CreateEventMessage<T>(std::forward<T>(data), eventCode, returnBus);
                component->ReceiveEvent(message);
            }

            return ks_success;
        }

        //! \brief
        //!
        //! \param returnBus
        KsResult Publish(KsEventCodeType eventCode, Bus* returnBus = nullptr) {
            if (m_ReceivingComponents.empty()) KS_THROW(ks_error_bus_no_subscribers);

            for (auto component: m_ReceivingComponents) {
                EventMessage* message = Framework::CreateEventMessage(eventCode, returnBus);
                component->ReceiveEvent(message);
            }

            return ks_success;
        }

        //! \brief Getter for the name of the bus
        //!
        //! \return the name of the bus
        [[nodiscard]] const String& GetName() const;

    protected:
        //! Name of the bus.
        String m_Name;

        //! A list of components subscribed to the bus.
        List<ComponentBase*> m_ReceivingComponents;

    };

}
