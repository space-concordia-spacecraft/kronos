#include "ks_bus.h"

namespace kronos {

    // ==================== Base Bus ====================

    BusBase::BusBase(KsEventCode eventCode, const String& name)
        : m_EventCode(eventCode), m_Name(name) {}

    String BusBase::GetName() {
        return m_Name;
    }

    // ==================== Synchronous Bus ====================

    BusSync::BusSync(KsEventCode eventCode, const String& name)
        : BusBase(eventCode, name) {}

    void BusSync::AddReceivingComponent(ComponentBase* component) {
        if (m_ReceivingComponent != nullptr) {
            // TODO: HANDLE ERROR
            return;
        }
        m_ReceivingComponent = component;
    }

    void BusSync::Publish(const EventMessage& message) const {
        if (m_ReceivingComponent == nullptr) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }

        if (m_EventCode != message.eventCode) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }

        m_ReceivingComponent->ReceiveEvent(message);
    }


    // ==================== Asynchronous Bus ====================

    BusAsync::BusAsync(KsEventCode opcode, const String& name)
        : BusBase(opcode, name) {}

    void BusAsync::AddReceivingComponent(ComponentBase* component) {
        if (m_ReceivingComponents.Find(component) >= 0) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }
        m_ReceivingComponents.Add(component);
    }

    void BusAsync::Publish(const EventMessage& message) const {
        if (m_ReceivingComponents.Size() == 0) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }

        if (m_EventCode != message.eventCode) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }

        for (size_t i = 0; i < m_ReceivingComponents.Size(); i ++) {
            ComponentBase* component = m_ReceivingComponents[i];
            component->ReceiveEvent(message);
        }
    }

    void BusAsync::PublishAsync(const EventMessage& message) {
        Publish(message);
    }

}