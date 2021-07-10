#include "ks_bus.h"

namespace kronos {

    // ==================== Base Bus ====================

    BusBase::BusBase(KsOpcode opcode, const String& name)
        : m_Opcode(opcode), m_Name(name) {}

    String BusBase::GetName() {
        return m_Name;
    }

    // ==================== Synchronous Bus ====================

    BusSync::BusSync(KsOpcode opcode, const String& name)
        : BusBase(opcode, name) {}

    void BusSync::AddReceivingComponent(ComponentBase* component) {
        if (m_ReceivingComponent != nullptr) {
            // TODO: HANDLE ERROR
            return;
        }
        m_ReceivingComponent = component;
    }

    void BusSync::Publish(const CommandMessage& message) const {
        if (m_ReceivingComponent == nullptr) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }

        if (m_Opcode != message.opcode) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }

        m_ReceivingComponent->ReceiveCommand(message);
    }


    // ==================== Asynchronous Bus ====================

    BusAsync::BusAsync(KsOpcode opcode, const String& name)
        : BusBase(opcode, name) {}

    void BusAsync::AddReceivingComponent(ComponentBase* component) {
        if (m_ReceivingComponents.Find(component) >= 0) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }
        m_ReceivingComponents.Add(component);
    }

    void BusAsync::Publish(const CommandMessage& message) const {
        if (m_ReceivingComponents.Size() == 0) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }

        if (m_Opcode != message.opcode) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }

        for (size_t i = 0; i < m_ReceivingComponents.Size(); i ++) {
            ComponentBase* component = m_ReceivingComponents[i];
            component->ReceiveCommand(message);
        }
    }

    void BusAsync::PublishAsync(const CommandMessage& message) {
        Publish(message);
    }

}