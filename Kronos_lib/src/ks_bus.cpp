#include "ks_bus.h"

namespace kronos {

    Bus::Bus(uint16_t opcode, String name)
        : m_Opcode(opcode), m_Name(name) {}

    void Bus::AddComponent(ComponentPassive* component) {
        m_Components.Add(component);
    }

    void Bus::RemoveComponent(ComponentPassive* component) {
        m_Components.Remove(component);
    }

    void Bus::Publish(CommandMessage message) {
        if (message.opcode != m_Opcode) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }

        for (int i = 0; i < m_Components.Size(); i++) {
            m_Components[i]->ReceiveCommand(message);
        }
    }

}