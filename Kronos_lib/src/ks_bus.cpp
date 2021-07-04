#include "ks_bus.h"

namespace kronos {

    Bus::Bus(uint16_t opcode, String name)
        : m_Opcode(opcode), m_Name(name) {}

    void Bus::AddComponent(ComponentPassive* component) {
        m_Components.Add(component);
    }
    void Bus::AddReceivingComponent(ComponentPassive * component) {
        m_ReceivingComponents.add(component);
    }

    void Bus::Publish(CommandMessage message) {
        if (message.opcode != m_Opcode) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }

        for(int i = 0; i < m_ReceivingComponents.size(); i ++) {
            m_ReceivingComponents[i]->ReceiveCommand(message);
        }
    }

}