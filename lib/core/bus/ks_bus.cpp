#include "ks_bus.h"

namespace kronos {

    // ==================== Base Bus ====================

    BusBase::BusBase(String name, KsEventCodeType eventCode)
        : m_EventCode(eventCode), m_Name(std::move(name)) {}

    const String& BusBase::GetName() const {
        return m_Name;
    }

    KsEventCodeType BusBase::GetEventCode() const {
        return m_EventCode;
    }

    // ==================== Synchronous Bus ====================

    BusSync::BusSync(String name, KsEventCodeType eventCode)
        : BusBase(std::move(name), eventCode) {}

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

    BusAsync::BusAsync(String name, KsEventCodeType opcode)
        : BusBase(std::move(name), opcode) {}

    void BusAsync::AddReceivingComponent(ComponentBase* component) {
        if (std::find(
            m_ReceivingComponents.begin(),
            m_ReceivingComponents.end(),
            component
        ) != m_ReceivingComponents.end()) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }
        m_ReceivingComponents.push_back(component);
    }

    void BusAsync::Publish(const EventMessage& message) const {
        if (m_ReceivingComponents.empty()) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }

        if (m_EventCode != message.eventCode) {
            // TODO: HANDLE ERROR OR WARNING
            return;
        }

        for (auto component: m_ReceivingComponents) {
            component->ReceiveEvent(message);
        }
    }

    void BusAsync::PublishAsync(const EventMessage& message) const {
        Publish(message);
    }

}