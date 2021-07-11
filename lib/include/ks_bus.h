#pragma once

#include "ks_vector.h"
#include "ks_component_base.h"

namespace kronos {

    class BusBase {
    public:
        BusBase(KsOpcode opcode, const String& name);

        virtual void AddReceivingComponent(ComponentBase* component) = 0;

        virtual void Publish(const EventMessage& message) const = 0;

        String GetName();

    protected:
        KsOpcode m_Opcode;
        String m_Name;
    };

    class BusSync : public BusBase {
    public:
        BusSync(KsOpcode opcode, const String& name);

        void AddReceivingComponent(ComponentBase* component) override;

        void Publish(const EventMessage& message) const override;

        template<typename R>
        R* PublishSync(const EventMessage& message) {
            if (m_ReceivingComponent == nullptr) {
                // TODO: HANDLE ERROR OR WARNING
                return static_cast<R*>(KS_CMDRESULT_NORETURN);
            }

            if (m_Opcode != message.opcode) {
                // TODO: HANDLE ERROR OR WARNING
                return static_cast<R*>(KS_CMDRESULT_NORETURN);
            }

            return static_cast<R*>(m_ReceivingComponent->ReceiveEvent(message));
        }

        template<typename T, typename R>
        R* PublishSync(T* data = nullptr) {
            if (m_ReceivingComponent == nullptr) {
                // TODO: HANDLE ERROR OR WARNING
                return static_cast<R*>(KS_CMDRESULT_NORETURN);
            }

            EventMessage message;
            message.opcode = m_Opcode;

            if (data != nullptr) {
                T* newData = new T;
                *newData = *data;
                message.data = reinterpret_cast<void*>(newData);
                message.dataSize = sizeof(T);
            }

            return PublishSync<R>(message);
        }

    private:
        ComponentBase* m_ReceivingComponent = nullptr;
    };

    class BusAsync : public BusBase {
    public:
        BusAsync(KsOpcode opcode, const String& name);

        void AddReceivingComponent(ComponentBase* component) override;

        void Publish(const EventMessage& message) const override;

        void PublishAsync(const EventMessage& message);

        template<typename T>
        void PublishAsync(T* data, BusBase* returnBus = nullptr) {
            if (m_ReceivingComponents.Size() == 0) {
                // TODO: HANDLE ERROR OR WARNING
                return;
            }

            EventMessage message;
            message.opcode = m_Opcode;
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
        Vector<ComponentBase*> m_ReceivingComponents;
    };

}
