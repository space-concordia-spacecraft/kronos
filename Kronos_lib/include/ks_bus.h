#pragma once

#include "ks_vector.h"
#include "ks_component_passive.h"

namespace kronos {

    class Bus {
    public:
        Bus(uint16_t opcode, String name);

        void AddReceivingComponent(ComponentPassive * component);
        void Publish(CommandMessage message);
    private:
        Vector<ComponentPassive*> m_ReceivingComponents;
        uint16_t m_Opcode;
        String m_Name;
    };

}
