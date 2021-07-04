#pragma once

#include "ks_vector.h"
#include "ks_component_passive.h"

namespace kronos {

    class Bus {
    public:
        Bus(uint16_t opcode, String name);

        void AddComponent(ComponentPassive * component);
        void RemoveComponent(ComponentPassive * component);
        void Publish(CommandMessage message);
    private:
        Vector<ComponentPassive*> m_Components;
        uint16_t m_Opcode;
        String m_Name;
    };

}
