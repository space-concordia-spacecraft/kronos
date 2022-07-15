#pragma once

#include "ks_component_base.h"

#include <algorithm>
#include <vector>

namespace kronos {

    //! \class BusBase
    //! \brief Base implementation for buses
    //!
    //! This class implements all the base functions used in both Async and Sync buses.
    class BusBase {
    public:
        //! \brief Constructor to create a new bus
        //!
        //! \param opcode the opcode the bus uses to publish
        //! \param name the name of the bus
        BusBase(KsEventCode opcode, const std::string& name);

        //! \brief Adds a new subscriber to the bus
        //!
        //! \param component pointer to the component that is subscribing to the bus
        virtual void AddReceivingComponent(ComponentBase* component) = 0;

        //! \brief Sends an event message to all subscribed components
        //!
        //! \param message reference to the event message being sent to the components
        virtual void Publish(const EventMessage& message) const = 0;

        //! \brief Getter for the name of the bus
        //!
        //! \return the name of the bus
        std::string GetName();

    protected:
        //! Event code that gets sent to all subscribed components
        KsEventCode m_EventCode;

        //! Name of the bus
        std::string m_Name;
    };

    //! \class BusSync
    //! \brief A class that implements synchronous buses
    //!
    //! This class implements synchronous buses. These are able to return values
    class BusSync : public BusBase {
    public:
        //! @copydoc
        BusSync(KsEventCode opcode, const std::string& name);

        //! @copydoc
        void AddReceivingComponent(ComponentBase* component) override;

        //! @copydoc
        void Publish(const EventMessage& message) const override;

        template<typename R>
        //! \brief Publishes the message synchronously to the subscribed components
        //!
        //! \tparam R type of the return
        //! \param message reference to the event message being sent to the components
        //! \return
        R* PublishSync(const EventMessage& message) {
            if (m_ReceivingComponent == nullptr) {
                // TODO: HANDLE ERROR OR WARNING
                return static_cast<R*>(KS_CMDRESULT_NORETURN);
            }

            if (m_EventCode != message.eventCode) {
                // TODO: HANDLE ERROR OR WARNING
                return static_cast<R*>(KS_CMDRESULT_NORETURN);
            }

            return static_cast<R*>(m_ReceivingComponent->ReceiveEvent(message));
        }

        template<typename T, typename R>
        //! \brief
        //! \tparam T
        //! \tparam R
        //! \param data
        //! \return
        R* PublishSync(T* data = nullptr) {
            if (m_ReceivingComponent == nullptr) {
                // TODO: HANDLE ERROR OR WARNING
                return static_cast<R*>(KS_CMDRESULT_NORETURN);
            }

            EventMessage message;
            message.eventCode = m_EventCode;

            if (data != nullptr) {
                T* newData = new T;
                *newData = *data;
                message.data = reinterpret_cast<void*>(newData);
                message.dataSize = sizeof(T);
            }

            return PublishSync<R>(message);
        }

    private:
        //!
        ComponentBase* m_ReceivingComponent = nullptr;
    };

    class BusAsync : public BusBase {
    public:
        BusAsync(KsEventCode opcode, const std::string& name);

        void AddReceivingComponent(ComponentBase* component) override;

        void Publish(const EventMessage& message) const override;

        void PublishAsync(const EventMessage& message);

        template<typename T>
        void PublishAsync(T* data, BusBase* returnBus = nullptr) {
            if (m_ReceivingComponents.size() == 0) {
                // TODO: HANDLE ERROR OR WARNING
                return;
            }

            EventMessage message;
            message.eventCode = m_EventCode;
            message.returnBus = returnBus;

            if (data != nullptr) {
                T* newData = new T;
                *newData = *data;
                message.data = reinterpret_cast<void*>(newData);
                message.dataSize = sizeof(T);
            }

            PublishAsync(message);
        }

    private:
        std::vector<ComponentBase*> m_ReceivingComponents;
    };

}
