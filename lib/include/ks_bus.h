#pragma once

#include "ks_vector.h"
#include "ks_component_base.h"

namespace kronos {

    class BusBase {
    public:
        BusBase(KsOpcode opcode, const String& name);

        virtual void AddReceivingComponent(ComponentBase* component) = 0;

        virtual void Publish(const CommandMessage& message) = 0;

        String GetName();

    protected:
        KsOpcode m_Opcode;
        String m_Name;
    };

    class BusSync : public BusBase {
    public:
        BusSync(KsOpcode opcode, const String& name);

        void AddReceivingComponent(ComponentBase* component) override;

        void Publish(const CommandMessage& message) override;

        template<typename T>
        T* PublishSync(const CommandMessage& message);

        template<typename T, typename R>
        R* PublishSync(T* data = nullptr);

    private:
        ComponentBase* m_ReceivingComponent = nullptr;
    };

    class BusAsync : public BusBase {
    public:
        BusAsync(KsOpcode opcode, const String& name);

        void AddReceivingComponent(ComponentBase* component) override;

        void Publish(const CommandMessage& message) override;

        void PublishAsync(const CommandMessage& message);

        template<typename T>
        void PublishAsync(T* data, BusBase* returnBus);

    private:
        Vector<ComponentBase*> m_ReceivingComponents;
    };

}
