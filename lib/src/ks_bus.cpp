#include "ks_bus.h"

namespace kronos {

    Bus::Bus(uint16_t opcode, const String& name)
        : m_Opcode(opcode), m_Name(name) {}

    void Bus::AddReceivingComponent(ComponentBase* component) {
        m_ReceivingComponents.Add(component);
    }

    void Bus::Publish(const CommandMessage& message) {
        if (message.opcode != m_Opcode) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }

        for(size_t i = 0; i < m_ReceivingComponents.Size(); i ++) {
            ComponentBase* component = m_ReceivingComponents[i];
            component->ReceiveCommand(message);
        }
    }

    String Bus::GetName() {
        return m_Name;
    }

}