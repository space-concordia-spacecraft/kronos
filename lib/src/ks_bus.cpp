#include "ks_bus.h"

namespace kronos {

    // ==================== Base Bus ====================

    BusBase::BusBase(KsEventCode eventCode, const std::string& name)
            : m_EventCode(eventCode), m_Name(name) {}

    std::string BusBase::GetName() {
        return m_Name;
    }

    // ==================== Synchronous Bus ====================

    BusSync::BusSync(KsEventCode eventCode, const std::string& name)
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

    BusAsync::BusAsync(KsEventCode opcode, const std::string& name)
            : BusBase(opcode, name) {}

    void BusAsync::AddReceivingComponent(ComponentBase* component) {
        if (std::find(
                m_ReceivingComponents.begin(),
                m_ReceivingComponents.end(),
                component) != m_ReceivingComponents.end()) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }
        m_ReceivingComponents.push_back(component);
    }

    void BusAsync::Publish(const EventMessage& message) const {
        if (m_ReceivingComponents.size() == 0) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }

        if (m_EventCode != message.eventCode) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }

        for (auto component : m_ReceivingComponents) {
            component->ReceiveEvent(message);
        }
    }

    void BusAsync::PublishAsync(const EventMessage& message) {
        Publish(message);
    }

}