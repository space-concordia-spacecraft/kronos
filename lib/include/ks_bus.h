#pragma once

#include "ks_vector.h"
#include "ks_component_base.h"

namespace kronos {

    class Bus {
    public:
        Bus(uint16_t opcode, const String& name);

        void AddReceivingComponent(ComponentBase * component);
        void Publish(const CommandMessage& message);

        String GetName();

    private:
        Vector<ComponentBase*> m_ReceivingComponents;
        uint16_t m_Opcode;
        String m_Name;
    };

}
