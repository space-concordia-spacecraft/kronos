#include "ks_bus.h"
#include "ks_logger.h"

namespace kronos {
    Bus::Bus(String name)
        : m_Name(std::move(name)) {}

    const String& Bus::GetName() const {
        return m_Name;
    }

    void Bus::AddReceivingComponent(ComponentBase* component) {
        KS_LIST_FIND(m_ReceivingComponents, component, it) {
            return;
        }

        m_ReceivingComponents.push_back(component);
    }

    void Bus::Publish(const EventMessage* message) const {
        if (m_ReceivingComponents.empty()) {
            KS_ASSERT("There are no components registered to this bus.");
            return;
        }

        for (auto component: m_ReceivingComponents) {
            component->ReceiveEvent(message);
        }
    }
}